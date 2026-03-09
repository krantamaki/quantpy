"""
@package quantpy.math.curve.TermStructureABC
@author Kasper Rantamäki
@date 2026-02-22
"""
from typing import Union, Dict, Optional
from abc import abstractmethod, ABC
import numpy as np


class TermStructureABC(ABC):
  """Abstract base class for term structure models"""
  
  
  @abstractmethod
  def __call__(self, maturity: Union[np.ndarray[float], float]) -> Union[np.ndarray[float], float]:
    """
    @brief The call method
    @details Computes the value on the term structure for a given year fraction
    @param maturity  The given year fraction
    @returns         The value on the term structure
    """
    pass


  @property
  @abstractmethod
  def params(self) -> Optional[Dict[any, any]]:
    """
    @brief Property for accessing the model parameters (if applicable)
    """
    pass
  