#include "ztools.h"

#include <unistd.h>
#include <sys/types.h>
#include <libgen.h>

namespace ztool {
    namespace zsystem {
        s32 get_current_process_id() {
            return ::getpid();
        }
    }

    namespace zfile {
        const char* get_app_path() {
            static char *s_path = nullptr;
            const size_t kPathLen = 256;
            if (nullptr == s_path) {
                char buf[kPathLen];
                char link[kPathLen];
                memset(buf, 0, sizeof(buf));
                memset(link, 0, sizeof(link));
                snprintf(link, sizeof(link), "/proc/self/...");
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
    }
}