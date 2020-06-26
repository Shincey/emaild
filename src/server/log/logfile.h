#ifndef __LOGFILE_H__
#define __LOGFILE_H__

#include "zsystem.h"

namespace zcore {
    class LogFile {
    public:

        LogFile() : open_tick_(0), file_(nullptr) {}

        bool open(const char *_path, const char *__name);
        bool is_open();
        void close();

        void write(const char *__data);
        void flush();

        inline s64 get_open_tick() {
            zassert(open_tick_ != 0, "open tick something wrong");
            return open_tick_;
        }

    private:
        s64 open_tick_;
        FILE *file_;
    };
}

#endif // __LOGFILE_H__