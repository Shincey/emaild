#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <tuple>
#include <string>
#include <fstream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>
#include <fcntl.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "../modules/email.hpp"
#include "../types/types.hpp"

#define MAX_THREADS 6


namespace server {

    enum class ConnPhasePT {
        PHASE_PT_INITIAL,
        PHASE_PT_HELLO,
        PHASE_PT_MAIL_FROM,
        PHASE_PT_MAIL_TO,
        PHASE_PT_DATA,
        PHASE_PT_DATA_END,
    };

    /**
     * Runs an server instance
     * @param port
     * @return
     */
    int run(const unsigned int& port, types::s32 *argc, char ***argv);

    /**
     * the method which handles one client
     * @param sock_addrin
     * @param sock_fd
     */
    void connection_thread(struct sockaddr_in *sock_addrin, types::s32 sock_fd);

    /**
     * Loads the passphrase from file
     * @param buffer
     * @param size
     * @param rwflag
     * @param u
     * @return
     */
    int ssl_congiure_context_load_password(char *buffer, types::s32 size, types::s32 rwflag, void *u);

    /**
     * Configures an OpenSSL context for OpenSSL Sockets
     * @param ctx
     * @return
     */
    int ssl_configure_context(SSL_CTX *ctx);

}

#endif // __SERVER_HPP__