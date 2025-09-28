#pragma once

// We define all the macros but initialize them to 0 or 1 so you can write cleaner code like
// #if   THIS
// #elif THAT
// #endif

// ICARUS_COMPILER_VERSION format will be encoded as MMmm (major minor)
// so 10.04 becomes 1004, 3.0.1 is 300 etc.

#ifdef __clang__
#define ICARUS_COMPILER_CLANG
#define ICARUS_COMPILER_VERSION (__clang_major__ * 100 + __clang_minor__)
#elif defined(__GNUC__)
#define ICARUS_COMPILER_GCC
#define ICARUS_COMPILER_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)
#elif defined(_MSC_VER)
#define ICARUS_COMPILER_MSVC
#define ICARUS_COMPILER_VERSION (_MSC_VER)
#else
#error "Unsupported compiler."
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
#define ICARUS_OS_WINDOWS 1
#define ICARUS_OS_LINUX 0
#elif defined(__linux__)
#define ICARUS_OS_LINUX 1
#define ICARUS_OS_WINDOWS 0
#else
#error "Unsupported OS."
#endif
