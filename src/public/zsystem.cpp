#include "zsystem.h"


#ifdef __cplusplus
extern "C" {
#endif
    void zassertion(const char *file, int line, const char *function, const char *debug) {
#ifdef DEBUG_MODE
        fflush(stdout);
        printf("ASSERT POS : %s:%d @function:%s\nASSERT INF : %s\n", file, line, function, debug);
        fflush(stdout);
        assert(false);
#endif // DEBUG_MODE
    }
#ifdef __cplusplus
};
#endif