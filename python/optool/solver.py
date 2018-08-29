from optool.problem import Problem
from optool.algorithm import Algorithm
from optool.result import Result

from time import time_ns, sleep

class Solver:
    def __init__(self, problem: Problem, algorithm: Algorithm):
        self.problem = problem
        self.algorithm = algorithm
        self.result = None
    
    def solve(self, iterations: int = 1000, refreshRate: int = 200):
        start = time_ns()
        self.result = self.algorithm.solve(self.problem, iterations, refreshRate)
        end = time_ns()
        print('\nElapsed time: {} seconds'.format((end - start) * 1e-9))
        return self.result
