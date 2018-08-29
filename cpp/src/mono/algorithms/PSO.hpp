#pragma once

#include <cstddef>
#include <vector>
#include <algorithm>
#include <random>

#include "../Problem.hpp"
#include "../Result.hpp"

namespace mono::algorithms {
    using std::size_t;
    using std::vector;

    class PSO {
        size_t popsize;
        double c1, c2;
        double inertia;

        vector<double> solutions;
        vector<double> velocity;
        vector<double> particleBestSolutions;
        vector<double> globalBestSolution;
        vector<double> particleBestCosts;
        vector<double> costs;
        double globalBestCost;

        std::default_random_engine generator;
        std::uniform_real_distribution<double> randomUniform;

        void initialize(Problem& problem);
        void evaluate(Problem& problem);
        void evolve(Problem& problem);

    public:
        PSO(size_t popsize, double c1, double c2, double inertia);
        Result solve(Problem& problem, size_t iterations);
    };
};