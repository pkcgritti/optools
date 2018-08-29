#include "Problem.hpp"

namespace mono {
    Problem::Problem(size_t dimension) {
        setDimension(dimension);
    }

    Problem::Problem(size_t dimension, double lowerBoundary, double upperBoundary) {
        vector<double> lb(dimension), ub(dimension);
        std::generate(lb.begin(), lb.end(), [&lowerBoundary] () {
            return lowerBoundary;
        });
        std::generate(ub.begin(), ub.end(), [&upperBoundary] () {
            return upperBoundary;
        });
        setDimension(dimension);
        setBoundaries(lb, ub);
    }

    Problem::Problem(size_t dimension, std::vector<double> lowerBoundaries, std::vector<double> upperBoundaries) {
        setDimension(dimension);
        setBoundaries(lowerBoundaries, upperBoundaries);
    }
};