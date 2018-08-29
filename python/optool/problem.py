from abc import ABC, abstractmethod

from numpy import array, ndarray

class Problem(ABC):
    def __init__(self, dimension: int, lowerBounds: ndarray, upperBounds: ndarray):
        self.__dimension = dimension
        
        if not isinstance(lowerBounds, (list, ndarray)):
            raise TypeError('lowerBounds: Expecting list or ndarray but got {}'.format(type(lowerBounds).__name__))
        if not isinstance(upperBounds, (list, ndarray)):
            raise TypeError('upperBounds: Expecting list or ndarray but got {}'.format(type(lowerBounds).__name__))
        if dimension < 1:
            raise ValueError('dimension: Must be greater then 0')
        if dimension != len(lowerBounds):
            raise ValueError('lowerBounds: Wrong dimension')
        if dimension != len(upperBounds):
            raise ValueError('upperBounds: Wrong dimension')
        
        self.__lowerBounds = array(lowerBounds)
        self.__upperBounds = array(upperBounds)
        self.__rangeBounds = self.__upperBounds - self.__lowerBounds
    
    @property
    def dimension(self):
        return self.__dimension
    
    @property
    def lowerBounds(self):
        return self.__lowerBounds

    @property
    def upperBounds(self):
        return self.__upperBounds
    
    @property
    def rangeBounds(self):
        return self.__rangeBounds
        
    @abstractmethod
    def evaluate(self, solutions: ndarray) -> ndarray:
        pass
