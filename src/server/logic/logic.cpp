#include "logic.h"

namespace zcore {
    Logic * Logic::instance() {
        static Logic *s_logic = nullptr;
        if (nullptr == s_logic) { 
            s_logic = new Logic();
            s_logic->launch();
        }
        return s_logic;
    }

    bool Logic::launch() { return false; }

    void Logic::process(const s32 __overtime) {}
    void Logic::shutdown() {}

    iModule * Logic::find_module(const char *__name) { return nullptr; }
    void Logic::load_module(const char *__path) {}
}