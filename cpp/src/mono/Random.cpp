#include "Random.hpp" 

#include <iostream>

thread_local mono::Random* mono::Random::instance = 0;

namespace mono {
    Random::Random() {
        std::random_device rd;
        generator = std::default_random_engine{rd()};
        uniformDistribution = std::uniform_real_distribution<double>(0.0, 1.0);
    }

    Random& Random::getInstance() {
        if (Random::instance == nullptr) {
            Random::instance = new Random();
        }
        return *Random::instance;
    }

    double Random::nextUniform() {
        return uniformDistribution(generator);
    }
};