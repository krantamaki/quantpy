"""
@package quantpy.math.curve.Svensson
@author Kasper Rantamäki
@date 2026-02-24
"""
from typing import List, Union, Optional
import numpy as np
from scipy.optimize import curve_fit

from .TermStructureABC import TermStructureABC


__all__ = ["Svensson"]


def _svensson(maturities: np.ndarray[float], tau_1: float, tau_2: float, 
              beta_0: float, beta_1: float, beta_2: float, beta_3: float) -> np.ndarray[float]:
  """Svensson model. Can be passed to scipy.optimize.curve_fit"""
  return beta_0 + beta_1 * (1. - np.exp(-maturities / tau_1)) / (maturities / tau_1) \
    + beta_2 * ((1. - np.exp(-maturities / tau_1)) / (maturities / tau_1) - np.exp(-maturities / tau_1)) \
    + beta_3 * ((1. - np.exp(-maturities / tau_2)) / (maturities / tau_2) - np.exp(-maturities / tau_2))


class Svensson(TermStructureABC):
  """
  @brief Term-structure model based on the Svensson model
  @details For more information on the model see the original paper [1]
  [1]: L. E. Svensson, “Estimating and interpreting forward interest rates: Sweden 1992-1994,” 
  Working Paper 4871, National Bureau of Economic Research, 1994.
  """
  
  def __init__(self, yields: np.ndarray[float], maturities: np.ndarray[float], init_params: Optional[List[float]] = None) -> None:
    """
    @brief Constructor method for the Svensson term-structure model
    @param yields       Array of observed zero-coupon yields
    @param maturities   Array of year fractions corresponding with the zero-coupon yields
    @param init_params  Initial guess for the model parameters as a list of form [tau_1, tau_2, beta_0, beta_1, beta_2, beta_3]. Optional
    @returns            None
    """
    popt, _ = curve_fit(_svensson, maturities, yields, p0=init_params)
    
    self.__tau_1  = popt[0]
    self.__tau_2  = popt[1]
    self.__beta_0 = popt[2]
    self.__beta_1 = popt[3]
    self.__beta_2 = popt[4]
    self.__beta_3 = popt[5]
    
    
  def __call__(self, maturity: Union[np.ndarray[float], float]) -> Union[np.ndarray[float], float]:
    return _svensson(maturity, self.__tau_1, self.__tau_2, self.__beta_0, self.__beta_1, self.__beta_2, self.__beta_3)
    
