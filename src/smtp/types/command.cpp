#include "command.h"

#include <algorithm>

#include "ztools.h"

namespace smtp {

    Command::Command(const CommandType &__type, const std::vector<std::string> &__args) : type_(__type), args_(__args) {}


    Command::Command() : type_(CommandType::CT_UNKNOWN), args_() {}


    Command::Command(const std::string &__raw) {
        std::size_t idx = __raw.find_first_of(':');
        std::string cmd;
        bool contain_args = false;
        if (idx != std::string::npos) {
            contain_args = true;
            cmd = __raw.substr(0, idx);
        } else {
            idx = __raw.find_first_of(' ');
            if (idx == std::string::npos) {
                cmd = __raw;
            } else {
                contain_args = true;
                cmd = __raw.substr(0, idx);
            }
        }

        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
        if (s_cmd_types.count(cmd.c_str())) { type_ = s_cmd_types[cmd.c_str()]; }
        else type_ = CommandType::CT_UNKNOWN;

        if (!contain_args) return;
        std::string args_raw = __raw.substr(++idx);
        std::string arg = tool::strip(args_raw);
        if (!arg.empty()) { args_.push_back(arg); }
    }


    void Command::generate(std::string &__cmd) {
        switch (type_) {
            case CommandType::CT_HELO: {
                __cmd += "HELO";
                break;
            }
            case CommandType::CT_MAIL_FROM: {
                __cmd += "MAIL FROM";
                break;
            }
            case CommandType::CT_RCPT_TO: {
                __cmd += "RCPT TO";
                break;
            }
            case CommandType::CT_DATA: {
                __cmd += "DATA";
                break;
            }
            case CommandType::CT_START_TLS: {
                __cmd += "STARTTLS";
                break;
            }
            case CommandType::CT_QUIT: {
                __cmd += "QUIT";
                break;
            }
            default: {
                break;
            }
        }
        for (const std::string &str : args_) {
            __cmd += ' ';
            __cmd += str;
        }
        __cmd += "\r\n";
    }
}