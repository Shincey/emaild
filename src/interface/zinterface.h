#ifndef __ZINTERFACE_H__
#define __ZINTERFACE_H__


#include "zsystem.h"
#include <string>
#include "ztools.h"

class iModule;

namespace zif {
    struct zAddress {
        std::string ip;
        s32 port;
        zAddress() : ip(""), port(0) {}
        zAddress(const std::string &__ip, const s32 __port) : ip(__ip), port(__port) {}
    };

    class iCore;

    class iPipe {
    public:
        virtual ~iPipe() {}
        virtual void cache() = 0;
        virtual void load() = 0;
        virtual void close() = 0;
    };

    class iUDPPipe : public iPipe {
    public:
        virtual ~iUDPPipe() {}
        virtual void sendto(const char *__ip, const s32 __port, const void *__content, const s32 __size) = 0;
    };

    class iTCPPipe : public iPipe {
    public:
        virtual ~iTCPPipe() {}
        virtual void send(const void *__data, const s32 __size, bool __immediate) = 0;
    };

    class iSocket {
    public:
        virtual ~iSocket() {}
        zAddress address_;
    };

    class iUDPSocket : public iSocket {
    public:
        iUDPSocket() : pipe_(nullptr) {}
        virtual ~iUDPSocket() {}
        iUDPPipe * const pipe_;
    };

    class iTCPSocket : public iSocket {
    public:
        virtual ~iTCPSocket() {}
        iTCPSocket() : pipe_(nullptr) {}
        iTCPPipe * const pipe_;
    };

    class iUDPSession : public iUDPSocket {
    public:
        virtual ~iUDPSession() {}
        virtual void on_create(bool success) = 0;
        virtual void on_recv(iCore *__core, const char *__ip, const s32 __port, const char *__content, const int __size) = 0;
        virtual void on_close(iCore *__core) = 0;

        void close() const { 
            if (this && pipe_) { pipe_->close(); }
        }

        inline void sendto(const char *__ip, const s32 __port, const char *__content, const int __size) {
            if (this && pipe_) { pipe_->sendto(__ip, __port, __content, __size); }
        }

        inline void load() {
            if (this && pipe_) { pipe_->load(); }
        }
    };

    class iAccepter {
    public:
        virtual ~iAccepter() {}
        virtual void release() = 0;
    };

    class iTCPSession : public iTCPSocket {
    public:
        virtual ~iTCPSession() {}
        iTCPSession() : initiative_(false) {}
        virtual int on_recv(iCore *__core, const void *__content, const int __size) = 0;
        virtual void on_connect(iCore *__core) = 0;
        virtual void on_disconnect(iCore *__core) = 0;
        virtual void on_connect_failed(iCore *__core) = 0;

        void close() const {
            if (this && pipe_) { pipe_->close(); }
        }

        inline void send(const void *__content, const int __size, bool __immediate=true) const {
            if (this && pipe_) { pipe_->send(__content, __size, __immediate); }
        }

        inline void cache() {
            if (this && pipe_) { pipe_->cache(); }
        }

        inline void load() {
            if (this && pipe_) { pipe_->load(); }
        }

        const bool initiative_;
    };

    class iUDPServer {
    public:
        iUDPServer() : ac_(nullptr) {}
        virtual ~iUDPServer() {}

        virtual iUDPSession * on_malloc_connection(iCore *__core, const char *__ip, const s32 __port) = 0;
        virtual void on_release(iCore *__core) = 0;
        virtual void close() { ac_->release(); }

        iAccepter *ac_;
    };

    class iTCPServer {
    public:
        iTCPServer() : ac_(nullptr) {}
        virtual ~iTCPServer() {}

        virtual iTCPSession * on_malloc_connection(iCore *__core, const char *__remote_ip, const s32 __remote_port) = 0;
        virtual void on_error(iCore *__core, iTCPSession *__session) = 0;
        virtual void on_relear(iCore *__core) = 0;
        virtual void close() { ac_->release(); }
        iAccepter *ac_;
    };

    class iUDPBoradcaster {
    public:
        virtual void broadcast(const void *__content, const s32 __size) = 0;
    };

    struct iContext {
        union {
            const void *context_point_;
            const s64 context_mark_;
        };
        iContext(const void *__point) : context_point_(__point) {}
        iContext(const s64 __mark) : context_mark_(__mark) {}
    };

    class iTimer {
    public:
        virtual void on_start(iCore *__core, const s32 __id, const iContext *__context, const s64 __tick) = 0;
        virtual void on_timer(iCore *__core, const s32 __id, const iContext *__context, const s64 __tick) = 0;
        virtual void on_end(iCore *__core, const s32 __id, const iContext *__context, bool __nonviolent, const s64 __tick) = 0;
        virtual void on_pause(iCore * __core, const s32 __id, const iContext & __context, const s64 __tick) = 0;
        virtual void on_resume(iCore * __core, const s32 __id, const iContext & __context, const s64 __tick) = 0;
    };

    class iHTTPRequest {
    public:
        virtual ~iHTTPRequest() {}
        virtual void post_param(const char *__key, const char *__value) = 0;
        virtual void do_request() = 0;
    };

    class iHTTPResponse {
    public:
        iHTTPResponse() : reference_(0) {}
        virtual ~iHTTPResponse() {}
        virtual void on_error(const s32 __id, const s32 __error, const iContext &__context) = 0;
        virtual void on_response(const s32 __id, const void *__data, const s32 __size, const iContext &__context) = 0;
        virtual bool is_requesting() { return reference_; }
        s32 reference_;
    };

