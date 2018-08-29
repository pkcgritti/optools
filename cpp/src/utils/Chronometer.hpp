#pragma once

#include <deque>
#include <chrono>
#include <utility>
#include <string>

namespace utils {
    class Chronometer {
        static Chronometer* instance;
        std::deque<double> measurements;

        Chronometer() {}
    public:
        template <typename F, typename ...Args>
        Chronometer& execute(F&& f, Args&& ...args) {
            auto startTime = Chronometer::now();
            std::forward<F>(f)(std::forward<Args>(args)...);
            auto endTime = Chronometer::now();
            std::chrono::duration<double, std::nano> duration = endTime - startTime;
            measurements.push_front(duration.count());
            return *this;
        }

        template <typename F, typename ...Args>
        Chronometer& executeMany(size_t times, F&& f, Args&& ...args) {
            for (size_t t = 0; t < times; t++) {
                execute(f, std::forward<Args>(args)...);
            }
            return *this;
        }

        Chronometer& digest();
        static std::pair<double, std::string> formatNanoseconds (double value);
        static std::chrono::time_point<std::chrono::steady_clock> now();
        static Chronometer& getInstance();
        static void destroy();
    };
};
