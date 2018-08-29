#include "Result.hpp"

#include <iostream>
#include <algorithm>

namespace mono {
    Result::Result(vector<double> solution) {
        setSolution(solution);
        setCost(0);
    }
    
    Result::Result(vector<double> solution, double cost) {
        setSolution(solution);
        setCost(cost);
    }

    Result::Result(vector<double> solution, double cost, vector<double> history) {
        setSolution(solution);
        setCost(cost);
        setHistory(history);
    }

    void Result::displaySolution() const {
        std::cout << "Cost     : " << cost << std::endl << "Solution : ";
        std::for_each(solution.begin(), solution.end(), [] (double value) {
            std::cout << value << "  ";
        });
        std::cout << std::endl;
    }

};