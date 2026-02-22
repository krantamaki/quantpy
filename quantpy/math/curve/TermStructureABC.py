"""
@package quantpy.math.curve.TermStructureABC
@author Kasper Rantamäki
@date 2026-02-22
"""
from abc import abstractmethod, ABC


class TermStructureABC(ABC):
  """Abstract base class for term structure models"""
  
  
  @abstractmethod
  def __call__(self, tau: float) -> float:
    """
    @brief The call method
    @details Computes the value on the term structure for a given year fraction
    @param tau  The given year fraction
    @returns    The value on the term structure
    """
    pass
