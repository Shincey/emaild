#include "logfile.h"
#include "ztools.h"

namespace core {
    bool LogFile::open(const char *__path, const char *__name) {
        if (file_) { return false; }
        std::string filepath;
        filepath += __path;
        filepath += __name;

        file_ = ::fopen(filepath.c_str(), "ab+");
        if (nullptr == file_) { return false; }
        open_tick_ = tool::time::milliseconds();
        return true;
    }

    bool LogFile::is_open() { return file_ != nullptr; }

    void LogFile::close() {
        if (!file_) { return; }
        ::fflush(file_);
        ::fclose(file_);
        file_ = nullptr;
        open_tick_ = 0;
    }

    void LogFile::write(const char *__data) {
        zassert(file_ && __data, "something wrong here");
        if (file_) {
            s32 len = ::strlen(__data);
            ::fwrite(__data, len, 1, file_);
        }
    }

    void LogFile::flush() {
        zassert(file_, "something wrong here");
        if (file_) { ::fflush(file_); }
    }
    
}