/*
** common.h for emaild
**
** Made by shincey
** Login   <login>
**
** Started on  Sat Jun 27 下午5:33:25 2020 shincey
** Last update Sun Jun 27 下午7:39:11 2020 shincey
*/

#ifndef __SERVER_COMMOM_H__
# define __SERVER_COMMOM_H__

#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/tcp.h>

#include "zinterface.h"
#include "zpool.h"
#include "core.h"

namespace core {

    extern core::Core *g_core;
    extern s32 g_epoller_fd;

    enum eCompletion {
        Accept,
        Connect,
        IO,
    };

    class iCompleter;
    struct zAssociate {
        const eCompletion cpn;
        iCompleter * const cpr;
        zAssociate(const eCompletion __cpn, iCompleter * const __cpr) : cpn(__cpn), cpr(__cpr) {}
    };

    class iCompleter {
    public:
        virtual ~iCompleter() {}

        virtual void on_completer(zAssociate *__ac, const eCompletion __type, const struct epoll_event &__ev) = 0;
    };

    struct zPackage {
        void * const data;
        const s32 len;
        const std::string ip;
        const s32 port;

        zPackage(const void *__data, const s32 __len, const std::string &__ip, const s32 __port) : data(new char[__len]), len(__len), ip(__ip), port(__port) {
            tool::mem::s_memcpy(data, len, __data, __len);
        }
        zPackage(const zPackage &__package) : data(__package.data), len(__package.len), ip(__package.ip), port(__package.port) {}
    };



    class iNet {
    public:
        virtual ~iNet() {}
        virtual bool launch() = 0;
        virtual bool launch_udp_session(zif::iUDPSession *__session, const char *__ip, const s32 __port) = 0;
        virtual bool launch_tcp_session(zif::iTCPSession *__session, const char *__ip, const s32 __port, int __max_ss, int __max_rs) = 0;
        virtual bool launch_tcp_server(zif::iTCPServer *__server, const char *__ip, const s32 __port, int __max_ss, int __max_rs) = 0;
        virtual void deal() = 0;
    };

    iNet * net_instance();

    class Epoller : public iNet {
    public:
        virtual ~Epoller() {}
        static Epoller * instance();
        virtual bool launch();
        virtual bool launch_udp_session(zif::iUDPSession *__session, const char *__ip, const s32 __port);
        virtual bool launch_tcp_session(zif::iTCPSession *__session, const char *__ip, const s32 __port, int __max_ss, int __max_rs);
        virtual bool launch_tcp_server(zif::iTCPServer *__server, const char *__ip, const s32 __port, int __max_ss, int __max_rs);
        virtual void deal();
    };
}

#ifdef __cplusplus
extern "C" {
#endif
    inline s32 setnonblocking(int sockfd) {
        return fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK);
    }

    inline s32 setreuse(const s32 fd) {
        int val = 1l;
        return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&val, sizeof(val));
    }

    inline s32 settcpnodelay(const s32 fd) {
        int val = 1l;
        return setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char *)&val, sizeof(val));
    }

    inline s32 settcpquickack(const s32 fd) {
        int val = 1l;
        return setsockopt(fd, IPPROTO_TCP, TCP_QUICKACK, (const char *)&val, sizeof(val));
    }

    inline s32 setsocksendbuff(int sockfd, int size) {
        return setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (const char *)&size, sizeof(size));
    }

    inline s32 setsockrecvbuff(int sockfd, int size) {
        return setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (const char *)&size, sizeof(size));
    }

    inline s32 setmaxopenfiles(const s32 size) {
        struct rlimit rt;
        rt.rlim_max = rt.rlim_cur = size;
        if (-1 == setrlimit(RLIMIT_NOFILE, &rt)) return -1;
        return 0;
    }

    inline s32 setstacksize(const s32 size) {
        struct rlimit rt;
        rt.rlim_max = rt.rlim_cur = size * 1024;
        if (-1 == setrlimit(RLIMIT_STACK, &rt)) return  -1;
        return 0;
    }

    inline bool get_ip_by_host(const char *__host, std::string &__ip) {
        struct hostent *hp;
        if ((hp = gethostbyname(__host)) == NULL) return false;
        __ip = inet_ntoa(*(struct in_addr*)hp->h_addr);
        return true;
    }

#ifdef __cplusplus
}
#endif

#endif /* !__SERVER_COMMOM_H__ */
