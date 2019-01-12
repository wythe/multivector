#pragma once
#include <sstream>
#include <iostream>
#include <stdexcept>
#ifdef ANDROID
#include <android/log.h>
#endif

namespace wythe {
inline std::runtime_error create_exception(const std::string & desc) {
    return std::runtime_error(desc);
}
inline std::runtime_error create_exception(const std::string & desc, const char * file, int line) {
    std::ostringstream os;
    os << '[' << file << ':' << line << "] " << desc;
    return std::runtime_error(os.str());
}
}

// Print to std::cerr a description with source file and line number.  Or log on ANDROID platforms.
#ifdef ANDROID
#define IT_WARN(s) \
do { \
    std::ostringstream os; \
    os << s; \
    auto e = wythe::create_exception(os.str(), __FILE__, __LINE__); \
    __android_log_print(ANDROID_LOG_VERBOSE, "xenon", e..what()); \
} while (0)
#else
#define IT_WARN(s) \
do { \
    std::ostringstream os; \
    os << s; \
    auto e = wythe::create_exception(os.str(), __FILE__, __LINE__); \
    std::cerr << e.what() << '\n'; \
} while (0)
#endif

// Throw a std::runtime_error with description.
#define IT_FATAL(desc) \
do { \
    std::ostringstream os; \
    os << desc; \
    throw wythe::create_exception(os.str()); \
} while (0)

// Throw a std::runtime_error with description and source file and line number added.
#define IT_PANIC(desc) \
do { \
    std::ostringstream os; \
    os << desc; \
    throw wythe::create_exception(os.str(), __FILE__, __LINE__); \
} while (0)

