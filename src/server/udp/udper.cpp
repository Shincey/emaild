#include "udper.h"

#include <sys/socket.h>

namespace core {
    lib::zpool<UDPer> g_udper_pool;


    UDPer * UDPer::create(zif::iUDPSession *__session, const std::string &__ip, const s32 __port) {
        u32 fd = socket(AF_INET, SOCK_DGRAM, 0);

        if (-1 == setreuse(fd) ||
            -1 == setsocksendbuff(fd, 0) ||
            -1 == setsockrecvbuff(fd, 0) ||
            -1 == setnonblocking(fd)) {
            ::close(fd);
            fd = -1;
            __session->on_create(false);
            return nullptr;
        }
        
        struct sockaddr_in addr;
        ::bzero(&addr, sizeof(addr));
        addr.sin_family = AF_INET;
        inet_pton(AF_INET, __ip.c_str(), &addr.sin_addr);
        addr.sin_port = htons(__port);

        if (int ret = ::bind(fd, (struct sockaddr *)&addr, sizeof(addr)); -1 == ret) {
            ::close(fd);
            fd = -1;
            __session->on_create(false);
            return nullptr;
        }

        UDPer *p = create_from_pool(g_udper_pool, __session, __ip, __port);
        p->socket_ = fd;

        struct epoll_event ev;
        ev.data.ptr = (void *)&p->associate_;
        ev.events = EPOLLIN;
        if (-1 == epoll_ctl(g_epoller_fd, EPOLL_CTL_ADD, fd, &ev)) {
            ::close(fd);
            fd = -1;
            recover_to_pool(g_udper_pool, p);
            __session->on_create(false);
            return nullptr;
        }
        tool::mem::s_memcpy((void *)&__session->pipe_, sizeof(void *), &p, sizeof(void *));
        __session->on_create(true);
        return p;
    }

    UDPer::UDPer(zif::iUDPSession *__session, const std::string &__ip, const s32 __port) 
        : session_(__session),
          socket_(-1), 
          is_cache_(false), associate_(eCompletion::IO, this) ,addr_(__ip, __port) {}

    void UDPer::cache() {}
    void UDPer::load() {}

    void UDPer::close() {
        if (-1 != socket_) {
            ::close(socket_);
            //socket_ = -1;                    // need to be considered
            epoll_ctl(g_epoller_fd, EPOLL_CTL_DEL, socket_, 0);
            tool::mem::s_memset((void *)&session_->pipe_, sizeof(void *), 0, sizeof(void *));
            session_->on_close(Core::instance());
            recover_to_pool(g_udper_pool, this);
        }
    }

    void UDPer::sendto(const char *__ip, const s32 __port, const void *__content, const s32 __size) {
        struct sockaddr_in addr;
        bzero(&addr, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(__port);
        inet_pton(AF_INET, __ip, (void *)&addr.sin_addr.s_addr);
        s32 rel = ::sendto(socket_, __content, __size, 0, (struct sockaddr *)&addr, sizeof(addr));
        debug(Core::instance(), "UDPer sendto %d bytes", rel);
    }

    void UDPer::on_completer(zAssociate *__ac, const eCompletion __type, const struct epoll_event &__ev) {
        zassert(__type == eCompletion::IO, "udper oncompleter type error");
        s64 tick = tool::time::microseconds();
        do {
            struct sockaddr_in addr;
            bzero(&addr, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = htons(addr_.port_);
            inet_pton(AF_INET, addr_.ip_.c_str(), &addr.sin_addr.s_addr);
            s32 addr_len = sizeof(addr);
            s32 len = ::recvfrom(socket_, recv_temp_, sizeof(recv_temp_), 0, (struct sockaddr *)&addr, (socklen_t *)addr_len);
            if (len > 0) {
                session_->on_recv(Core::instance(), inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), recv_temp_, len);
            } else {
                if (len <= 0 && errno == EAGAIN) { return; }
                else { this->close(); }
            }
        } while (tool::time::microseconds() - tick < 1000);
    }
}