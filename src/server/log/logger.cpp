
#include "logger.h"
#include "ztools.h"

#include <unistd.h>

namespace core {
#define TIME_OUT_FOR_CUT_FILE 5*60*60*1000
#define READ_COUNT 64

    Logger *Logger::instance() {
        static Logger *s_logger = nullptr;
        if (nullptr == s_logger) {
            s_logger = new Logger();
            s_logger->launch();
        }
        return s_logger;
    }

    bool Logger::launch() {
        s32 pid = getpid(); //tool::get_process_id();
        set_sync_file_prefix(tool::int64_to_str(pid).c_str());
        set_async_file_prefix(tool::int64_to_str(pid).c_str());
        start();
        thread_status_ = STATUS_STARTING;
        return true;
    }

    void Logger::shutdown() {
        terminate();
        delete this;
    }

    void Logger::log_sync(const s64 __tick, const char *__log, const bool __echo) {
        std::string timestr = tool::time::time(__tick);
        sync_file_.write(timestr.c_str());
        sync_file_.write("|");
        sync_file_.write(__log);
        sync_file_.write("\n");
        sync_file_.flush();
        if (__echo) {
            printf(timestr.c_str());
            printf(" | %s\n", __log);
        }
    }

    void Logger::log_async(const s64 __tick, const char *__log, const bool __echo) {
        while (queue_[write_index_].has_data) {
            usleep(1000);
        }
        queue_[write_index_].tick = __tick;
        queue_[write_index_].log = __log;
        queue_[write_index_].echo = __echo;
        queue_[write_index_].has_data = true;
        write_count_++;
        if (++write_index_ >= LOG_QUEUE_MAX_SIZE) { write_index_ = 0; }
    }

    void Logger::set_sync_file_prefix(const char *__prefix) {
        sync_prefix_ = __prefix;
        if (sync_file_.is_open()) { sync_file_.close(); }

        std::string path = ""; //................................................................
        zassert(!sync_file_.is_open(), "sync log file has been craeted");
        if (!sync_file_.is_open()) {
            std::string name;
            name.append(sync_prefix_).append("_").append(tool::time::now("sync_%4d_%02d_%02d_%02d_%02d_%02d.log"));
            if (!sync_file_.open(path.c_str(), name.c_str())) {
                zassert(false, "something wrong here");
                printf("open log file : %s error \n", name.c_str());
            }
        }

    }

    void Logger::set_async_file_prefix(const char *__prefix) {
        async_prefix_ = __prefix;
        if (async_file_.is_open()) { async_file_.close(); }

        std::string path = "";
        path.append("/log/");

        // tool::file::mkdir(path.c_str());
        zassert(!async_file_.is_open(), "async log file has been created");

        std::string name;
        name.append(async_prefix_.c_str()).append("_").append(tool::time::now("async_%4d_%02d_%02d_%02d_%02d_%02d.log"));

        if (!async_file_.open(path.c_str(), name.c_str())) {
            zassert(false, "something wrong here");
            printf("open log file : %s error \n", name.c_str());
        }
    }

    bool Logger::start(s32 __threads) {
        ThreadCallBack callback = [](void *__param) -> void* {
            zif::iThread *ptr = static_cast<zif::iThread *>(__param);
            zassert(ptr, "the pointer to zthread is null");
            ptr->run();
            return ptr;
        };
        for (int i = 0; i < __threads; ++i) {
            pthread_t tid = 0;
            int ret = pthread_create(&tid, nullptr, callback, (void *)this);
            if (ret != 0) {
                return false;
            } else {

            }
        }
        return true;
    }


    void Logger::terminate() {
        zassert(thread_status_ == STATUS_STARTING, "wrong thread status");
        if (thread_status_ == STATUS_STARTING) {
            thread_status_ = STATUS_STOPPING;
            while (thread_status_ != STATUS_STOPPED) { usleep(1000); }
        }
    }

    void Logger::run() {
        while (true) {
            while (!queue_[read_index_].has_data) {
                if (read_count_ == write_index_) {
                    if (thread_status_ == STATUS_STOPPING) {
                        thread_status_ = STATUS_STOPPED;
                        return;
                    }
                }
                usleep(1000);
            }

            while(async_prefix_ == "") { usleep(1000); }

            if (!async_file_.is_open()) {
                std::string name;
                name.append(async_prefix_).append(tool::time::now("async_%4d_%02d_%02d_%02d_%02d_%02d.log"));

                std::string path = ""; //.......................................
                path.append("/log/");

                if (!async_file_.open(path.c_str(), name.c_str())) {
                    zassert(false, "something wrong here");
                    printf("open log file : %s error\n", name.c_str());
                    return;
                }
            }

            s64 tick = tool::time::milliseconds();
            if (tick - async_file_.get_open_tick() >= TIME_OUT_FOR_CUT_FILE) {
                async_file_.close();
                std::string name;
                name.append(async_prefix_).append("_").append(tool::time::now("async_%4d_%02d_%02d_%02d_%02d_%02d.log"));
                std::string path = "";
                path.append("/log/");

                if (!async_file_.open(path.c_str(), name.c_str())) {
                    zassert(false, "something wrong here");
                    printf("open log file : %s error\n", name.c_str());
                    return;
                }
            }

            s32 count = 0;
            while (queue_[read_index_].has_data && count++ <= read_count_) {
                async_file_.write(tool::time::time(queue_[read_index_].tick).c_str());
                async_file_.write("|");
                async_file_.write(queue_[read_index_].log.c_str());
                async_file_.write("\n");

                if (queue_[read_index_].echo) {
                    printf("%s|%s\n", tool::time::time(queue_[read_index_].tick).c_str(), queue_[read_index_].log.c_str());
                }

                queue_[read_index_].clear();
                read_index_++;
                read_count_++;

                if (read_index_ >= LOG_QUEUE_MAX_SIZE) { read_index_ = 0; }
            }

            async_file_.flush();
        }
    }

}