    class iCore {
    public:
        virtual ~iCore() {}
        virtual iModule * find_module(const std::string &__name) = 0;
        virtual const char * get_env() = 0;
        virtual const char * get_args(const char *__name) = 0;

        virtual void set_core_name(const char *__name) = 0;
        virtual const char * get_core_name() = 0;


        virtual bool launch_udp_session(iUDPSession *__session, const char *__ip, const s32 __port) = 0;
        virtual bool launch_tcp_session(iTCPSession *__session, const char *__ip, const s32 __port, int __max_ss, int __max_rs) = 0;
        virtual bool launch_tcp_server(iTCPServer *__server, const char *__ip, const s32 __port, int __max_ss, int __max_rs) = 0;

        virtual iHTTPRequest * get_http_request(const s64 __account, const s64 __id, const char * __url, iHTTPResponse * __response, const iContext & __context) = 0;

        virtual void start_timer(iTimer *__timer, const s32 __id, s64 __delay, s32 __count, s64 __interval, const iContext __context, const char * __file, const s32 __line) = 0;
        virtual void kill_timer(iTimer *__timer, const s32 __id, const iContext __context=(s64)0) = 0;
        virtual void pause_timer(iTimer *__timer, const s32 __id, const iContext __context=(s64)0) = 0;
        virtual void resume_timer(iTimer *__timer, const s32 __id, const iContext __context=(s64)0) = 0;
        virtual void trace_timer() = 0;

        virtual void log_sync(const s64 __tick, const char *__log, const bool __echo) = 0;
        virtual void log_async(const s64 __tick, const char *__log, const bool __echo) = 0;
        virtual void set_sync_file_prefix(const char *__prefix) = 0;
        virtual void set_async_file_prefix(const char *__prefix) = 0;
    };

    class iThread {
    public:
        typedef void * (* ThreadCallBack)(void *);

        virtual ~iThread() {}

        virtual bool start(s32 __threads=1) = 0;
        virtual void run() = 0;
        virtual void terminate() = 0;
    };

}

#ifdef DEBUG_MODE
#define debug(core, format, ...) { \
            printf("debug: %s : %d "#format, __FILE__, __LINE__, ##__VA_ARGS__); \
            printf("\n"); \
        }

#define trace(core, format, ...) { \
            char log[4096] = { 0 }; \
            snprintf(log, sizeof(log), "trace: %s : %d "#format, __FILE__, __LINE__, ##__VA_ARGS__); \
            printf("%s\n", log); \
            core->log_async(tool::time::milliseconds(), log, false); \
        }

#define error(core, format, ...) { \
        char log[4096] = { 0 }; \
        snprintf(log, sizeof(log), "error: %s : %d "#format, __FILE__, __LINE__, ##__VA_ARGS__); \
        printf("%s\n", log); \
        core->log_sync(tool::time::milliseconds(), log, true); \
        }

#define imp(core, format, ...) { \
        char log[4096] = { 0 }; \
        snprintf(log, sizeof(log), "%s : %d "#format, __FILE__, __LINE__, ##__VA_ARGS__); \
        printf("%s\n", log); \
        core->log_sync(tool::time::milliseconds(), log, true); \
        }

#else
#define debug(core, format, ...) (void)0;

#define trace(core, format, ...) { \
            char log[4096] = { 0 }; \
            snprintf(log, sizeof(log), "trace: %s : %d "#format, __FILE__, __LINE__, ##__VA_AR); \
            core->log_async(tool::time::milliseconds(), log, false); \
        }

#define error(core, format, ...) { \
            char log[4096] = { 0 }; \
            zassert(false, format, ##__VA_ARGS__); \
            snprintf(log, sizeof(log), "error: %s : %d"#format, __FILE__, __LINE__, ##__VA_ARGS__); \
            core->log_sync(tool::time::milliseconds(), log, false); \
        }

#define imp(core, format, ...) { \
            char log[4096] = { 0 }; \
            snprintf(log, sizeof(log), "%s : %d "#format, __FILE__, __LINE__, ##__VA_ARGS__); \
            core->log_sync(tool::time::milliseconds(), log, false)
        }

#endif // DEBUG_MODE

class iModule {
public:
    virtual ~iModule() {}
    virtual bool init(zif::iCore *__core) = 0;
    virtual bool launch(zif::iCore *__core) = 0;
    virtual bool destroy(zif::iCore *__core) = 0;

public:
    iModule() : next_(nullptr) {}
    inline void set_next(iModule *&__iModule) { next_ = __iModule; }
    inline iModule *get_next() { return next_; }
    inline void set_name(const char *__name) { name_ = __name; }
    inline const char *get_name() const { return name_.c_str(); }
private:
    iModule *next_;
    std::string name_;
};

typedef iModule *(*iModulePtr)(void);

#define CREATE_iModule(name) \
class factory##name { \
public: \
    factory##name(iModule *&__iModule) { \
        iModule *iModule##name = new name; \
        iModule##name->set_name(name); \
        iModule##name->set_next(__iModule); \
        __iModule = iModule##name; \
    } \
}; \
factory##name factory##name(s_iModules);

#define GET_DLL_INSTANCE \
    static iModule *s_iModules = nullptr; \
    extern "C" iModule *GetiModule() { \
        return s_iModules; \
    }


#endif // __ZINTERFACE_H__