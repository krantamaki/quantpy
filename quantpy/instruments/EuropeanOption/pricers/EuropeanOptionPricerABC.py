"""
@package quantpy.instruments.EuropeanOption.pricers.EuropeanOptionPricerABC
@brief Abstarct base class for European option pricers
@author Kasper Rantamäki
@date 2026-01-16
"""
from __future__ import annotations
from abc import ABC, abstractmethod
from typing import List, Dict, Callable


class EuropeanOptionPricerABC(ABC):
  """Abstract base class for European option pricers"""
  
  
  @abstractmethod
  def __call__(self, underlying_value: float, time_to_maturity: float, *args: List[any], **kwargs: Dict[any, any]) -> float:
    """
    @brief Call method
    @details Call method calculates the price of the European option given the market price of the underlying and time to maturity
    @param underlying_value  The market price of the underlying security
    @param time_to_maturity  The time to maturity in years
    @param *args             Additional class specific arguments
    @param **kwargs          Additional class specific keyword arguments
    @returns                 The value of the European option
    """
    pass
  
  
  @abstractmethod
  def __str__(self) -> str:
    """Simple string representation"""
    pass
  
  
  @abstractmethod
  def __repr__(self) -> str:
    """Exhaustive string representation"""
    pass
  
  
  def __ge__(self, other: EuropeanOptionPricerABC) -> bool:
    """Greater than or equal to comparison"""
    return repr(self) >= repr(other)
  
  
  def __le__(self, other: EuropeanOptionPricerABC) -> bool:
    """Less than or equal to comparison"""
    return repr(self) <= repr(other)
  
  
  def __gt__(self, other: EuropeanOptionPricerABC) -> bool:
    """Greater than comparison"""
    return repr(self) > repr(other)
  
  
  def __lt__(self, other: EuropeanOptionPricerABC) -> bool:
    """Less than comparison"""
    return repr(self) < repr(other)
  
  
  def __eq__(self, other: EuropeanOptionPricerABC) -> bool:
    """Equality comparison"""
    return repr(self) == repr(other)
  
  
  @property
  @abstractmethod
  def volatility(self) -> float:
    """The volatility"""
    pass
  
  
  @property
  @abstractmethod
  def risk_free_rate(self) -> float:
    """The (continuously compounding) risk-free rate"""
    pass
  
  
  @abstractmethod
  def delta(self, underlying_value: float, time_to_maturity: float, *args: List[any], **kwargs: Dict[any, any]) -> float:
    """
    @brief The option delta (sensitivity to the value of the underlying)
    @param underlying_value  The market price of the underlying security
    @param time_to_maturity  The time to maturity in years
    @param *args             Additional class specific arguments
    @param **kwargs          Additional class specific keyword arguments
    @returns                 The delta of the European option
    """
    pass
  
  
  @abstractmethod
  def gamma(self, underlying_value: float, time_to_maturity: float, *args: List[any], **kwargs: Dict[any, any]) -> float:
    """
    @brief The option gamma (sensitivity to the delta)
    @param underlying_value  The market price of the underlying security
    @param time_to_maturity  The time to maturity in years
    @param *args             Additional class specific arguments
    @param **kwargs          Additional class specific keyword arguments
    @returns                 The gamma of the European option
    """
    pass
  
  
  @abstractmethod
  def vega(self, underlying_value: float, time_to_maturity: float, *args: List[any], **kwargs: Dict[any, any]) -> float:
    """
    @brief The option vega (sensitivity to the volatility)
    @param underlying_value  The market price of the underlying security
    @param time_to_maturity  The time to maturity in years
    @param *args             Additional class specific arguments
    @param **kwargs          Additional class specific keyword arguments
    @returns                 The vega of the European option
    """
    pass
  
  
  @abstractmethod
  def rho(self, underlying_value: float, time_to_maturity: float, *args: List[any], **kwargs: Dict[any, any]) -> float:
    """
    @brief The option rho (sensitivity to the risk-free rate)
    @param underlying_value  The market price of the underlying security
    @param time_to_maturity  The time to maturity in years
    @param *args             Additional class specific arguments
    @param **kwargs          Additional class specific keyword arguments
    @returns                 The rho of the European option
    """
    pass
  
  
  @abstractmethod
  def theta(self, underlying_value: float, time_to_maturity: float, *args: List[any], **kwargs: Dict[any, any]) -> float:
    """
    @brief The option theta (sensitivity to the time)
    @param underlying_value  The market price of the underlying security
    @param time_to_maturity  The time to maturity in years
    @param *args             Additional class specific arguments
    @param **kwargs          Additional class specific keyword arguments
    @returns                 The theta of the European option
    """
    pass