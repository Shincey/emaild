#include "common.h"
#include "udper.h"
#include "tcper.h"

#define EPOLLER_DESC_COUNT 10240
#define EPOLLER_EVS_MAX_COUNT 512

namespace core {
    s32 g_epoller_fd = -1;
    Core *g_core = nullptr;

    iNet * net_instance() {
        return Epoller::instance();
    }

    Epoller * Epoller::instance() {
        static Epoller *s_epoller = nullptr;
        if (nullptr == s_epoller) {
            s_epoller = new Epoller;
            if (!s_epoller->launch()) {
                delete s_epoller;
                s_epoller = nullptr;
            }
        }
        return s_epoller;
    }

    bool Epoller::launch() {
        g_core = Core::instance();
        zassert(g_core, "something wrong here");
        if (g_epoller_fd = ::epoll_create(EPOLLER_DESC_COUNT); g_epoller_fd == -1) {
            error(g_core, "create epoller fd failed %s", strerror(errno));
            return false;
        }
        return true;
    }

    bool Epoller::launch_udp_session(zif::iUDPSession *__session, const char *__ip, const s32 __port) {
        UDPer *udp = UDPer::create(__session, __ip, __port);
        return udp != nullptr;
    }

    bool Epoller::launch_tcp_session(zif::iTCPSession *__session, const char *__ip, const s32 __port, int __max_ss, int __max_rs) {
        TCPer *tcp = TCPer::create(__session, __ip, __port, __max_ss, __max_rs);
        return tcp != nullptr;
    }

    bool Epoller::launch_tcp_server(zif::iTCPServer *__server, const char *__ip, const s32 __port, int __max_ss, int __max_rs) {
        Accepter *accepter = Accepter::create(__server, __ip, __port, __max_ss, __max_rs);
        return accepter != nullptr;
    }

    void Epoller::deal() {
        static struct epoll_event evs[EPOLLER_EVS_MAX_COUNT];
        s64 tick = tool::time::microseconds();
        do {
            errno = 0;
            s32 count = 0;
            if (count = epoll_wait(g_epoller_fd, evs, EPOLLER_EVS_MAX_COUNT, 1); 0 == count) { return; }
            if (-1 == count) { return; }
            for (s32 i = 0; i < count; ++i) {
                struct sAssociation *association = (struct sAssociation *)evs[i].data.ptr;
                association->event_handler->on_epoll_event(association, association->type, evs[i]);
            }
        } while (tool::time::microseconds() - tick < 1000);
    }

}