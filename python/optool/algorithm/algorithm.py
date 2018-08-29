from abc import ABC, abstractmethod

from optool.problem import Problem
from optool.result import Result

class Algorithm(ABC):
    @abstractmethod
    def solve(self, problem: Problem, iterations: int) -> Result:
        pass
