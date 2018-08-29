#pragma once

#include <cstddef>
#include <vector>

namespace mono {
    using std::size_t;
    using std::vector;

    class Result {
        vector<double> solution;
        vector<double> history;
        double cost;
    public:
        Result(vector<double> solution);
        Result(vector<double> solution, double cost);
        Result(vector<double> solution, double cost, vector<double> history);

        void displaySolution() const;

        void setSolution(vector<double> solution) {
            this->solution = vector<double>(solution);
        }

        const vector<double>& getSolution() const {
            return solution;
        }

        void setHistory(vector<double> history) {
            this->history = vector<double>(history);
        }

        const vector<double>& getHistory() const {
            return history;
        }

        void setCost(double cost) {
            this->cost = cost;
        }

        double getCost() const {
            return cost;
        }
    };
};