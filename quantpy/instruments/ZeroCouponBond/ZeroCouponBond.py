"""
@package quantpy.instruments.ZeroCouponBond.ZeroCouponBond
@author Kasper Rantamäki
@date 2026-02-22
"""
from typing import Literal


from ...QuantDatetime import QuantDatetime
from .pricers.ZeroCouponPricerABC import ZeroCouponPricerABC


class ZeroCouponBond:
  """Class for pricing zero-coupon bonds"""
  
  def __init__(self, maturity_date: QuantDatetime, notional: float = 100., pricer: Literal["CIR", "CIRPlusPlus", "Vasicek", "ExtendedVasicek", "Discount"] = "Discount"):
    """
    @brief Constructor for a zero-coupon bond 
    @param 
    """
    assert pricer in ["CIR", "CIRPlusPlus", "Vasicek", "ExtendedVasicek", "Discount"], f"Invalid pricer name specified! ({pricer} not in ['CIR', 'CIRPlusPlus', 'Vasicek', 'ExtendedVasicek', 'Discount'])"
    
    self.__maturity   = maturity_date
    self.__notional   = notional
    self.__picer_name = pricer
    
    self.__pricer = ...
    
    
  def __call__(self, current_date: QuantDatetime, risk_free_rate: float) -> float:
    """
    
    """
    return 0.
  
  
  @property
  def maturity_date(self) -> QuantDatetime:
    """The maturity date for the zero-coupon bond"""
    return self.__maturity
  
  
  @property
  def notional(self) -> float:
    """The notional of the zero-coupon bond"""
    return self.__notional
  
  
  @property
  def pricer(self) -> ZeroCouponPricerABC:
    """The pricer for the zero-coupon bond"""
    return self.__pricer
  
  
  