
#include "./server.hpp"

#include <atomic>


namespace server {

    std::atomic<int> used_threads(0);
    int max_thread = MAX_THREADS;
    
    int run(const unsigned int& port, types::s32 *argc, char ***argv) {
        return 0;
    }

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
    int ssl_congiure_context_load_password(char *buffer, types::s32 size, types::s32 rwflag, void *u) {
        FILE *file = fopen("../keys/ssl/pp.pem", "r");
        if (file == nullptr) {
            // TODO: handle the error
        }
        fgets(&buffer[0], size, file);
        return strlen(&buffer[0]);
    }

    /**
     * Configures an OpenSSL context for OpenSSL Sockets
     * @param ctx
     * @return
     */
    int ssl_configure_context(SSL_CTX *ctx) {
        SSL_CTX_set_ecdh_auto(ctx, 1);
        SSL_CTX_set_default_passwd_cb(ctx, &ssl_congiure_context_load_password);
        // Sets the certificate
        if (SSL_CTX_use_certificate_file(ctx, "../keys/ssl/cert.pem", SSL_FILETYPE_PEM) <= 0) {
            ERR_print_errors_fp(stderr);
            return -1;
        }
        // set the private key
        if (SSL_CTX_use_PrivateKey_file(ctx, "../keys/ssl/key.pem", SSL_FILETYPE_PEM) <= 0) {
            ERR_print_errors_fp(stderr);
            return -1;
        }
        return 0;
    }



}