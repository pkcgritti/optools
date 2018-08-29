import numpy as np

from optool.algorithm.evolutionary import EvolutionaryAlgorithm
from optool import Algorithm, Problem

class PSO(EvolutionaryAlgorithm):
    def __init__(self, particles: int, c1: float = 1.0, c2: float = 1.0, inertia: float = 0.9):
        self.particles = particles
        self.c1 = c1
        self.c2 = c2
        self.inertia = inertia

    def initialize(self, problem: Problem) -> np.ndarray:
        dimension = problem.dimension
        rb = problem.rangeBounds
        lb = problem.lowerBounds

        self.position = np.random.random((self.particles, dimension)) * rb + lb
        self.velocity = np.zeros((self.particles, dimension))
        self.pBest = np.array(self.position)
        self.gBest = np.zeros((1, dimension))
        self.pBestCost = np.Inf * np.ones((self.particles, 1))
        self.gBestCost = np.Inf
        self.dimension = dimension
        
        return self.position
    
    def getSolution(self):
        return self.gBest[0]
    
    def getSolutionCost(self):
        return self.gBestCost

    def beforeEvolve(self, problem: Problem, costs: np.ndarray):
        for p in range(self.particles):
            if costs[p] < self.pBestCost[p]:
                self.pBestCost[p] = costs[p]
                self.pBest[p,:] = self.position[p,:]
                if costs[p] < self.gBestCost:
                    self.gBestCost = costs[p]
                    self.gBest[0,:] = self.position[p,:]
    
    def evolve(self, problem: Problem, costs: np.ndarray, iteration: int) -> np.ndarray:
        self.beforeEvolve(problem, costs)
        shape = self.position.shape
        self.velocity = self.inertia * self.velocity \
            + self.c1 * np.random.random(shape) * (self.pBest - self.position) \
            + self.c2 * np.random.random(shape) * (self.gBest - self.position)
        self.position = self.position + self.velocity
        return self.position
