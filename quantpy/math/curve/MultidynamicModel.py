"""
@package quantpy.math.curve.MultidynamicModel
@author Kasper Rantamäki
@date 2026-02-24
"""
from typing import List, Union, Optional
import numpy as np
from scipy.optimize import curve_fit

from .TermStructureABC import TermStructureABC


__all__ = ["MultidynamicModel"]


def _multidynamic_model(maturities: np.ndarray[float], tau: float, beta_0: float, beta_1: float, 
                        beta_2: float, beta_3: float, beta_4: float) -> np.ndarray[float]:
  """Multidynamic model. Can be passed to scipy.optimize.curve_fit"""
  return beta_0 + beta_1 / 2. * maturities + tau / maturities * (beta_2 + beta_3) * (1. - np.exp(-maturities / tau)) \
    + beta_3 * np.exp(-maturities / tau) - beta_4 * np.pow(maturities, 2.)


class MultidynamicModel(TermStructureABC):
  """
  @brief Proposed term-structure model
  """
  
  def __init__(self, yields: np.ndarray[float], maturities: np.ndarray[float], init_params: Optional[List[float]] = None, **kwargs) -> None:
    """
    @brief Constructor method for the Svensson term-structure model
    @param yields       Array of observed zero-coupon yields
    @param maturities   Array of year fractions corresponding with the zero-coupon yields
    @param init_params  Initial guess for the model parameters as a list of form [tau, beta_0, beta_1, beta_2, beta_3, beta_4]. Optional
    @param **kwargs     Optional keyword arguments. To be passed to the scipy.optimize.curve_fit function
    @returns            None
    """
    popt, _ = curve_fit(_multidynamic_model, maturities, yields, p0=init_params, **kwargs)
    
    self.__tau    = popt[0]
    self.__beta_0 = popt[1]
    self.__beta_1 = popt[2]
    self.__beta_2 = popt[3]
    self.__beta_3 = popt[4]
    self.__beta_4 = popt[5]
    
    
  def __call__(self, maturity: Union[np.ndarray[float], float]) -> Union[np.ndarray[float], float]:
    return _multidynamic_model(maturity, self.__tau, self.__beta_0, self.__beta_1, self.__beta_2, self.__beta_3, self.__beta_4)
    
