#pragma once

#if defined (_MSC_VER)
    #define thread_local __declspec( thread )
#elif defined (__GCC__)
    #define thread_local __thread
#elif defined (__CLANG__)
    #define thread_local __thread
#endif

#include <random>

namespace mono {
    class Random {
        static thread_local Random* instance;
        std::default_random_engine generator;
        std::uniform_real_distribution<double> uniformDistribution;
        Random ();
    public:
        static Random& getInstance();
        double nextUniform();
    };  
};