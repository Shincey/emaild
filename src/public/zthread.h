#ifndef __ZTHREAD_H__
#define __ZTHREAD_H__

#include "zsystem.h"

namespace zlib {
    enum {
        THREAD_STOPED = 0,
        THREAD_WORKING,
        THREAD_STOPPING,
    };

    class zthread {
    public:
        virtual ~zthread() {}

    public:
        bool start(s32 __threads=1);
        virtual void terminate() = 0;
        virtual void run() = 0;

    protected:
        s8 _status;
    };
}

#endif // __ZTHREAD_H__