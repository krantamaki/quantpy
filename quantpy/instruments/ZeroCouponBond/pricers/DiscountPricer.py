"""
@package quantpy.instruments.ZeroCouponBond.pricers.DiscountPricer
@author Kasper Rantamäki
@date 2026-02-22
"""
import numpy as np

from .ZeroCouponBondPricerABC import ZeroCouponBondPricerABC
from ....QuantDatetime import QuantDatetime


class DiscountPricer(ZeroCouponBondPricerABC):
  """Class for pricing zero-coupon bonds via discounting"""
  
  
  def __init__(self, maturity_date: QuantDatetime, notional: float = 100.) -> None:
    """
    @brief Constructor for discount pricer
    @param maturity_date  The maturity date of the zero-coupon bond
    @param notional       The notional of the zero-coupon bond
    @returns              None
    """
    self.__maturity_date = maturity_date
    self.__notional      = notional
  
  
  def __call__(self, current_date: QuantDatetime, risk_free_rate: float) -> float:
    return self.__notional * np.exp(-risk_free_rate * current_date.timedelta(self.__maturity_date))
