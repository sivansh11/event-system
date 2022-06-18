#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

#define RUNTIME_ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Runtime assertion `" #condition "` failed: " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)

#define RUNTIME_WEAK_ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Runtime weak assertion `" #condition "` failed: " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)


#ifndef NDEBUG 
#define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)

#define WEAK_ASSERT(condition, message) \
    if (! (condition)) \
    { \
        std::cerr << "Weak Assertion '" #condition "' failed in " << __FILE__\
                << " line " << __LINE__ << ": " << message << std::endl; \
    }

#else
#define WEAK_ASSERT(condition, message) condition;
#define ASSERT(condition, message) condition;

#endif

#endif