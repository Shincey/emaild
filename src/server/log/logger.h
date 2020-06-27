#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>

#include "logfile.h"
#include "zthread.h"

#define LOG_QUEUE_MAX_SIZE 10240

namespace core {
    class Logger : public lib::zthread {
        struct LogUnit {
            LogUnit() : tick(0), log(""), echo(false), has_data(false) {}

            void clear() {
                tick = 0;
                log.clear();
                echo = false;
                has_data = false;
            }

            s64 tick;
            std::string log;
            bool echo;
            bool has_data;
        };

    public:
        static Logger * instance();

        virtual bool launch();
        virtual void process(const s32 overtiem) {}
        virtual void shutdown();

        virtual void log_sync(const s64 __tick, const char *__log, const bool echo);
        virtual void log_async(const s64 __tick, const char *__log, const bool echo);
        virtual void set_sync_file_prefix(const char *__prefix);
        virtual void set_async_file_prefix(const char *__prefix);

        virtual void terminate();
        virtual void run();

    private:
        LogUnit queue_[LOG_QUEUE_MAX_SIZE];
        s32 read_index_;
        s32 read_count_;
        s32 write_index_;
        s32 write_count_;

        std::string sync_prefix_;
        LogFile sync_file_;

        std::string async_prefix_;
        LogFile async_file_;

        enum { 
            STATUS_UNINITIALIZED = 0,
            STATUS_STARTING = 1,
            STATUS_STOPPING = 2,
            STATUS_STOPPED = 3,
        };
        s8 thread_status_;

    };
}

#endif // __LOGGER_H__