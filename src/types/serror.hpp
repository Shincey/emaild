#ifndef __SERROR_HPP__
#define __SERROR_HPP__

#include <iostream>

#define DEBUG

// ----
// Preprocessing error messages
// ----
#define PREP_ERROR(a, b)                                                            \
    std::cout << "\033[31m[ERROR]: \033[0m" << __FILE__ << "@" << __LINE__ << ": "  \
    << a << ": " << b << std::endl;


#define PREP_ERROR_SIN(a)                                                           \
    std::cout << "\033[31m[ERROR]: \033[0m" << __FILE__ << "@" << __LINE__ << ": "  \
    << a << std::endl;



#endif // __SERROR_HPP__