#ifndef __CORE_H__
#define __CORE_H__

#include "zinterface.h"

namespace core {
    
    class Core : public zif::iCore {
    public:
        virtual ~Core() {}

        virtual iModule * find_module(const std::string &__name);
        virtual const char * get_env() { return nullptr; }

        void parse_args(int __argc, const char **__argv);
        virtual const char * get_args(const char *__name);

        virtual void set_core_name(const char *__name) { core_name_ = __name; }
        virtual const char * get_core_name() { return core_name_.c_str(); }

        static Core *instance();

        bool launch();

        virtual bool launch_udp_session(zif::iUDPSession *__session, const char *__ip, const s32 __port);
        virtual bool launch_tcp_session(zif::iTCPSession *__session, const char *__ip, const s32 __port, int __max_ss, int __max_rs);
        virtual bool launch_tcp_server(zif::iTCPServer *__server, const char *__ip, const s32 __port, int __max_ss, int __max_rs);

        virtual zif::iHTTPRequest * get_http_request(const s64 __account, const s64 __id, const char * __url, zif::iHTTPResponse * __response, const zif::iContext & __context);

        virtual void start_timer(zif::iTimer *__timer, const s32 __id, s64 __delay, s32 __count, s64 __interval, const zif::iContext __context, const char * __file, const s32 __line);
        virtual void kill_timer(zif::iTimer *__timer, const s32 __id, const zif::iContext __context=(s64)0);
        virtual void pause_timer(zif::iTimer *__timer, const s32 __id, const zif::iContext __context=(s64)0);
        virtual void resume_timer(zif::iTimer *__timer, const s32 __id, const zif::iContext __context=(s64)0);
        virtual void trace_timer();

        virtual void log_sync(const s64 __tick, const char *__log, const bool __echo);
        virtual void log_async(const s64 __tick, const char *__log, const bool __echo);
        virtual void set_sync_file_prefix(const char *__prefix);
        virtual void set_async_file_prefix(const char *__prefix);

        void loop();

    private:
        std::string core_name_;
        std::string env_;
    };
}

#endif // __CORE_H__