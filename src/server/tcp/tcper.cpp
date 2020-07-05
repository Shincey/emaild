#include "tcper.h"

#define MAX_PIPE_SIZE (20*1024*1024)
#define RECV_TMP_SIZE 4096

namespace core {

    lib::zpool<Accepter> g_accerpter_pool;
    lib::zpool<TCPer> g_tcper_pool;
    lib::zpool<sAssociation> g_associate_pool;


    Accepter::Accepter(zif::iTCPServer * __server, const char *__ip, const s32 __port, s32 __ssize, s32 __rsize)
    : server_(__server), socket_(-1), address_(__ip, __port), ssize_(__ssize <= MAX_PIPE_SIZE ? __ssize : MAX_PIPE_SIZE),
      rsize_(__rsize <= MAX_PIPE_SIZE ? __rsize : MAX_PIPE_SIZE), association_(eEpollEventType::Accept, this)
    {

    }

    Accepter * Accepter::create(zif::iTCPServer * __server, const char *__ip, const s32 __port, s32 __ssize, s32 __rsize) {
        zassert(__server, "something wrong with server");
        //struct timeval tv;
        struct sockaddr_in addr;
        bzero(&addr, sizeof(addr));
        addr.sin_family = AF_INET;
        inet_pton(AF_INET, __ip, &addr.sin_addr);
        addr.sin_port = htons(__port);

        s32 sockfd = -1;
        if (-1 == (sockfd = socket(AF_INET, SOCK_STREAM, 0)) ||
            -1 == setreuse(sockfd) ||
            -1 == setnonblocking(sockfd) ||
            -1 == ::bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) ||
            -1 == ::listen(sockfd, 200)) {

            sockfd = -1;
            return nullptr;
        }

        Accepter *ac = create_from_pool(g_accerpter_pool, __server, __ip, __port, __ssize, __rsize);
        ac->socket_ = sockfd;
        epoll_event ev;
        ev.data.ptr = (void *)&ac->association_;
        ev.events = EPOLLIN;
        
        if (-1 == epoll_ctl(g_epoller_fd, EPOLL_CTL_ADD, sockfd, &ev)) {
            ac->release();
            return nullptr;
        }
        
