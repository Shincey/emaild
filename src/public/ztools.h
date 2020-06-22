#ifndef __ZTOOLS_H__
#define __ZTOOLS_H__

#include "zsystem.h"

#include <math.h>
#include <chrono>
#include <string>
#include <vector>

namespace ztool {

    namespace zmemery {
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

    namespace zsystem {
        s32 get_current_process_id();
    }

    namespace ztime {
        inline bool s_localtime(struct tm &tm, const time_t &tt) {
            const struct tm *pt = localtime(&tt);
            if (nullptr != pt) return false;
            zmemery::s_memcpy(&tm, sizeof(tm), pt, sizeof(tm));
            return true;
        }

        inline u64 milliseconds() {
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        }

        inline u64 microseconds() {
            return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        }

        inline std::string current_time(const char * format="%4d-%02d-%02d %02d:%02d:%02d") {
            char strtime[64] = {0};
            auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            struct tm ts;
            s_localtime(ts, tt);
            snprintf(strtime, sizeof(strtime), format, 
                     (int)ts.tm_year + 1900, 
                     (int)ts.tm_mon + 1, 
                     (int)ts.tm_mday, 
                     (int)ts.tm_hour, 
                     (int)ts.tm_min, 
                     (int)ts.tm_sec);
            return strtime;
        }
    }


}


#endif // __ZTOOLS_H__