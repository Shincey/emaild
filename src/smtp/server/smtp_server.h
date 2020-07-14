#ifndef __SMTP_SERVER_H__
#define __SMTP_SERVER_H__

#include <vector>
#include "zinterface.h"
#include "command.h"

namespace smtp {
    class SMTPSession : public zif::iTCPSession {
    public:
        int on_recv(zif::iCore *__core, const void *__content, const int __size);
        void on_connect(zif::iCore *__core);
        void on_disconnect(zif::iCore *__core);
        void on_connect_failed(zif::iCore *__core);
    
    private:
        CommandType status_;
        std::vector<Command> cmds_;
    };

    class SMTPServer : public zif::iTCPServer {
        zif::iTCPSession * on_malloc_connection(zif::iCore *__core, const char *__remote_ip, const s32 __remote_port);
        void on_error(zif::iCore *__core, zif::iTCPSession *__session);
        void on_release(zif::iCore *__core);
        void close();
    };
}

#endif // __SMTP_SERVER_H__