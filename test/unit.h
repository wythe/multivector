#pragma once
#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>
#include <sstream>
#include <stdexcept>

namespace wythe {
class timer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time, stop_time;
public:
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    double stop() {
        stop_time = std::chrono::high_resolution_clock::now();
        return milli();
    }

    double nano() const {
        return double(std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count());
    }

    double micro() const {
        return double(std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time).count());
    }

    double milli() const {
        return double(std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time).count());
    }
};

inline std::string to_string(const timer & timer) {
    std::ostringstream os;
    if (timer.milli() > 0) os << timer.milli() << " ms";
    else if (timer.micro() > 0) os << timer.micro() << " us";
    else os << timer.nano() << " ns";
    return os.str();
}

inline std::ostream & operator<<(std::ostream & os, const timer & timer) {
    os << to_string(timer);
    return os;
}
enum Platform {

    MSVC = 0x000000001,  
    Apple = 0x000000002, 
    Linux = 0x000000004,
    Unknown = 0x000000008
};

inline Platform hostPlatform()
{
#if defined(_MSC_VER) 
    return MSVC;
#endif
#if defined(__APPLE__)
    return Apple;
#endif
#if defined(__linux__)
    return Linux;
#endif
    return Unknown;
}
}

#ifdef IT_ASSERT
#undef IT_ASSERT
#endif
#define IT_ASSERT(condition) \
    do { \
    if (!(condition)) throw wythe::unit_error(#condition, __FILE__, \
        __LINE__); \
    } while (0)

#ifdef IT_ASSERT_MSG
#undef IT_ASSERT_MSG
#endif
#define IT_ASSERT_MSG(desc, condition) \
    do { \
        if (!(condition)) { \
            std::ostringstream os; \
            os << desc << ": " << #condition; \
            throw wythe::unit_error(os.str(), __FILE__, __LINE__); \
        } \
    } while (0)

#define IT_FORCE_ASSERT(desc) \
    do { \
        std::ostringstream os;  \
        os << desc; \
        throw wythe::unit_error(os.str(), __FILE__, __LINE__); \
    } while (0)

#define IT_WARN_MSG(desc, condition) \
    do { \
        if (!(condition)) { \
            std::ostringstream os; \
            os << desc << ": " << #condition; \
            throw unit_error(os.str(), __FILE__, __LINE__, true); \
        } \
    } while (0)

namespace wythe {

struct unit_error {
    unit_error(const std::string & desc, const char *file, int line,
        bool warn = false) : desc(desc), file(file), line(line), warn(warn) {}
    std::string desc;
    std::string file;
    int line;
    bool warn;
};

template <typename T> 
class unit_test {
    public:
        typedef void (T::* Pointer)();

        struct test {
            test(Pointer p, unsigned int mask = 0, bool iso = false) : 
                p(p), mask(mask), iso(iso) {}
            Pointer p;
            unsigned int mask;
            bool iso;
        };

        unit_test (T * test) : _test(test), _mask(0), _iso(false) {
            tests.reserve(100);
            _test->register_tests(*this);
        }
        void skip(unsigned int mask = 0xFFFFFFFF) { _mask = mask; }
        void skip(Pointer p) {
            test test(p, 0xFFFFFFFF);
            tests.push_back(test);
        }
        void cont() { _mask = 0; }
        void add(Pointer p) {
            test test(p, _mask);
            tests.push_back(test);
        }
        void iso(Pointer p) {
            test test(p, 0, true);
            tests.push_back(test);
            _iso = true;
        }
        int run() {
            wythe::timer tmr;
            int skipped = 0;
            typename std::vector<test>::iterator it;
            tmr.start();
            for (it = tests.begin(); it != tests.end(); ++it) {
                Pointer p = it->p;
                try {
                    if ((_iso && !it->iso) || (wythe::hostPlatform() & it->mask)) {
                        std::cerr << "S";
                        skipped++;
                    } else {
                        std::cerr << ".";
                        (_test->*p)();

                    }
                } catch (unit_error & e) {
                    std::cerr << std::endl << e.file << ":" << 
                        e.line << ": ";
                    if (e.warn) std::cerr << "warning: ";
                    else std::cerr << "error: ";
                    std::cerr << e.desc << std::endl;

                    if (!e.warn) return 1;
                } 
                catch (std::exception & e) {
                    std::cerr << "\n The unit test framework caught an unexpected exception: " << e.what() << '\n';
                    return 1;
                }
            }
            tmr.stop();
            std::cerr << "\ntotal time: " << wythe::to_string(tmr);

            if (skipped) {
                std::cerr << std::endl << "warning: " << skipped << 
                    " test" << ((skipped > 1) ? "s " : " ") << "skipped";
            }
            std::cerr << std::endl;
            return 0;
        }

    private:
        std::vector<test> tests;
        T * _test;
        unsigned int _mask;
        bool _iso;
};

}
