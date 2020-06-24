#include "zthread.h"

#include <pthread.h>


namespace zlib {

    static void* thread_proc(void *__param) {
        zthread *ptr = reinterpret_cast<zthread *>(__param);
        zassert(ptr, "the pointer to zthread is null");
        ptr->run();
        return ptr;
    }

    bool zthread::start(s32 __threads) {
        for (s32 i = 0; i < __threads; i++) {
            pthread_t pid = 0;
            if (int ret = ::pthread_create(&pid, nullptr, thread_proc, (void*)this); ret != 0) {
                return false;
            } else {
                // TODO SOMETHING ...
            }
        }
        return true;
    }
}