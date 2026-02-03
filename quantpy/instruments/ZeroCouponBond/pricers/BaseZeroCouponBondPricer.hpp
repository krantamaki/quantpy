/**
 * @file BaseZeroCouponBondPricer.hpp
 * @brief Base class for zero-coupon bond pricers
 * @author Kasper Rantamäki
 * @date 2026-02-03
 */

#ifndef BASE_ZERO_COUPON_BOND_PRICER_HPP
#define BASE_ZERO_COUPON_BOND_PRICER_HPP


#include "../../../math/special/differentiation.hpp"


namespace quantpy {

  namespace instruments {

    namespace EuropeanOption {


      template<typename T>
      class BaseEuropeanOptionPricer {

        protected:

          T _vol;  /**< The volatility */

        public:

          /**
           * @brief Virtual function for pricing the zero-coupon bond. Needs to be overriden
           * @param rt   The prevailing (continuously compounded) interest rate
           * @param tau  The time to maturity in years
           * @param vol  The volatility. Optional and defaults to the one used to initialize the object
           * @returns    The price of the zero-coupon bond
           */
          virtual T operator() (T rt, T tau, T vol = (T)-1.) const {

            return (T)0.;

          };


          /**
           * @brief Setter for the volatility
           * @returns The volatility
           */
          T& vol() { return this->_vol; }
          

          /**
           * @brief Getter for the volatility
           * @returns The volatility
           */
          const T& vol() const { return this->_vol; }


          /**
           * @brief Virtual function for calculating the interest rate delta via finite differences
           * @param rt   The prevailing (continuously compounded) interest rate
           * @param tau  The time to maturity in years
           * @param h    The finite difference step size. Defaults to 1e-6
           * @returns    The interest rate delta for the bond
           */
          virtual const T delta(T rt, T tau, T h = 1e-6) const {

            auto deltaFunc = [=, this](T r) {
              return this->operator() (r, tau);
            };

            return quantpy::math::special::differentiation::firstCentralDiff<T>(deltaFunc, rt, h);

          }


          /**
           * @brief Virtual function for calculating the interest rate gamma via finite differences
           * @param rt   The prevailing (continuously compounded) interest rate
           * @param tau  The time to maturity in years
           * @param h    The finite difference step size. Defaults to 1e-6
           * @returns    The interest rate gamma for the bond
           */
          virtual const T gamma(T rt, T tau, T h = 1e-6) const {

            auto gammaFunc = [=, this](T r) {
              return this->operator() (r, tau);
            };

            return quantpy::math::special::differentiation::secondCentralDiff<T>(gammaFunc, rt, h);

          }


          /**
           * @brief Virtual function for calculating the vega for the bond via finite differences
           * @param rt   The prevailing (continuously compounded) interest rate
           * @param tau  The time to maturity in years
           * @param h    The finite difference step size. Defaults to 1e-6
           * @returns    The vega of the bond
           */
          virtual const T vega(T rt, T tau, T h = 1e-6) const {

            auto vegaFunc = [=, this](T vol) {
              return this->operator() (rt, tau, this->r(), vol);
            };

            return quantpy::math::special::differentiation::firstCentralDiff<T>(vegaFunc, this->_vol, h);

          }


          /**
           * @brief Virtual function for calculating the theta for the bond via finite differences
           * @param rt   The prevailing (continuously compounded) interest rate
           * @param tau  The time to maturity in years
           * @param h    The finite difference step size. Defaults to 1e-6
           * @returns    The theta of the bond
           */
          virtual const T theta(T rt, T tau, T h = 1e-6) const {

            auto thetaFunc = [=, this](T t) {
              return this->operator() (rt, t);
            };

            return quantpy::math::special::differentiation::firstCentralDiff<T>(thetaFunc, tau, h);

          }

      };


    }

  }

}


#endif