"""
@package quantpy.math.curve.Anderson
@author Kasper Rantamäki
@date 2026-03-01
"""
from typing import Union, Optional, Dict, Tuple, Callable, List
import numpy as np
from scipy.integrate import quad
from scipy.linalg import solve

from .TermStructureABC import TermStructureABC


__all__ = ["Anderson"]


def _b_spline_elem(order: int, knots: List[float]) -> Callable[[float], float]:
  """A piecewise polynomial basis spline defined using De Boor recursion"""
  assert len(knots) == order + 2
  # assert knots == sorted(knots)
  
  if order == 0:
    
    def base_case(x: Union[float, np.ndarray[float]]) -> Union[float, np.ndarray[float]]:
      if hasattr(x, "__iter__"):
        x_tmp = np.array(x)
        return ((x_tmp >= knots[0]) & (x_tmp <= knots[1])).astype(int)
      
      return 1 if (x >= knots[0]) and (x <= knots[1]) else 0
    
    return base_case
  
  else:
    
    def recursion(x):
      if hasattr(x, "__iter__"):
        x_tmp = np.array(x)
        
        coef_0 = np.zeros(x_tmp.shape) if knots[order] - knots[0] == 0 else (x - knots[0]) / (knots[order] - knots[0])
        coef_1 = np.zeros(x_tmp.shape) if knots[order + 1] - knots[1] == 0 else (knots[order + 1] - x) / (knots[order + 1] - knots[1])
        return coef_0 * _b_spline_elem(order - 1, knots[:-1])(x) + coef_1 * _b_spline_elem(order - 1, knots[1:])(x)

      coef_0 = 0 if knots[order] - knots[0] == 0 else (x - knots[0]) / (knots[order] - knots[0])
      coef_1 = 0 if knots[order + 1] - knots[1] == 0 else (knots[order + 1] - x) / (knots[order + 1] - knots[1])
      return coef_0 * _b_spline_elem(order - 1, knots[:-1])(x) + coef_1 * _b_spline_elem(order - 1, knots[1:])(x)
    
    return recursion


def _b_spline_2nd_derivative(spline: Callable[[float], float], h: float = 1e-6) -> Callable[[float], float]:
  """The second derivative of a basis spline calculated with finite differences"""
  return lambda x: (spline(x + h) - 2 * spline(x) + spline(x - h)) / (h ** 2)


def _lambda(maturities: np.ndarray[float], L: float, S: float, i: float) -> np.ndarray[float]:
  """The smoothing function for the Anderson method"""
  return np.exp(L - (L - S) * np.exp(-maturities / i))


class Anderson(TermStructureABC):
  """
  @brief Term-structure model based on the Anderson basis spline method. This is more of a proof of concept than an efficient implementation
  @details For more information on the model see the original paper [1]
  [1]: N. Anderson and J. Sleath, “New estimates of the UK real and nominal yield curves,” 
  Bank of England Quarterly Bulletin, vol. 39, no. 4, pp. 384-392, 1999.
  """
  
  def __init__(self, yields: np.ndarray[float], maturities: np.ndarray[float], smoothing_params: Tuple[float, float, float], n_knots: Optional[int] = None, **kwargs: Dict[any, any]) -> None:
    """
    @brief Constructor method for the Anderson term-structure model
    @param yields            Array of observed zero-coupon yields
    @param maturities        Array of year fractions corresponding with the zero-coupon yields
    @param smoothing_params  The parameters for the smoothing function defined by Anderson and Sleath. Should be of form (L, S, i)
    @param n_knots           The number of knots used when defining the spline. Optional, defaults to len(yields) // 3
    @param **kwargs          Optional keyword arguments. To be passed to the scipy.optimize.curve_fit function
    @returns                 None
    """
    assert len(yields) == len(maturities), f"Lengths of the arrays must match! ({len(yields)} != {len(maturities)})"
    
    self.__y = np.array(yields)
    self.__m = np.array(maturities)
    
    self.__order   = 3
    self.__n_knots = n_knots if n_knots is not None else int(len(yields) / 3.) + 1
    
    m_0 = min(self.__m)
    m_n = max(self.__m)

    # Define the knot vector. The knots are assumed to be evenly spaced in the range defined by the observations
    self.__knots = np.linspace(m_0, m_n, self.__n_knots)
    
    # Pad the knot vector
    self.__knots   = np.array([m_0] * self.__order + list(self.__knots) + [m_n] * self.__order)
    self.__n_basis = len(self.__knots) - self.__order - 1
    
    # Build the basis matrix and the collect the basis splines into an array
    self.__B            = np.zeros((len(yields), self.__n_basis))
    self.__spline_elems = []
    
    for i in range(self.__n_basis):
      b_spline       = _b_spline_elem(self.__order, self.__knots[i:i + self.__order + 2])
      self.__B[:, i] = b_spline(self.__m)
      self.__spline_elems.append(b_spline)
      
    # Build the varying penalty term matrix
    self.__L = np.zeros((self.__n_basis, self.__n_basis))
    
    for i in range(self.__n_basis):
      for j in range(self.__n_basis):
        spline_i = self.__spline_elems[i]
        spline_j = self.__spline_elems[j]      
        
        func = lambda x: _b_spline_2nd_derivative(spline_i)(x) * _b_spline_2nd_derivative(spline_j)(x) * _lambda(x, *smoothing_params)
        
        L_ij = quad(func, m_0, m_n)
        
        self.__L[i, j] = L_ij[0]
    
    # Find the smoothing spline coefficients
    self.__a = solve(self.__B.T @ self.__B + self.__L, self.__B.T @ self.__y)
      
    
  def __call__(self, maturity: Union[np.ndarray[float], float]) -> Union[np.ndarray[float], float]:
    if hasattr(maturity, "__iter__"):
      return np.array([self(m) for m in maturity])
     
    return sum([self.__a[i] * self.__spline_elems[i](maturity) for i in range(len(self.__a))])
    
