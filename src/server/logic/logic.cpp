#include "logic.h"

namespace core {
    Logic * Logic::instance() {
        static Logic *s_logic = nullptr;
        if (nullptr == s_logic) { 
            s_logic = new Logic();
            s_logic->launch();
        }
        return s_logic;
    }
    // 导入需要的资源
    bool Logic::launch() { return true; }

    void Logic::process(const s32 __overtime) {}

    // 释放资源
    void Logic::shutdown() {}

    iModule * Logic::find_module(const char *__name) { return nullptr; }
    void Logic::load_module(const char *__path) {}
}