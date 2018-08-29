#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

#include "mono/algorithms/PSO.hpp"
#include "mono/Random.hpp"
#include "mono/Problem.hpp"
#include "mono/Result.hpp"
#include "utils/Chronometer.hpp"

class Quadratic: public mono::Problem {
public:
    Quadratic(size_t dimension): Problem(dimension) {
        std::vector<double> lb(dimension);
        std::vector<double> ub(dimension);
        std::generate(lb.begin(), lb.end(), [] () { return -100.0; });
        std::generate(ub.begin(), ub.end(), [] () { return 100.0; });
        setBoundaries(lb, ub);
    }

    double evaluate(double *solution) {
        // using namespace std::chrono_literals;
        double result = 0;
        for (size_t i = 0; i < dimension; i++) {
            double x = solution[i];
            result += x * x;
        }
        // std::this_thread::sleep_for(1ms);
        return result;
    }
};

using namespace utils;

int main(int argc, char* argv[]) {
    mono::algorithms::PSO pso(40, 1.0, 1.0, 0.9);
    Quadratic problem(20);

    Chronometer& chrono = Chronometer::getInstance();
    
    chrono.executeMany(50, [&pso, &problem] {
        mono::Result result = pso.solve(problem, 1000);
        result.displaySolution();
    }).digest();
}
