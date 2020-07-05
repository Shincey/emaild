/*
** udper.h for emaild
**
** Made by shincey
**
** Started on  Sat Jun 27 下午4:18:42 2020 shincey
** Last update Sun Jun 27 下午5:56:52 2020 shincey
*/

#ifndef __UDPER_H__
# define __UDPER_H__

#include <queue>
#include "common.h"
#include "zinterface.h"
#include "zpool.h"

#define PACKAGE_MAX_SIZE 512

namespace core {
    class UDPer : public zif::iUDPPipe, public iEpollEvent {
    public:
        virtual ~UDPer() {}

        static UDPer * create(zif::iUDPSession *__session, const std::string &__ip, const s32 __port);
        virtual void cache();
        virtual void load();
        virtual void close();

        virtual void sendto(const char *__ip, const s32 __port, const void *__content, const s32 __size);
        virtual void on_epoll_event(sAssociation *__association, const eEpollEventType __type, const struct epoll_event &__ev);

        inline u32 get_socket() { return socket_; }


    private:
        friend lib::zpool<UDPer>;
        UDPer(zif::iUDPSession *__session, const std::string &__ip, const s32 __port);

        bool async_recv();
        bool async_send();

    private:
        zif::iUDPSession *session_;
        s32 socket_;
        bool is_cache_;
        sAssociation association_;
        zif::sAddress addr_;
        char recv_temp_[PACKAGE_MAX_SIZE];
        std::queue<sPackage> send_queue_;
    };
}

#endif /* !__UDPER_H__ */