        return ac;
    }

    void Accepter::on_epoll_event(sAssociation *__association, const eEpollEventType __type, const struct epoll_event &__ev) {
        zassert(eEpollEventType::Accept == __type, "Accepter on Completer error");
        if (__ev.events & EPOLLIN) {
            struct sockaddr_in addr_in;
            socklen_t len = sizeof(addr_in);
            s32 sockfd = -1;
            s32 time = 0;
            while ((sockfd = ::accept(socket_, (struct sockaddr *)&addr_in, &len)) >= 0 && time++ <= 256) {
                if (-1 != setnonblocking(sockfd) ||
                    -1 != settcpnodelay(sockfd) ||
                    -1 != setreuse(sockfd) ||
                    -1 != setsocksendbuff(sockfd, 0) ||
                    -1 != setsockrecvbuff(sockfd, 0)) {

                    struct sockaddr_in sai;
                    socklen_t len = sizeof(sai);
                    ::getpeername(sockfd, (struct sockaddr *)&sai, (socklen_t *)&len);
                    const std::string ip = ::inet_ntoa(sai.sin_addr);
                    const s32 port = ::ntohs(sai.sin_port);

                    zif::iTCPSession *session = server_->on_malloc_connection(Core::instance(), ip.c_str(), port);
                    if (!session) {
                        server_->on_error(Core::instance(), session);
                        ::close(sockfd);
                        sockfd = -1;
                        continue;
                    }

                    session->address_.ip = ip;
                    session->address_.port = port;

                    TCPer *tcp = create_from_pool(g_tcper_pool, session, ip, port, ssize_, rsize_);
                    struct epoll_event ev;
                    ev.events = EPOLLIN;
                    if (-1 == epoll_ctl(g_epoller_fd, EPOLL_CTL_ADD, sockfd, &ev)) {
                        server_->on_error(Core::instance(), session);
                        ::close(sockfd);
                        recover_to_pool(g_tcper_pool, tcp);
                        continue;
                    }
                    tcp->connected_ = true;
                    tcp->socket_ = sockfd;
                    zif::iTCPPipe *pipe = tcp;
                    tool::mem::s_memcpy((void *)&session->pipe_, sizeof(void *), &pipe, sizeof(void *));
                    session->on_connect(Core::instance());
                    continue;
                }
                ::close(sockfd);
                sockfd = -1;
            }  // while
        } else { //if (__ev.events & EPOLLIN)
            // do something
        }
    }

    void Accepter::release() {
        epoll_ctl(g_epoller_fd, EPOLL_CTL_DEL, socket_, nullptr);
        ::close(socket_);
        server_->on_release(Core::instance());
        recover_to_pool(g_accerpter_pool, this);
    }


















    // TCPer class implementation

    TCPer::TCPer(zif::iTCPSession *__session, const std::string &__ip, const s32 __port, const s32 __ssize, const s32 __rsize)
        :session_(__session), peer_address_(__ip, __port),
         connected_(false), caching_(false),
         association_(eEpollEventType::IO, this), socket_(-1),
         send_buff_(__ssize), recv_buff_(__ssize)
    {
    }

    TCPer * TCPer::create(zif::iTCPSession *__session, const std::string &__host, const s32 __port, s32 __ssize, s32 __rsize) {
        (__ssize <= MAX_PIPE_SIZE) ? : (__ssize = MAX_PIPE_SIZE);
        (__rsize <= MAX_PIPE_SIZE) ? : (__rsize <= MAX_PIPE_SIZE);

        const s32 host_len = __host.size() + 1;
        char *tmp = (char *)alloca(host_len);
        tool::mem::s_memcpy((void *)tmp, host_len, __host.c_str(), host_len);

        static const bool initiative = true;
        tool::mem::s_memcpy((void *)&__session->initiative_, sizeof(bool), (void *)&initiative, sizeof(bool));

        std::string ip;
        if (!get_ip_by_host(__host.c_str(), ip)) {
            __session->on_connect_failed(Core::instance());
            return nullptr;
        }

        __session->address_.ip = ip;
        __session->address_.port = __port;

        struct timeval tv;
        struct sockaddr_in addr;
        bzero(&addr, sizeof(struct sockaddr_in));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(__port);

        s32 sockfd = -1;
        if (-1 == (sockfd = socket(AF_INET, SOCK_STREAM, 0)) ||
            -1 == setnonblocking(sockfd) ||
            0 >= inet_pton(AF_INET, ip.c_str(), &addr.sin_addr)) {

            ::close(sockfd);
            sockfd = -1;
            __session->on_connect_failed(Core::instance());
            return nullptr;
        }

        TCPer * tcp = create_from_pool(g_tcper_pool, __session, ip, __port, __ssize, __rsize);
        tcp->socket_ = sockfd;
        if (s32 ret = ::connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)); ret == 0) {
            struct epoll_event ev;
            ev.data.ptr = (void *)&tcp->association_;
            ev.events = EPOLLIN;
            if (-1 == epoll_ctl(g_epoller_fd, EPOLL_CTL_ADD, sockfd, &ev)) {
                __session->on_connect_failed(Core::instance());
                recover_to_pool(g_tcper_pool, tcp);
                return nullptr;
            }
            tcp->connected_ = true;

            tool::mem::s_memcpy((void *)&__session->pipe_, sizeof(void *), &tcp, sizeof(tcp));
            __session->on_connect(Core::instance());
            
        } else if (ret < 0 && errno != EINPROGRESS) {
            ::close(sockfd);
            sockfd = -1;
            __session->on_connect_failed(Core::instance());
            recover_to_pool(g_tcper_pool, tcp);
            return nullptr;
        } else {
            sAssociation * associate = create_from_pool(g_associate_pool, eEpollEventType::Connect, tcp);
            struct epoll_event ev;
            ev.data.ptr = (void *)associate;
            ev.events = EPOLLOUT;
            if (-1 == epoll_ctl(g_epoller_fd, EPOLL_CTL_ADD, sockfd, &ev)) {
                __session->on_connect_failed(Core::instance());
                ::close(sockfd);
                recover_to_pool(g_tcper_pool, tcp);
                //recover_to_pool(g_associate_pool, associate);                        ----- ???
                return nullptr;
            }
        }

        return tcp;
    }

    void case_connect(TCPer *__tcp, sAssociation *__association, const eEpollEventType __type, const struct epoll_event &__ev) {
        auto close_session = [&] () {
            ::close(__tcp->socket_);
            __tcp->socket_ = -1;
            __tcp->session_->on_connect_failed(Core::instance());
            recover_to_pool(g_tcper_pool, __tcp);
        };

        recover_to_pool(g_associate_pool, __association);
        if (-1 == settcpnodelay(__tcp->socket_)) {
            close_session();
            return;
        }

        epoll_ctl(g_epoller_fd, EPOLL_CTL_DEL, __tcp->socket_, nullptr);
        if (__ev.events &  (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
            close_session();
            return;
        }

        if (__ev.events & EPOLLOUT) {
            struct epoll_event ev;
            ev.data.ptr = (void *)&__tcp->association_;
            ev.events = EPOLLIN;
            if (-1 == epoll_ctl(g_epoller_fd, EPOLL_CTL_ADD, __tcp->socket_, &ev)) {
                close_session();
                return;
            }
            zif::iPipe *pipe = __tcp;
            tool::mem::s_memcpy((void *)&__tcp->session_->pipe_, sizeof(void *), &pipe, sizeof(void *));
            __tcp->connected_ = true;
            __tcp->session_->on_connect(Core::instance());

        } else {
            close_session();
        }
    }

    void case_io(TCPer *__tcp, sAssociation *__association, const eEpollEventType __type, const struct epoll_event &__ev) {
        auto error_handler = [&] () {
            zassert(__tcp, "tcp is null");
            if (!__tcp) return;
            ::close(__tcp->socket_);
            __tcp->socket_ = -1;
            if (__tcp->session_) {
                tool::mem::s_memcpy((void *)&__tcp->session_->pipe_, sizeof(void *), 0, sizeof(void *));
                __tcp->session_->on_disconnect(Core::instance());
                tool::mem::s_memset((void *)&__tcp->session_, sizeof(void *), 0, sizeof(void *));
            }
            recover_to_pool(g_tcper_pool, __tcp);
        };

        if (__ev.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
            error_handler();
        } else {
            if (__ev.events & EPOLLIN) {
                static char tmp[RECV_TMP_SIZE];
                zassert(__tcp->connected_, "no connected");
                if (__tcp->connected_) {
                    const s64 tick = tool::time::microseconds();
                    do {
                        s32 len = ::recv(__tcp->socket_, tmp, sizeof(tmp), 0);
                        if (len > 0) {
                            if (__tcp->recv_buff_.in(tmp, len)) {
                                while (__tcp->recv_buff_.length() > 0 && !__tcp->caching_) {
                                    s32 use = __tcp->session_->on_recv(Core::instance(), __tcp->recv_buff_.data(), __tcp->recv_buff_.length());
                                    if (!__tcp->connected_) {
                                        epoll_ctl(g_epoller_fd, EPOLL_CTL_DEL, __tcp->socket_, nullptr);
                                        error_handler();
                                        return;
                                    }
                                    if (use <= 0) {
                                        return;
                                    }
                                    __tcp->recv_buff_.out(use);
                                }
                            } else {
                                __tcp->close();
                                return;
                            }
                        } else if (len < 0 && EAGAIN == errno) {
                            return;
                        } else {
                            __tcp->close();
                            return;
                        }
                    } while(__tcp->connected_ && tool::time::microseconds() - tick <= 1000);
                }
            } //if (__ev.events & EPOLLIN)

            if (__ev.events & EPOLLOUT) {
                zassert(__tcp->connected_, "tcp connection lost");
                s32 len = ::send(__tcp->socket_, __tcp->send_buff_.data(), __tcp->send_buff_.length(), 0);
                if (len > 0) {
                    __tcp->send_buff_.out(len);
                    if (__tcp->send_buff_.length() == 0) {
                        struct epoll_event ev;
                        ev.data.ptr = (void *)&__tcp->association_;
                        ev.events = EPOLLIN;
                        if (-1 == epoll_ctl(g_epoller_fd, EPOLL_CTL_MOD, __tcp->socket_, &ev)) {
                            epoll_ctl(g_epoller_fd, EPOLL_CTL_DEL, __tcp->socket_, nullptr);
                            error_handler();
                            return;
                        }
                    }
                } else if (len <= 0 && EAGAIN != errno) {
                    epoll_ctl(g_epoller_fd, EPOLL_CTL_DEL, __tcp->socket_, nullptr);
                    error_handler();
                }
            } // if (__ev.events & EPOLLOUT)
        } // if (__ev.events & EPOLLIN)
    }



    void TCPer::on_epoll_event(sAssociation *__association, const eEpollEventType __type, const struct epoll_event &__ev) {
        switch (__type) {
            case eEpollEventType::Connect: {
                case_connect(this, __association, __type, __ev);
                break;
            }
            case eEpollEventType::IO: {
                case_io(this, __association, __type, __ev);
                break;
            }
        }
    }

    void TCPer::close() {
        if (socket_ != -1) {
            connected_ = false;
            epoll_ctl(g_epoller_fd, EPOLL_CTL_DEL, socket_, nullptr);
            ::close(socket_);
            socket_ = -1;
            if (session_) {
                tool::mem::s_memset((void *)session_->pipe_, sizeof(void *), 0, sizeof(void *));
                session_->on_disconnect(Core::instance());
                tool::mem::s_memset((void *)&session_, sizeof(void *), 0, sizeof(void *));
            }
            recover_to_pool(g_tcper_pool, this);
        }
    }

    void TCPer::cache() {

    }

    void TCPer::load() {

    }

    void TCPer::send(const void *__data, const s32 __size, bool __immediately) {
        if (!connected_) return;

        if (send_buff_.in(__data, __size)) {
            if (__immediately) {
                struct epoll_event ev;
                ev.data.ptr = (void *)&association_;
                ev.events = EPOLLIN | EPOLLOUT;
                epoll_ctl(g_epoller_fd, EPOLL_CTL_MOD, socket_, &ev);
            }
        } else {
            close();
        }
    }


}