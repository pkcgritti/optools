#include "Chronometer.hpp"

#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <numeric>
#include <functional>
#include <cmath>

namespace utils {
    Chronometer* Chronometer::instance = 0;

    Chronometer& Chronometer::digest() {
        if (!measurements.empty()) {
            if (measurements.size() == 1) {
                double elapsed = measurements.front();
                measurements.pop_front();
                std::pair<double, std::string> formatted = Chronometer::formatNanoseconds(elapsed);
                std::cout << "Elapsed time is " << formatted.first << " " << formatted.second << std::endl;
            } else {
                size_t size = measurements.size();
                double sum = std::accumulate(measurements.begin(), measurements.end(), 0.0);
                double meanTime = sum / size;
                double maxTime = *std::max_element(measurements.begin(), measurements.end());
                double minTime = *std::min_element(measurements.begin(), measurements.end());
                std::transform(measurements.begin(), measurements.end(), measurements.begin(), [&meanTime] (double& value) {
                    double x = value - meanTime;
                    return x * x;
                });
                double stdTime = std::sqrt(std::accumulate(measurements.begin(), measurements.end(), 0.0) / (meanTime - 1));
                std::pair<double, std::string> fSum = Chronometer::formatNanoseconds(sum);
                std::pair<double, std::string> fMean = Chronometer::formatNanoseconds(meanTime);
                std::pair<double, std::string> fMax = Chronometer::formatNanoseconds(maxTime);
                std::pair<double, std::string> fMin = Chronometer::formatNanoseconds(minTime);
                std::pair<double, std::string> fStd = Chronometer::formatNanoseconds(stdTime);
                std::cout << "Total elapsed time is " << fSum.first << " " << fSum.second << " in " << size << " runs" << std::endl;
                std::cout << "Mean          " << fMean.first << " " << fMean.second << std::endl;
                std::cout << "Max           " << fMax.first << " " << fMax.second << std::endl;
                std::cout << "Min           " << fMin.first << " " << fMin.second << std::endl;
                std::cout << "Deviation     " << fStd.first << " " << fStd.second << std::endl;
            }
        }
        return *this;
    }

    std::pair<double, std::string> Chronometer::formatNanoseconds (double value) {
        const std::vector<std::string> choices = {
            "nanoseconds",
            "microseconds",
            "milliseconds",
            "seconds",
            "minutes"
        }; 
        size_t index = 0;
        while (value > 1000 && index < choices.size() - 1) {
            value /= 1000;
            index++;
        }
        return std::make_pair(value, choices[index]);
    }

    std::chrono::time_point<std::chrono::steady_clock> Chronometer::now() {
        return std::chrono::steady_clock::now();
    }

    Chronometer& Chronometer::getInstance() {
        if (Chronometer::instance == 0)
            Chronometer::instance = new Chronometer();
        return *Chronometer::instance;
    }

    void Chronometer::destroy() {
        if (Chronometer::instance != 0)
            delete Chronometer::instance;
        Chronometer::instance = 0;
    }
};
