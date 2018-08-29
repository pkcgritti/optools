#include <thread>
#include <iostream>

#include "utils/Chronometer.hpp"
#include <vector>
#include <algorithm>
#include <future>

#define N_THREADS 2

class Adder {
    double secondMethodWorker(double* start, size_t len) {
        double result = 0.0;
        for (size_t i = 0; i < len; i++)
            result += *(start + i);
        return result;
    }
public:
    double firstMethod(std::vector<double>& vec) {
        double sum = 0;
        for (size_t i = 0; i < vec.size(); i++) {
            sum += vec[i];
        }
        return sum;
    }

    double secondMethod(std::vector<double>& vec) {
        size_t batchSize = vec.size() / N_THREADS;
        std::vector<std::future<double>> futures(N_THREADS);
        double result = 0.0;
        for (size_t i = 0, j = 0; i < vec.size(); i += batchSize, j++) {
            futures[j] = std::async([=, &vec] { return secondMethodWorker(vec.data() + i, batchSize); });
            // std::future<double> fut = std::async([=, &vec] { return secondMethodWorker(vec.data() + i, batchSize); });
            // futures.push_back(std::move(fut));
        }
        for (size_t i = 0; i < futures.size(); i++) {
            result += futures[i].get();
        }
        return result;
    }
};




int main(int argc, char* argv[]) {
    std::vector<double> vec;
    for (int i = 0; i < 100000000; i++) {
        vec.push_back(1.0);
    }

    utils::Chronometer::getInstance().execute([&vec] {
        Adder adder;
        for (int i = 0; i < 10; i++) {
            adder.firstMethod(vec);
        }
        std::cout << adder.firstMethod(vec) << std::endl;
    }).digest();

    utils::Chronometer::getInstance().execute([&vec] {
        Adder adder;
        for (int i = 0; i < 10; i++) {
            adder.secondMethod(vec);
        }
        std::cout << adder.secondMethod(vec) << std::endl;
    }).digest();

    utils::Chronometer::getInstance().execute([] {
        std::thread thread([] {
            std::cout << 2.0 << std::endl;
        });
        thread.join();
        // std::cout << fut.get() << std::endl;
    }).digest();

    utils::Chronometer::getInstance().execute([] {
        std::cout << 2.0 << std::endl;
    }).digest();
    
    return 0;
}