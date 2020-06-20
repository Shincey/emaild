#ifndef __RESPONSE_HPP__
#define __RESPONSE_HPP__

#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <openssl/ssl.h>

#include "./commands/commands.hpp"
#include "../types/types.hpp"
#include "../types/serror.hpp"

namespace server::response {

    /**
     * Writes an message to an encrypted socket or plain text socket
     * @param soc
     * @param ssl
     * @param msg
     * @param msg_len
     */
    inline void write(const types::s32 *soc, SSL *ssl, const char *msg, int msg_len) {
        if (ssl == nullptr) {
            if (send(*soc, msg, msg_len, 0) < 0) {
                PREP_ERROR("Could not send message", msg);
            }
        } else {
            // sends the message over the socket
            SSL_write(ssl, msg, msg_len);
            // TODO: check for errors
        }
    }

    /**
     * Sends that the mailer may continue
     * @param ssl
     */
    inline void pre_context_proceed(const types::s32 *soc, SSL *ssl) {
        char *message = server::cmd::gen(250, "OK: proceed", nullptr, 0);
        write(soc, ssl, message, strlen(message));
        free(message);
    }

    /**
     * Sends that an bad sequence occurred
     * @param ssl
     * @param req_msg
     */
    inline void pre_context_bad_sequence(const types::s32 *soc, SSL *ssl, const char *req_msg) {
        char *message = server::cmd::gen(503, req_msg, nullptr, 0);
        write(soc, ssl, message, strlen(message));
        free(message);
    }

    /**
     * Sends an syntax error
     * @param ssl
     */
    inline void systax_error(const types::s32 *soc, SSL *ssl) {
        char *message = server::cmd::gen(501, "Syntax Error", nullptr, 0);
        write(soc, ssl, message, strlen(message));
        free(message);
    }


}

#endif // __RESPONSE_HPP__