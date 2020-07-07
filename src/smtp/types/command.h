#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string>
#include <vector>
#include <map>

namespace smtp {
    
    enum class CommandType {
        CT_HELO = 0,
        CT_MAIL_FROM,
        CT_RCPT_TO,
        CT_DATA,
        CT_RSET,
        CT_NOOP,
        CT_QUIT,
        CT_START_TLS,
        CT_UNKNOWN,
    };

    static std::map<const char *, CommandType> s_cmd_types = {
        {"helo", CommandType::CT_HELO},
        {"ehlo", CommandType::CT_HELO},
        {"starttls", CommandType::CT_START_TLS},
        {"mail from", CommandType::CT_MAIL_FROM},
        {"rcpt to", CommandType::CT_RCPT_TO},
        {"data", CommandType::CT_DATA},
        {"quit", CommandType::CT_QUIT},
        {"unknown", CommandType::CT_UNKNOWN}
    };
    

    class Command {
    public:
        Command(const CommandType &__type, const std::vector<std::string> &__args);
        Command();
        Command(const std::string &__raw);

        void generate(std::string &__cmd);
    private:
        CommandType type_;
        std::vector<std::string> args_;
    };
}

#endif // __COMMAND_H__