#ifndef __LOGIC_H__
#define __LOGIC_H__

#include "zinterface.h"

namespace zcore {
    class Logic {
    public:
        static Logic * instance();

        virtual bool launch();
        virtual void process(const s32 __overtime);
        virtual void shutdown();

        virtual iModule * find_module(const char *__name);
        virtual void load_module(const char *__path);
    };
}

#endif // __LOGIC_H__