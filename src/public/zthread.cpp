#include "zthread.h"

#include <pthread.h>


namespace zlib {

    // static void* threadProc(void *param) {
    //     zthread *ptr = reinterpret_cast<zthread *>(param);
    //     zassert(ptr, "the pointer to zthread is null");
    //     ptr->run();
    //     return ptr;
    // }

    // bool zthread::start(s32 threads) {
    //     for (s32 i = 0; i < threads; i++) {
    //         pthread_t pid = 0;
    //         if (int ret = pthread_create(&pid, nullptr, threadProc, (void*)this); ret != 0) {
    //             return false;
    //         } else {
    //             // TODO SOMETHING ...
    //         }
    //     }
    //     return true;
    // }
}