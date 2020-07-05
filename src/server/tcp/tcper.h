#ifndef __TCPER_H__
#define __TCPER_H__

#include "zinterface.h"
#include "common.h"
#include "zbuffer.h"

namespace core {
    class Accepter : public zif::iAccepter, public core::iEpollEvent {
    public:
        virtual ~Accepter() {}

        static Accepter * create(zif::iTCPServer * __server, const char *__ip, const s32 __port, s32 __ssize, s32 __rsize);

        virtual void on_epoll_event(sAssociation *__association, const eEpollEventType __type, const struct epoll_event &__ev);

        virtual void release();

    private:
        friend lib::zpool<Accepter>;
        Accepter(zif::iTCPServer * __server, const char *__ip, const s32 __port, s32 __ssize, s32 __rsize);

    private:
        zif::iTCPServer * const server_;
        s32 socket_;
        zif::sAddress address_;
        const s32 ssize_;
        const s32 rsize_;
        sAssociation association_;
        sockaddr_in addr_in_;
    };



    

    class TCPer : public iEpollEvent, public zif::iTCPPipe {
    public:
        virtual ~TCPer() {}

        static TCPer * create(zif::iTCPSession *__session, const s32 __sock, const std::string &__ip, const s32 __port, s32 __ssize, s32 __rsize);
        static TCPer * create(zif::iTCPSession *__session, const std::string &__host, const s32 __port, s32 __ssize, s32 __rsize);
        // overide from iEpollEvent
        virtual void on_epoll_event(sAssociation *__association, const eEpollEventType __type, const struct epoll_event &__ev);
        // overide from iTCPPipe
        virtual void close();
        virtual void cache();
        virtual void load();
        virtual void send(const void *__data, const s32 __size, bool __immediately);

    private:
        friend void case_connect(TCPer *__tcp, sAssociation *__association, const eEpollEventType __type, const struct epoll_event &__ev);
        friend void case_io(TCPer *__tcp, sAssociation *__association, const eEpollEventType __type, const struct epoll_event &__ev);

    private:
        TCPer(zif::iTCPSession *__session, const std::string &__ip, const s32 __port, const s32 __ssize, const s32 __rsize);

    private:
        friend Accepter;
        friend lib::zpool<TCPer>;

    private:
        zif::iTCPSession * const session_;
        const zif::sAddress peer_address_;
        bool connected_;
        bool caching_;
        const sAssociation association_;
        s32 socket_;

        lib::zbuffer send_buff_;
        lib::zbuffer recv_buff_;
    };
}

#endif // __TCPER_H__