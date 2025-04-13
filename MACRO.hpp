#pragma once
#include <iostream>
#include "fmt_includes.h"


    #ifdef _DEBUG

    #define PAUSE_IF_DEBUGGING   \
      do { \
        system("pause");\
        } while(0)

    #else // !_DEBUG
    #define PAUSE_IF_DEBUGGING do {} while(0)
    #endif

#define TODO(msg, ...)  \
    do{\
    fmt::println(stderr, "Not yet implemented [{}:{} in {}]: {}", __FILE__, __LINE__, __func__, fmt::format(msg, ##__VA_ARGS__)); \
    PAUSE_IF_DEBUGGING; \
    std::exit(1); \
}while (0)


#define Panic(msg, ...)  \
do {    \
    fmt::println(stderr, "Manual abort at [{}:{} in {}]:\n{}", __FILE__, __LINE__, __func__, fmt::format(msg, ##__VA_ARGS__)); \
    PAUSE_IF_DEBUGGING; \
    std::exit(1); \
} while (0)