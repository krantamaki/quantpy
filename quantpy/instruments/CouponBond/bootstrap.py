"""
@package quantpy.instruments.CouponBond.bootstrap
@author Kasper Rantamäki
@date 2026-02-22
"""
from typing import List, Tuple
import numpy as np

from ...QuantDatetime import QuantDatetime
from .CouponBond import CouponBond


def bootstrap(current_date: QuantDatetime, bonds: List[CouponBond], prices: List[float]) -> List[Tuple[float, float]]:
  """
  @brief Calculates the zero-coupon discount factors from the prices of zero-coupon and coupon-bearing bonds
  @details Discount factors are found by solving a linear system of equations. If there is no bond maturing at a given
  coupon date, linear interpolation of the discount factors is applied. The choice of linear interpolation is a direct
  result of using a linear system to represent the bootstrapping problem.
  @param bonds            List of coupon-bearing (or zero-coupon) bonds issued by a single entity
  @param prices           The prices corresponding with the coupon-bearing bonds
  @raises AssertionError  Raised if the length of arrays doesn't match or the bonds can not be used for solving the discount factors
  @returns                List of tuples of form (<year fraction>, <discount factor>)
  """
  assert len(bonds) == len(prices), f"There needs to be an equal number of bonds and prices! ({len(bonds)} != {len(prices)})"
  
  bond_price_tuples = list(zip(bonds, prices))
  bond_price_tuples.sort(key=lambda tup: tup[0].maturity_date)
  
  bond_arr  = [tup[0] for tup in bond_price_tuples]
  price_arr = [tup[1] for tup in bond_price_tuples]
  tau_arr   = [current_date.timedelta(tup[0].maturity_date) for tup in bond_price_tuples]
  
  system_matrix = np.zeros((len(price_arr), len(tau_arr)))
  
  for row, bond in enumerate(bond_arr):
    active_zeros = [zero for zero in bond.zeros if zero.maturity_date >= current_date]
    
    if row == 0:
      assert len(active_zeros) == 1, f"The bond with shortest time to maturity must have a single coupon!"

    for zero in active_zeros:
      zero_tau = current_date.timedelta(zero.maturity_date)
      
      if zero_tau in tau_arr:
        system_matrix[row, tau_arr.index(zero_tau)] += zero.notional
      else:
        try:
          sup_i = [i for i, val in enumerate(tau_arr) if val >= zero_tau][0]
        except IndexError:
          print(zero_tau)
          print(tau_arr)
          print(repr(zero))
          print(repr(bond))
          raise IndexError("Index out of range")
        
        inf_i = sup_i - 1
        
        sup = tau_arr[sup_i]
        inf = tau_arr[inf_i] if inf_i >= 0 else 0.
        
        inf_weight = (zero_tau - inf) / (sup - inf)
        
        print(inf, zero_tau, sup, inf_weight)
        print()
        
        system_matrix[row, inf_i] += inf_weight * zero.notional
        system_matrix[row, sup_i] += (1. - inf_weight) * zero.notional
        
  discount_factors = np.linalg.solve(system_matrix, np.array(price_arr))
  
  return list(zip(tau_arr, discount_factors))
 