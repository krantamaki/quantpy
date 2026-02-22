"""
@package quantpy.instruments.ZeroCouponBond.ZeroCouponBond
@author Kasper Rantamäki
@date 2026-02-22
"""
from typing import Literal

from ...QuantDatetime import QuantDatetime
from .pricers.DiscountPricer import DiscountPricer
from .pricers.ZeroCouponBondPricerABC import ZeroCouponBondPricerABC


class ZeroCouponBond:
  """Class for pricing zero-coupon bonds"""
  
  def __init__(self, maturity_date: QuantDatetime, notional: float = 100., pricer: Literal["CIR", "CIRPlusPlus", "Vasicek", "ExtendedVasicek", "Discount"] = "Discount"):
    """
    @brief Constructor for a zero-coupon bond 
    @param maturity_date  The maturity date of the zero-coupon bond
    @param notional       The notional of the zero-coupon bond
    @param pricer         The pricing method for the zero coupon bonds. Options are ['CIR', 'CIRPlusPlus', 'Vasicek', 'ExtendedVasicek', 'Discount']. 
    Defaults to 'Discount' i.e. standard dicounting
    @returns None
    """
    assert pricer in ["CIR", "CIRPlusPlus", "Vasicek", "ExtendedVasicek", "Discount"], f"Invalid pricer name specified! ({pricer} not in ['CIR', 'CIRPlusPlus', 'Vasicek', 'ExtendedVasicek', 'Discount'])"
    
    self.__maturity   = maturity_date
    self.__notional   = notional
    # self.__pricer_name = pricer
    
    self.__pricer = DiscountPricer(maturity_date, notional)
    
    
  def __call__(self, current_date: QuantDatetime, risk_free_rate: float) -> float:
    """
    @brief Call method
    @details Prices the zero-coupon bond
    @param current_date    The datetime for which the bond is priced
    @param risk_free_rate  The risk-free rate
    @returns               The value of the zero-coupon bond
    """
    return self.__pricer(current_date, risk_free_rate)
  
  
  @property
  def maturity_date(self) -> QuantDatetime:
    """The maturity date for the zero-coupon bond"""
    return self.__maturity
  
  
  @property
  def notional(self) -> float:
    """The notional of the zero-coupon bond"""
    return self.__notional
  
  
  @property
  def pricer(self) -> ZeroCouponBondPricerABC:
    """The pricer for the zero-coupon bond"""
    return self.__pricer
