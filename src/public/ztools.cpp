#include "ztools.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>

namespace tool {
    
    s32 get_process_id() {
        return ::getpid();
    }

    namespace file {
        const char* get_app_path() {
            static char *s_path = nullptr;
            const size_t kPathLen = 256;
            if (nullptr == s_path) {
                char buf[kPathLen];
                char link[kPathLen];
                memset(buf, 0, sizeof(buf));
                memset(link, 0, sizeof(link));
                snprintf(link, sizeof(link), "/proc/self/emaild.d");
                if (int cnt = readlink(link, buf, kPathLen); cnt >= 256) {
                    zassert(false, "system error: path too long");
                    return nullptr;
                }
                const char *p = dirname(buf);
                s_path = new char[kPathLen + 2];
                snprintf(s_path, kPathLen, "%s/", p);
            }
            return s_path;
        }

        bool mkdir(const char *__path) {
            if(::access(__path, S_IRUSR) == 0) { return true; }
            int res = ::mkdir(__path, S_IRUSR | S_IWUSR | S_IXUSR);
            return res == 0;
        }

        bool rmdir(const char *__path) {
            return ::rmdir(__path) == 0;
        }
        bool rmfile(const char *__path) {
            return ::remove(__path) == 0;
        }
    }
}