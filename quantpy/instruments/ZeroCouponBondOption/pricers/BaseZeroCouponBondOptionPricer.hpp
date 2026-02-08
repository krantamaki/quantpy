/**
 * @file BaseZeroCouponBondOptionPricer.hpp
 * @brief Base class for zero-coupon bond option pricers
 * @author Kasper Rantamäki
 * @date 2026-02-08
 */

#ifndef BASE_ZERO_COUPON_BOND_OPTION_PRICER_HPP
#define BASE_ZERO_COUPON_BOND_OPTION_PRICER_HPP


#include "../../../math/special/differentiation.hpp"


namespace quantpy {

  namespace instruments {

    namespace ZeroCouponBondOption {


      template<typename T>
      class BaseZeroCouponBondOptionPricer {

        protected:

          T _vol;        /**< The volatility */
          T _K;          /**< The strike price */
          bool _isCall;  /**< Boolean flag telling the option type */

        public:

          /**
           * @brief Virtual function for pricing the zero-coupon bond option. Needs to be overriden
           * @param rt   The prevailing (continuously compounded) interest rate
           * @param tau  The time to maturity in years
           * @param vol  The volatility. Optional and defaults to the one used to initialize the object
           * @returns    The price of the zero-coupon bond option
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
           * @brief Setter for the strike price
           * @returns The strike price
           */
          T& strike() { return this->_K; }
          

          /**
           * @brief Getter for the strike price
           * @returns The strike price
           */
          const T& strike() const { return this->_K; }


          /**
           * @brief Setter for the 'isCall' flag
           * @returns The 'isCall' flag
           */
          T& isCall() { return this->_isCall; }
          

          /**
           * @brief Getter for the 'isCall' flag
           * @returns The 'isCall' flag
           */
          const T& isCall() const { return this->_isCall; }


          /**
           * @brief Virtual function for calculating the interest rate delta via finite differences
           * @param rt   The prevailing (continuously compounded) interest rate
           * @param tau  The time to maturity in years
           * @param h    The finite difference step size. Defaults to 1e-6
           * @returns    The interest rate delta for the bond option
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
           * @returns    The interest rate gamma for the bond option
           */
          virtual const T gamma(T rt, T tau, T h = 1e-6) const {

            auto gammaFunc = [=, this](T r) {
              return this->operator() (r, tau);
            };

            return quantpy::math::special::differentiation::secondCentralDiff<T>(gammaFunc, rt, h);

          }


          /**
           * @brief Virtual function for calculating the vega for the bond option via finite differences
           * @param rt   The prevailing (continuously compounded) interest rate
           * @param tau  The time to maturity in years
           * @param h    The finite difference step size. Defaults to 1e-6
           * @returns    The vega of the bond option
           */
          virtual const T vega(T rt, T tau, T h = 1e-6) const {

            auto vegaFunc = [=, this](T vol) {
              return this->operator() (rt, tau, vol);
            };

            return quantpy::math::special::differentiation::firstCentralDiff<T>(vegaFunc, this->_vol, h);

          }


          /**
           * @brief Virtual function for calculating the theta for the bond option via finite differences
           * @param rt   The prevailing (continuously compounded) interest rate
           * @param tau  The time to maturity in years
           * @param h    The finite difference step size. Defaults to 1e-6
           * @returns    The theta of the bond option
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