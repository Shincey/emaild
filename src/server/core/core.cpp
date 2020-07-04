#include "core.h"

#include <unordered_map>

#include "common.h"
#include "logger.h"

namespace core {
    static Core *s_core = nullptr;
    static std::unordered_map<std::string, std::string> s_args_map;

    Core * Core::instance() {
        if (nullptr == s_core) { s_core = new Core; }
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


    bool Core::launch() {
        // 读取配置表，更改信息
        return true;
    }

    bool Core::launch_udp_session(zif::iUDPSession *__session, const char *__ip, const s32 __port) {
        return net_instance()->launch_udp_session(__session, __ip, __port);
    }

    bool Core::launch_tcp_session(zif::iTCPSession *__session, const char *__ip, const s32 __port, int __max_ss, int __max_rs) {
        return net_instance()->launch_tcp_session(__session, __ip, __port, __max_ss, __max_rs);
    }

    bool Core::launch_tcp_server(zif::iTCPServer *__server, const char *__ip, const s32 __port, int __max_ss, int __max_rs) {
        return net_instance()->launch_tcp_server(__server, __ip, __port, __max_ss, __max_rs);
    }

    zif::iHTTPRequest * Core::get_http_request(const s64 __account, const s64 __id, const char * __url, zif::iHTTPResponse * __response, const zif::iContext & __context) {
        return nullptr;
    }

    void Core::start_timer(zif::iTimer *__timer, const s32 __id, s64 __delay, s32 __count, s64 __interval, const zif::iContext __context, const char * __file, const s32 __line) {

    }
    void Core::kill_timer(zif::iTimer *__timer, const s32 __id, const zif::iContext __context) {

    }
    void Core::pause_timer(zif::iTimer *__timer, const s32 __id, const zif::iContext __context) {

    }
    void Core::resume_timer(zif::iTimer *__timer, const s32 __id, const zif::iContext __context) {

    }
    void Core::trace_timer() {

    }

    void Core::log_sync(const s64 __tick, const char *__log, const bool __echo) {
        Logger::instance()->log_sync(__tick, __log, __echo);
    }

    void Core::log_async(const s64 __tick, const char *__log, const bool __echo) {
        Logger::instance()->log_async(__tick, __log, __echo);
    }

    void Core::set_sync_file_prefix(const char *__prefix) {
        Logger::instance()->set_sync_file_prefix(__prefix);
    }

    void Core::set_async_file_prefix(const char *__prefix) {
        Logger::instance()->set_async_file_prefix(__prefix);
    }

    void Core::loop() {
        net_instance()->deal();
        //...
    }

}