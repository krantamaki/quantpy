"""
@package quantpy.QuantDatetime
@brief Datetime class with correct day-count handling
@author Kasper Rantamäki
@date 2026-02-21
"""
from __future__ import annotations
from typing import Literal, Callable
from math import ceil
import QuantLib as ql


__all__ = ["QuantDatetime", "comparable"]


def _time_diff(end: QuantDatetime, start: QuantDatetime) -> float:
  """Calculates the year fraction for an intraday difference. Does the calculations to the accuracy of seconds"""
  return (end.hour - start.hour) / 8_760 + (end.minute - start.minute) / 525_600 + (end.second - start.second) / 31_536_000 
  

# Map from the name of the calendar to the ql.Calendar object
_calendar_map = {
  "Eurex"     : ql.Germany(ql.Germany.Eurex),
  "Frankfurt" : ql.Germany(ql.Germany.FrankfurtStockExchange),
  "Xetra"     : ql.Germany(ql.Germany.Xetra),
  "London"    : ql.UnitedKingdom(ql.UnitedKingdom.Exchange),
  "NYSE"      : ql.UnitedStates(ql.UnitedStates.NYSE)
}


# Map from the convention name to the function for calculating the time delta
_convention_map = {
  "30/360"       : lambda end, start: (360 * (end.year - start.year) + 30 * (end.month - start.month) + (end.day - start.day)) / 360 + _time_diff(end, start),
  "ACT/365"      : lambda end, start: start.days_until(end) / 365 + _time_diff(end, start),
  "ACT/360"      : lambda end, start: start.days_until(end) / 360 + _time_diff(end, start),
  "Business/252" : lambda end, start: start.bank_days_until(end) / 252 + _time_diff(end, start)
}


# Map from the convention name to the number of days in the year
_day_count_map = {
  "30/360"       : 360,
  "ACT/365"      : 365,
  "ACT/360"      : 360,
  "Business/252" : 252
}


def comparable(func: Callable[[QuantDatetime, QuantDatetime], any]) -> Callable:
  """
  @brief Decorator that asserts that two QfDate instances are comparable i.e. share the calendar and day count convention
  @param func  The function to be decorated
  @return      The decorated function
  """
  
  def wrapper(this: QuantDatetime, that: QuantDatetime) -> None:
    assert this.convention == that.convention, f"The conventions must match! ({this.convention} != {that.convention})"
    assert this.calendar  == that.calendar, f"The calendars must match! ({this.calendar} != {that.calendar})"
    
    return func(this, that)
  
  return wrapper


