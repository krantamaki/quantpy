"""
@package quantpy.math.curve.NelsonSiegel
@author Kasper Rantamäki
@date 2026-02-24
"""
from typing import List, Union, Optional, Dict
import numpy as np
from scipy.optimize import curve_fit

from .TermStructureABC import TermStructureABC


__all__ = ["NelsonSiegel"]


def _nelson_siegel(maturities: np.ndarray[float], tau: float, beta_0: float, beta_1: float, beta_2: float) -> np.ndarray[float]:
  """Nelson-Siegel model. Can be passed to scipy.optimize.curve_fit"""
  return beta_0 + beta_1 * (1. - np.exp(-maturities / tau)) / (maturities / tau) \
    + beta_2 * ((1. - np.exp(-maturities / tau)) / (maturities / tau) - np.exp(-maturities / tau))


class NelsonSiegel(TermStructureABC):
  """
  @brief Term-structure model based on the Nelson-Siegel model
  @details For more information on the model see the original paper [1]
  [1]: C. R. Nelson and A. F. Siegel, “Parsimonious modeling of yield curves,” Journal of Business,
  vol. 60, no. 4, pp. 473-489, 1987
  """
  
  def __init__(self, yields: np.ndarray[float], maturities: np.ndarray[float], init_params: Optional[List[float]] = None, **kwargs: Dict[any, any]) -> None:
    """
    @brief Constructor method for the Nelson-Siegel term-structure model
    @param yields       Array of observed zero-coupon yields
    @param maturities   Array of year fractions corresponding with the zero-coupon yields
    @param init_params  Initial guess for the model parameters as a list of form [tau, beta_0, beta_1, beta_2]. Optional
    @param **kwargs     Optional keyword arguments. To be passed to the scipy.optimize.curve_fit function
    @returns            None
    """
    popt, _ = curve_fit(_nelson_siegel, maturities, yields, p0=init_params, **kwargs)
    
    self.__tau    = popt[0]
    self.__beta_0 = popt[1]
    self.__beta_1 = popt[2]
    self.__beta_2 = popt[3]
    
    
  def __call__(self, maturity: Union[np.ndarray[float], float]) -> Union[np.ndarray[float], float]:
    return _nelson_siegel(maturity, self.__tau, self.__beta_0, self.__beta_1, self.__beta_2)
    
