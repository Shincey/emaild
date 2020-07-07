#ifndef __ZTOOLS_H__
#define __ZTOOLS_H__

#include "zsystem.h"

#include <math.h>
#include <chrono>
#include <string>
#include <vector>

namespace tool {

    namespace mem {
        inline void s_memset(void *__restrict __dest, size_t __max, int __val, size_t __n) {
            zassert(__n <= __max, "over flow");
            memset(__dest, __val, (__max >= __n) ? (__n) : (__max));
        }

        inline void s_memcpy(void *__restrict __dest, size_t __max, const void * __restrict __src, size_t __n) {
            zassert(__n <= __max, "over flow");
            memcpy(__dest, __src, (__max >= __n) ? (__n) : (__max));
        }

        inline bool s_streql(const char *__restrict __str1, const char *__restrict __str2) {
            return !strcmp(__str1, __str2);
        }

        inline int s_strcmp(const s8 *__restrict __dest, const s8 *__restrict __src) {
            return strcmp(__dest, __src);
        }
    }

    s32 get_process_id();

    namespace time {
        inline bool s_localtime(struct tm &__ts, const time_t &__tt) {
            const struct tm *pt = localtime(&__tt);
            if (nullptr == pt) { return false; }
            mem::s_memcpy(&__ts, sizeof(__ts), pt, sizeof(__ts));
            return true;
        }

        inline u64 milliseconds() {
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        }

        inline u64 microseconds() {
            return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        }

        inline std::string now(const char * __format="%4d-%02d-%02d %02d:%02d:%02d") {
            char strtime[64] = {0};
            auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            struct tm ts;
            s_localtime(ts, tt);
            snprintf(strtime, sizeof(strtime), __format, 
                     (int)ts.tm_year + 1900, 
                     (int)ts.tm_mon + 1, 
                     (int)ts.tm_mday, 
                     (int)ts.tm_hour, 
                     (int)ts.tm_min, 
                     (int)ts.tm_sec);
            return strtime;
        }

        /**
         * Convert timestamp to readable time representation
         * @param __timestamp : timestamp in milliseconds
         * @param format : time format to convert
         * @return : returns the formatted time.
         */
        inline std::string time(const s64 __timestamp , const char *__format = "%4d-%02d-%02d %02d:%02d:%02d") {
            char strtime[128];
            auto ms = std::chrono::milliseconds(__timestamp);
            auto tp = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>(ms);
            auto tt = std::chrono::system_clock::to_time_t(tp);
            struct tm ts;
            s_localtime(ts, tt);
            snprintf(strtime, sizeof(strtime), __format,
                     (int)ts.tm_year+1900,
                     (int)ts.tm_mon+1,
                     (int)ts.tm_mday,
                     (int)ts.tm_hour,
                     (int)ts.tm_min,
                     (int)ts.tm_sec);
            return strtime;
        }
    }

    namespace file {
        const char* get_app_path();
        bool exists(const char *__path);
        bool mkdir(const char *__path);
        bool rmdir(const char *__path);
        bool rmfile(const char *__path);
    }

    template <typename T>
    struct zHashFunction {
        size_t operator()(const T &__src) const { return (size_t)__src; }
    };

    inline s32 split(const std::string &__src, const std::string &__delimiter, std::vector<std::string> &__res) {
        if (__src.empty()) return 0;
        std::string strs = __src + __delimiter;
        size_t pos = strs.find(__delimiter);
        size_t len = strs.size();
        while (pos != std::string::npos) {
            std::string x = strs.substr(0, pos);
            if (x != "") __res.push_back(x.c_str());
            strs = strs.substr(pos + __delimiter.size(), len);
            pos = strs.find(__delimiter);
        }
        return __res.size();
    }

    inline std::string strip(const std::string &__str, char ch=' ') {
        if (__str.empty()) return std::string();
        int i = 0, j = __str.size() - 1;
        while (__str[i] == ch) i++;
        while (__str[j] == ch) j--;
        if (i > j) return std::string();
        return __str.substr(i, j - i + 1);
    }

    inline s32 str_to_int(const char *__val) {
        zassert(__val, "null pointer");
        return atoi(__val);
    }

    inline float str_to_float(const char *__val) {
        zassert(__val, "null pointer");
        return atof(__val);
    }

    inline s64 str_to_int64(const char *__val) {
        zassert(__val, "null pointer");
        return atoll(__val);
    }

    inline std::string int_to_str(const s32 __val) {
        char str[128] = {0};
        snprintf(str, sizeof(str), "%d", __val);
        return str;
    }

    inline std::string int64_to_str(const s64 __val) {
        char str[128] = {0};
        snprintf(str, sizeof(str), "%ld", __val);
        return str;
    }

    inline std::string float_to_str(const double __val) {
        char str[128] = {0};
        snprintf(str, sizeof(str), "%f", __val);
        return str;
    }

    inline void str_replace(std::string &__content, const std::string &__src, const std::string &__dest) {
        std::size_t pos = 0;
        std::size_t sz_src = __src.size();
        std::size_t sz_dest = __dest.size();
        while ((pos = __content.find(__src, pos)) != std::string::npos) {
            __content.replace(pos, sz_src, __dest);
            pos += sz_dest;
        }
    }

    inline s32 rand(s32 __range) {
        if (0 == __range) { return 0; }
        static u64 s_seed = time::milliseconds();
        s_seed = (((s_seed = s_seed * 214013L + 2531011L) >> 16) & 0x7fff);
        return s_seed % __range;
    }

}

    inline std::string& operator<<(std::string &__target, const std::string &__val) {
        __target += __val;
        return __target;
    }

    inline std::string& operator<<(std::string &__target, const char *__val) {
        __target += std::string(__val);
        return __target;
    }

    inline std::string& operator<<(std::string &__target, const s32 __val) {
        __target += tool::int_to_str(__val);
        return __target;
    }

    inline std::string& operator<<(std::string &__target, const s64 __val) {
        __target += tool::int64_to_str(__val);
        return __target;
    }

    inline std::string& operator<<(std::string &__target, const double __val) {
        __target += tool::float_to_str(__val);
        return __target;
    }

#endif // __ZTOOLS_H__