class QuantDatetime:
  """Datetime object with calendar and day-count functionality"""
  
  def __init__(self, year: int, month: int, day: int, 
               hour: int = 16, minute: int = 0, second: int = 0, millisecond: int = 0, 
               calendar: Literal["Eurex", "Frankfurt", "Xetra", "London", "NYSE"] = "Frankfurt", 
               convention: Literal["30/360", "ACT/365", "ACT/360", "Business/252"] = "Business/252") -> None:
    """
    @brief Main constructor
    @param year             The year of the datetime
    @param month            The month of the datetime. Needs to be in set {1, 2, ..., 12}
    @param day              The day of the datetime. Needs to be in set {1, 2, ..., 31}
    @param hour             The hour of the datetime. Needs to be in set {0, 1, ..., 23}
    @param minute           The minute of the datetime. Needs to be in set {0, 1, ..., 59}
    @param second           The second of the datetime. Needs to be in set {0, 1, ..., 59}
    @param millisecond      The millisecond of the datetime. Needs to be in set {0, 1, ..., 999}
    @param calendar         The used calendar. Options are ['Eurex', 'Frankfurt', 'Xetra', 'London', 'NYSE']. Defaults to 'Frankfurt'
    @param convention       The day-count convention. Options are ['30/360', 'ACT/365', 'ACT/360', 'Business/252']. Defaults to 'Business/252'
    @raises AssertionError  Raised if invalid values passed to the constructor
    @returns                None
    """
    
    assert (month > 0)        and (month < 13),         f"Month out of range! ({month} not between 1 and 12)"
    assert (day > 0)          and (day < 32),           f"Day out of range! ({day} not between 1 and 31)"
    assert (hour >= 0)        and (day < 25),           f"Hour out of range! ({hour} not between 0 and 23)"
    assert (minute >= 0)      and (minute < 60),        f"Minute out of range! ({minute} not between 0 and 59)"
    assert (second >= 0)      and (second < 60),        f"Second out of range! ({second} not between 0 and 59)"
    assert (millisecond >= 0) and (millisecond < 1000), f"millisecond out of range! ({millisecond} not between 0 and 999)"
    
    assert calendar in list(_calendar_map.keys()), f"Invalid calendar given! ({calendar} not in {list(_calendar_map.keys())})"
    assert convention in list(_convention_map.keys()), f"Invalid day count convention given! ({convention} not in {list(_convention_map.keys())})"
    
    self.__year            = year
    self.__month           = month
    self.__day             = day
    self.__hour            = hour
    self.__minute          = minute
    self.__second          = second
    self.__millisecond     = millisecond
    
    self.__calendar_name   = calendar
    self.__calendar        = _calendar_map[calendar]
    self.__convention_name = convention
    self.__convention      = _convention_map[convention]
    
    try:
      self.__ql_date = ql.Date(day, month, year)
    except Exception as e:
      assert False, f"Couldn't construct date {self.__year}-{self.__month:02}-{self.__day:02}!"
      
    self.__serial_number = self.__ql_date.serialNumber()
    
    
  def __str__(self) -> str:
    """Simple string representation"""
    return f"{self.__year}-{self.__month:02}-{self.__day:02} {self.__hour:02}:{self.__minute:02}:{self.__second:02}.{self.__millisecond:03}"
  
  
  def __repr__(self) -> str:
    """Comprehensive string representation"""
    return f"Date: {self}\nConvention: {self.__convention_name}\nCalendar: {self.__calendar_name}"


  def __hash__(self) -> str:
    """Hash for the datetime"""
    return hash(repr(self))
  
  
  @comparable
  def __eq__(self, other: QuantDatetime) -> bool:
    """Equality comparison operator"""
    return (self.year == other.year) and (self.month == other.month) and (self.day == other.day) and \
           (self.hour == other.hour) and (self.minute == other.minute) and (self.second == other.second) and (self.millisecond == other.millisecond)
  
  
  @comparable
  def __gt__(self, other: QuantDatetime) -> bool:
    """Greater than comparison operator"""
    return ((self.year == other.year) and (self.month == other.month) and (self.day == other.day) and (self.hour == other.hour) and (self.minute == other.minute) and (self.second == other.second) and (self.millisecond > other.millisecond)) or \
           ((self.year == other.year) and (self.month == other.month) and (self.day == other.day) and (self.hour == other.hour) and (self.minute == other.minute) and (self.second > other.second)) or \
           ((self.year == other.year) and (self.month == other.month) and (self.day == other.day) and (self.hour == other.hour) and (self.minute > other.minute)) or \
           ((self.year == other.year) and (self.month == other.month) and (self.day == other.day) and (self.hour > other.hour)) or \
           ((self.year == other.year) and (self.month == other.month) and (self.day > other.day)) or \
           ((self.year == other.year) and (self.month > other.month)) or \
           ((self.year > other.year)) 
  
  
  @comparable
  def __lt__(self, other: QuantDatetime) -> bool:
    """Less than comparison operator"""
    return not (self >= other)
  
  
  @comparable
  def __ge__(self, other: QuantDatetime) -> bool:
    """Greater than or equal to comparison operator"""
    return (self > other) or (self == other)
  
  
  @comparable
  def __le__(self, other: QuantDatetime) -> bool:
    """Less than or equal to comparison operator"""
    return (self < other) or (self == other)
  
  
  @property
  def year(self) -> int:
    """The year of the datetime"""
    return self.__year
  
  
  @property
  def month(self) -> int:
    """The month of the datetime"""
    return self.__month
  
  
  @property
  def day(self) -> int:
    """The day of the datetime"""
    return self.__day
  
  
  @property
  def hour(self) -> int:
    """The hour of the datetime"""
    return self.__year
  
  
  @property
  def minute(self) -> int:
    """The minute of the datetime"""
    return self.__month
  
  
  @property
  def second(self) -> int:
    """The second of the datetime"""
    return self.__day
  
  
  @property
  def millisecond(self) -> int:
    """The millisecond of the datetime"""
    return self.__day
  
  
  @property
  def calendar(self) -> str:
    """The calendar of the datetime"""
    return self.__calendar_name
  
  
  @property
  def convention(self) -> str:
    """The convention of the datetime"""
    return self.__convention_name
  
  
  def shift(self, years: float = 0., days: int = 0, hours: int = 0, minutes: int = 0, seconds: int = 0, milliseconds: int = 0) -> QuantDatetime:
    """
    @brief Shifts the datetime by the specified amount of time
    @details Note that as months are not of constant length, shifting by some number of months is not a permitted operation
    @param years         The year fraction by which the datetime is shifted (impacts the days only)
    @param days          The number of days by which the datetime is shifted
    @param hours         The number of hours by which the datetime is shifted
    @param minutes       The number of minutes by which the datetime is shifted
    @param seconds       The number of seconds by which the datetime is shifted
    @param milliseconds  The number of milliseconds by which the datetime is shifted
    @returns             A 'QuantDatetime' object shifted by the specified amount
    """
    new_millisecond = (self.millisecond + milliseconds) % 1000
    millisecond_res = (self.millisecond + milliseconds) // 1000
    new_second      = (self.second + seconds + millisecond_res) % 60
    second_res      = (self.second + seconds + millisecond_res) // 60
    new_minute      = (self.minute + minutes + second_res) % 60
    minute_res      = (self.minute + minutes + second_res) // 60
    new_hour        = (self.hour + hours + minute_res) % 24
    hour_res        = (self.hour + hours + minute_res) // 24
    day_shift       = days + hour_res + round(years * 365)
    
    new_ql_date = ql.Date(self.__serial_number + day_shift)
    
    return QuantDatetime(new_ql_date.year(), new_ql_date.month(), new_ql_date.dayOfMonth(), 
                         hour = new_hour, minute = new_minute, second = new_second, millisecond = new_millisecond, 
                         calendar=self.__calendar_name, convention=self.__convention_name)
  
  
  @comparable
  def timedelta(self, other_date: QuantDatetime) -> float:
    """
    @brief Calculates the year fraction between this and a given date
    @param other_date  The date of comparison
    @returns           The year fraction
    """
    if self > other_date:
      return -self.__convention(self, other_date)
    
    return self.__convention(other_date, self)
  
  
  @comparable
  def days_until(self, other_date: QuantDatetime) -> int:
    """
    @brief Counts the number of days from this until a given date
    @param other_date  The date of comparison
    @returns           The number of days
    """
    assert self <= other_date, f"The given date cannot be less than the instance date! ({self} < {other_date})"
    return other_date.__serial_number - self.__serial_number
  
  
  @comparable
  def days_since(self, other_date: QuantDatetime) -> int:
    """
    @brief Counts the number of days to this since a given date
    @param other_date  The date of comparison
    @returns           The number of days
    """
    assert self >= other_date, f"The given date cannot be greater than the instance date! ({self} < {other_date})"
    return other_date.days_until(self)
  
  
  @comparable
  def bank_days_until(self, other_date: QuantDatetime) -> int:
    """
    @brief Counts the number of banks days from this until a given date
    @details Inclusive from start but not end
    @param other_date  The date of comparison
    @returns           The number of bank days
    """
    assert self <= other_date, f"The given date cannot be less than the instance date! ({self} < {other_date})"
    
    bank_day_count = 0
    
    if self.is_bank_date():
      bank_day_count += 1
      
    date = self.next_bank_date()
    
    while date < other_date:
      bank_day_count += 1
      date = date.next_bank_date()
      
    return bank_day_count
  

  @comparable
  def bank_days_since(self, other_date: QuantDatetime) -> int:
    """
    @brief Counts the number of bank days to this since a given date
    @details Inclusive from start but not end
    @param other_date  The date of comparison
    @returns           The number of days
    """
    assert self >= other_date, f"The given date cannot be greater than the instance date! ({self} < {other_date})"
    return other_date.bank_days_until(self)
    

  def is_bank_date(self) -> bool:
    """
    @brief Checks if this date is a bank day
    @returns  True if this day is bank day False otherwisw
    """
    return self.__calendar.isBusinessDay(self.__ql_date)
  
  
  def next_bank_date(self) -> QuantDatetime:
    """
    @brief Iterates until the next bank day is found
    @returns  The next bank day
    """
    next_date = self.shift(days=1)
    
    while not next_date.is_bank_date():
      next_date = next_date.shift(days=1)
      
    return next_date
  
  
  def prev_bank_date(self) -> QuantDatetime:
    """
    @brief Iterates until the last bank day is found
    @returns  The lastt bank day
    """
    prev_date = self.shift(days=-1)
    
    while not prev_date.is_bank_date():
      prev_date = prev_date.shift(days=-1)
      
    return prev_date
