#ifndef __TIMER_H__
#define __TIMER_H__

#include <chrono>
#include <iostream>

#include "../types/types.hpp"

namespace modules {
    
    class Timer {
    public:
        Timer(const char *label) : _label(label) {
            // gets the current time
            std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
            // stores the current time in ms
            this->_start = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
            // print the start
            std::cout << "\033[36m ~ \033[34mTimer@" << this->_label << ":\033[0m started .." << std::endl;
        }

        ~Timer() {
            // gets the current time
            std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
            // gets the time difference in ms
            types::s64 df = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count() - this->_start;
            // prints the time difference
            std::cout << "\033[36m ~ \033[34mTimer@" << this->_label << ":\033[0m finished in " << df << "ns, "
                      << df / 1000.0f << "μs" << "," << df / 100000.0f << "ms" << std::endl;
        }

    private:
        const char *_label;
        types::s64 _start;
    };
}

#endif //_TIMER_H__