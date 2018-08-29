from abc import abstractmethod

from optool.algorithm import Algorithm
from optool.problem import Problem
from optool.result import Result

import numpy as np

class EvolutionaryAlgorithm(Algorithm):
    @abstractmethod
    def initialize(self, problem: Problem) -> np.ndarray:
        pass

    @abstractmethod
    def evolve(self, problem: Problem, costs: np.ndarray, iteration: int):
        pass

    @abstractmethod
    def getSolution(self) -> np.ndarray:
        pass
    
    @abstractmethod
    def getSolutionCost(self) -> float:
        pass

    def evaluate(self, problem: Problem, population: np.ndarray) -> np.ndarray:
        costs = problem.evaluate(population)
        return costs

    def solve(self, problem: Problem, iterations: int, refreshRate: int = 200) -> Result:

        print('Starting solver for problem {} using {}\n'.format(type(problem).__name__, type(self).__name__))
        print('-' * 41)
        print('| %-10s | %-24s |' % ('Iteration', 'Best Cost'))
        print('-' * 41)

        history = []

        population = self.initialize(problem)
        for i in range(iterations):
            costs = self.evaluate(problem, population)
            population = self.evolve(problem, costs, i)
            bestAtMoment = self.getSolutionCost()
            if (i + 1) % refreshRate == 0 or i == 0 or i == iterations - 1:
                print('| %10d | %-24.17e |' % (i + 1, bestAtMoment))
            history.append(bestAtMoment)
            
        print('-' * 41 + '\n')

        costs = self.evaluate(problem, population)
        history.append(self.getSolutionCost())
        result = Result(self.getSolution(), self.getSolutionCost(), history)
        result.display()

        return result