from optool.algorithm.evolutionary import PSO
from optool.benchmarks import Quadratic
from optool import Solver

from time import time_ns

if __name__ == '__main__':
    problem = Quadratic(10)
    result = Solver(problem, PSO(40, c1 = 0.8, c2 = 1.2)).solve(200)
    result.plotHistory()
