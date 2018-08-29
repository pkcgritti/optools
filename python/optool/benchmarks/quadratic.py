from optool.problem import Problem

import numpy as np

class Quadratic(Problem):
    def __init__(self, dimension):
        super().__init__(dimension, [-100.0 for x in range(dimension)], [100.0 for x in range(dimension)])
    
    def evaluate(self, solutions):
        return np.sum(solutions ** 2, axis = 1)
