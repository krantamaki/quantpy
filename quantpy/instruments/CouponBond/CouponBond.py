"""
@package quantpy.instruments.CouponBond.CouponBond
@author Kasper Rantamäki
@date 2026-02-22
"""
from typing import Literal, List


from ..ZeroCouponBond.ZeroCouponBond import ZeroCouponBond
from ...QuantDatetime import QuantDatetime
from ...math.curve.TermStructureABC import TermStructureABC


class CouponBond:
  """Class for pricing coupon bonds"""
  
  def __init__(self, issuance_date: QuantDatetime, maturity_date: QuantDatetime, coupon_rate: float, 
               notional: float = 100., coupon_freq: int = 1, zero_pricer: Literal["CIR", "CIRPlusPlus", "Vasicek", "ExtendedVasicek", "Discount"] = "Discount") -> None:
    """
    @brief Constructor for coupon bearing bond
    @details The coupon dates are assumed to coincide at equivalent dates to the maturity date, i.e., if the maturity date coincides with month end then 
    each coupon date is also month end or if the maturity date is the 15th of the month then coupon dates will also be paid on the 15th 
    @param issuance_date   The date when the bond was issued
    @param maturity_date   The maturity date of the bond
    @param coupon_rate     The coupon rate for the bond
    @param notional        The notional of the bond. Optional, defaults to 100
    @param coupon_freq     The number of coupons the bond pays yearly. Optional, defaults to 1
    @param zero_pricer     The pricing method for the zero coupon bonds. Options are ['CIR', 'CIRPlusPlus', 'Vasicek', 'ExtendedVasicek', 'Discount']. 
    Defaults to 'Discount' i.e. standard dicounting
    @returns               None
    """
    assert zero_pricer in ["CIR", "CIRPlusPlus", "Vasicek", "ExtendedVasicek", "Discount"], f"Invalid pricer name specified! ({zero_pricer} not in ['CIR', 'CIRPlusPlus', 'Vasicek', 'ExtendedVasicek', 'Discount'])"
    
    self.__issuance    = issuance_date
    self.__maturity    = maturity_date
    self.__coupon      = coupon_rate
    self.__notional    = notional
    self.__coupon_freq = coupon_freq
    self.__pricer_name = zero_pricer
    
    total_coupons = round(issuance_date.timedelta(maturity_date) / (1. / coupon_freq))
    
    self.__zeros  = []
    
    for i in range(total_coupons):
      year_shift   = (maturity_date.month - i * 12 // coupon_freq) // 12
      coupon_month = (maturity_date.month - i * 12 // coupon_freq) % 12 
      
      if coupon_month <= 0:
        coupon_month = coupon_month + 12
      
      if maturity_date.is_month_end():
        coupon_date = QuantDatetime(year=maturity_date.year + year_shift, month=coupon_month, day=1, 
                                    hour=maturity_date.hour, minute=maturity_date.minute, second=maturity_date.second, millisecond=maturity_date.millisecond,
                                    calendar = maturity_date.calendar, convention = maturity_date.convention)
        coupon_date = coupon_date.next_month_end()
      else:
        coupon_date = QuantDatetime(year=maturity_date.year + year_shift, month=coupon_month, day=maturity_date.day, 
                                    hour=maturity_date.hour, minute=maturity_date.minute, second=maturity_date.second, millisecond=maturity_date.millisecond,
                                    calendar = maturity_date.calendar, convention = maturity_date.convention)
      
      if i == 0:
        self.__zeros.append(ZeroCouponBond(coupon_date, self.notional * (1. + coupon_rate / coupon_freq)))
      else:
        self.__zeros.append(ZeroCouponBond(coupon_date, self.notional * (coupon_rate / coupon_freq)))
    
    
  def __call__(self, current_date: QuantDatetime, term_structure: TermStructureABC) -> float:
    """
    @brief The pricing method
    @param current_date    The date for which the bond is priced
    @param term_structure  The term structure of interest rates for the given date
    @returns               The price of the bond
    """
    return sum([zero(current_date, term_structure(current_date.timedelta(zero.maturity_date))) for zero in self.__zeros if current_date < zero.maturity_date])
  
  
  def __str__(self) -> str:
    """Simple string representation"""
    return f"Maturity date: {self.__maturity}\nNotional: {self.__notional}\nCoupon rate: {self.__coupon}\nCoupon frequency: {self.__coupon_freq}"
  
  
  def __repr__(self) -> str:
    """Comprehensive string representation"""
    return f"{str(self)}\nIssuance date: {self.__issuance}\nZero pricer: {self.__pricer_name}"
  
  
  @property
  def maturity_date(self) -> QuantDatetime:
    """The maturity date for the bond"""
    return self.__maturity
  
  
  @property
  def issuance_date(self) -> QuantDatetime:
    """The issuance date for the bond"""
    return self.__issuance
  
  
  @property
  def notional(self) -> float:
    """The notional of the bond"""
    return self.__notional
  
  
  @property
  def zero_pricer(self) -> Literal["CIR", "CIRPlusPlus", "Vasicek", "ExtendedVasicek", "Discount"]:
    """The name of the zero-coupon pricer"""
    return self.__pricer_name
  
  
  @property
  def coupon_freq(self) -> int:
    """The number of coupons paid yearly"""
    return self.__coupon_freq


  @property
  def zeros(self) -> List[ZeroCouponBond]:
    """The list of constituent zero-coupon bonds"""
    return self.__zeros
    