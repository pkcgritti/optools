import numpy as np
import matplotlib.pyplot as plt

class Result:
    def __init__(self, solution: np.ndarray, cost: float, history: np.ndarray = None):
        self.__solution = solution
        self.__cost = cost
        self.__history = history
    
    @property
    def solution(self):
        return np.array(self.__solution)
    
    @property
    def cost(self):
        return float(self.__cost)
    
    @property
    def history(self):
        return np.array(self.__history)
    
    def display(self):
        print('Final cost is {}'.format(self.cost))
        print('with solution: {}'.format(str(self.solution)))
    
    def plotHistory(self):
        plt.plot(self.history, 'k--')
        plt.title('Cost history')
        plt.xlabel('Iteration')
        plt.ylabel('Cost')
        plt.grid()
        plt.show()


