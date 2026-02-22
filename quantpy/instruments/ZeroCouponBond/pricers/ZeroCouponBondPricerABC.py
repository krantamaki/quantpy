"""
@package quantpy.instruments.ZeroCouponBond.pricers.ZeroCouponBondPricerABC
@author Kasper Rantamäki
@date 2026-02-22
"""
from abc import ABC, abstractmethod

from ....QuantDatetime import QuantDatetime


class ZeroCouponBondPricerABC(ABC):
  """Abstact base class for zero-coupon bond pricers"""
  
  @abstractmethod
  def __call__(self, current_date: QuantDatetime, risk_free_rate: float) -> float:
    """
    @brief Call method
    @details Prices the zero-coupon bond
    @param current_date    The datetime for which the bond is priced
    @param risk_free_rate  The risk-free rate
    @returns               The value of the zero-coupon bond
    """
    pass