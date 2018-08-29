#ifndef __Problem__
#define __Problem__

#include <cstddef>
#include <vector>
#include <algorithm>

namespace mono {
    using std::size_t;
    using std::vector;

    class Problem {
    protected:
        vector<double> lowerBoundaries;
        vector<double> upperBoundaries;
        size_t dimension;

    public:
        Problem(size_t dimension);
        Problem(size_t dimension, double lowerBoundary, double upperBoundary);
        Problem(size_t dimension, vector<double> lowerBoundaries, vector<double> upperBoundaries);
        virtual double evaluate(double* solution) = 0;

        void setDimension(size_t dimension) {
            this->dimension = dimension;
        }

        size_t getDimension() const {
            return dimension;
        }

        void setLowerBoundaries(vector<double> lowerBoundaries) {
            this->lowerBoundaries = vector<double>(lowerBoundaries);
        }

        const vector<double>& getLowerBoundaries() const {
            return lowerBoundaries;
        }

        void setUpperBoundaries(vector<double> upperBoundaries) {
            this->upperBoundaries = vector<double>(upperBoundaries);
        }

        const vector<double>& getUpperBoundaries() const {
            return upperBoundaries;
        }

        void setBoundaries(vector<double> lowerBoundaries, vector<double> upperBoundaries) {
            setLowerBoundaries(lowerBoundaries);
            setUpperBoundaries(upperBoundaries);
        }
    };
};

#endif