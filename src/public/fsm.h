#ifndef __FSM_H__
#define __FSM_H__

#include <vector>
#include <map>
#include <functional>
#include <algorithm>

namespace fsm {
    typedef std::function<void()> Callback;
    typedef std::function<void(int)> CommandHandler;
    typedef int State;
    typedef std::vector<State> StatesVector;
    typedef std::pair<State, State> StatePair;
    typedef std::map<StatePair, Callback> CallbackMap;

    class FSM {
    public:
        FSM(std::vector<State> __states, int __start) : states_(__states), current_(__start) {}

        void add(const State __state) {
            auto it = std::find(states_.begin(), states_.end(), __state);
            if (it != states_.end()) {
                states_.push_back(__state);
            }
        }

        void del(const State __state) {
            auto it = std::find(states_.begin(), states_.end(), __state);
            if (it != states_.end()) { states_.erase(it); }
        }

        bool go(const State __state) {
            auto it = std::find(states_.begin(), states_.end(), __state);
            if (it != states_.end()) {
                auto last = current_;
                auto it_exit = callbacks_exit_.find(StatePair(current_, __state));
                auto it_enter = callbacks_enter_.find(StatePair(current_, __state));
                if (it_exit != callbacks_exit_.end()) it_exit->second();
                current_ = *it;
                if (it_enter != callbacks_enter_.end()) it_enter->second();
                return true;
            }
            return false;
        }

        Callback & on_exit(const State &__from, const State &__to) {
            return callbacks_exit_[StatePair(__from, __to)];
        }

        Callback & on_enter(const State &__from, const State &__to) {
            return callbacks_enter_[StatePair(__from, __to)];
        }

        void annul_exit(const State &__from, const State &__to) {
            auto it = callbacks_exit_.find(StatePair(__from, __to));
            if (it != callbacks_exit_.end()) {
                callbacks_exit_.erase(it);
            }
        }
        void annul_enter(const State &__from, const State &__to) {
            auto it = callbacks_enter_.find(StatePair(__from, __to));
            if (it != callbacks_enter_.end()) {
                callbacks_enter_.erase(it);
            }
        }

        void command(int __cmd) {
            if (command_) { command_(__cmd); }
        }

        CommandHandler & command() {
            return command_;
        }

    private:
        StatesVector states_;
        State current_;
        CallbackMap callbacks_exit_;
        CallbackMap callbacks_enter_;
        CommandHandler command_;
    };
}

#endif // __FSM_H__