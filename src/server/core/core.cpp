#include "core.h"

#include <unordered_map>

namespace zcore {
    static Core *s_core = nullptr;
    static std::unordered_map<std::string, std::string> s_args_map;

    Core * Core::instance() {
        if (nullptr == s_core) { s_core = new Core(); }
        return s_core;
    }

    const char * Core::get_args(const char *__name) {
        if (s_args_map.count(__name)) { return s_args_map[__name].c_str(); }
        else { return nullptr; }
    }

    void Core::parse_args(int __argc, const char ** __argv) {
        for (int i = 0; i < __argc; ++i) {
            if (strncmp(__argv[i], "--", 2) == 0) {
                const char *start = __argv[i] + 2;
                const char *equal = strstr(start, "=");
                if (nullptr != equal) {
                    std::string name(start, equal);
                    std::string value(equal + 1);
                    s_args_map[name] = value;
                } else if (strlen(__argv[i]) > 2) {
                    s_args_map[__argv[i] + 2] = "";
                }
            } else {
                // error handle
            }
        }
    }

    iModule * Core::find_module(const std::string &__name) {
        return nullptr;
    }

}