#include "smtp_server.h"

namespace smtp {
    int SMTPSession::on_recv(zif::iCore *__core, const void *__content, const int __size) {
        char tmp[1024];
        snprintf(tmp, __size, "%s", (const char *)__content);
        printf("on_recv: %s\n", tmp);
        return __size;
    }

    void SMTPSession::on_connect(zif::iCore *__core) {
        printf("on_connect\n");
    }

    void SMTPSession::on_disconnect(zif::iCore *__core) {
        printf("on_disconnect\n");
    }

    void SMTPSession::on_connect_failed(zif::iCore *__core) {
        printf("on_connect_failed\n");
    }


    zif::iTCPSession * SMTPServer::on_malloc_connection(zif::iCore *__core, const char *__remote_ip, const s32 __remote_port) {
        return new SMTPSession;
    }

    void SMTPServer::on_error(zif::iCore *__core, zif::iTCPSession *__session) {
        printf("on_error\n");
    }

    void SMTPServer::on_release(zif::iCore *__core) {
        printf("on_release\n");
    }

    void SMTPServer::close() { accepter_->release(); }
}