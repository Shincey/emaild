#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <tuple>

namespace modules::command {
    enum class SMTPServerCommand {
        INVALID,
        HELLO,
        MAIL_FROM,
        RCPT_TO,
        DATA,
        RSET,
        VRFY,
        EXPN,
        NOOP,
        QUIT,
        HELP,
        START_TLS,
        AUTH
    };

    /**
     * Parses an mailer to server command
     * @param buf
     * @return
     */
    std::tuple<SMTPServerCommand, const char*> parse(char* buf);

    /**
     * Generates an response with code
     * @param code
     * @param param
     * @param listParams
     * @param listParamsN
     * @return
     */
    char* gen(int code, const char* param, const char* listParams[], char listParamsN);

    /**
     * Converts command enum to string
     * @param command
     * @return
     */
    const char* serverCommandToString(const SMTPServerCommand& command);
}

#endif //__COMMANDS_H__