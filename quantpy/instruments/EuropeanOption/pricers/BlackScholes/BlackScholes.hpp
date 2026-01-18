/**
 * @file BlackScholes.hpp
 * @brief Simple analytical Black-Scholes pricer for European options. Assumes no dividends
 * @author Kasper Rantamäki
 * @date 2026-01-16
 */

#ifndef BLACK_SCHOLES_HPP
#define BLACK_SCHOLES_HPP


#include <cmath>

#include "../../../../math/probability/normal.hpp"
#include "../../../../math/optimization/rootFinding.hpp"
#include "../BaseEuropeanOptionPricer.hpp"


namespace quantpy {

  namespace instruments {

    namespace EuropeanOption {


      /**
       * @brief Black-Scholes helper function \f$ d_+ \f$. To be called by 'BlackScholes' class
       * @tparam T   The template type. Should support cmath math operations
       * @param St   The market price of the underlying
       * @param tau  The time to maturity in years
       * @param r    The (continuously compounding) risk-free rate
       * @param K    The strike price
       * @param vol  The volatility
       * @returns    The value \f$ d_+ \f$
       */
      template <typename T>
      T __BlackScholesDp(T St, T tau, T r, T K, T vol) {

        return (T)1. / (vol * sqrt(tau)) * (log(St / K) + (r + vol * vol / (T)2.) * tau);

      }


      /**
       * @brief Black-Scholes helper function \f$ d_- \f$. To be called by 'BlackScholes' class
       * @tparam T   The template type. Should support cmath math operations
       * @param St   The market price of the underlying
       * @param tau  The time to maturity in years
       * @param r    The (continuously compounding) risk-free rate
       * @param K    The strike price
       * @param vol  The volatility
       * @returns    The value \f$ d_- \f$
       */
      template <typename T>
      T __BlackScholesDm(T St, T tau, T r, T K, T vol) {

        return __BlackScholesDp(St, tau, r, K, vol) - vol * sqrt(tau);

      }


      /**
       * @brief Black-Scholes option price. To be called by 'BlackScholes' class
       * @tparam T      The template type. Should support cmath math operations
       * @param St      The market price of the underlying
       * @param tau     The time to maturity in years
       * @param r       The (continuously compounding) risk-free rate
       * @param K       The strike price
       * @param vol     The volatility
       * @param isCall  Boolean flag telling if the option is a call or a put. Defaults to 'true'
       * @returns       The price of the option
       */
      template <typename T>
      T __BlackScholesPrice(T St, T tau, T r, T K, T vol, bool isCall = true) {

        if ( isCall ) {
          return quantpy::math::probability::normal::cdf<T>((T)0., (T)1., __BlackScholesDp(St, tau, r, K, vol)) * St -
                 quantpy::math::probability::normal::cdf<T>((T)0., (T)1., __BlackScholesDm(St, tau, r, K, vol)) * K * exp(-r * tau);
        }

        return quantpy::math::probability::normal::cdf<T>((T)0., (T)1., -__BlackScholesDm(St, tau, r, K, vol)) * K * exp(-r * tau) -
               quantpy::math::probability::normal::cdf<T>((T)0., (T)1., -__BlackScholesDp(St, tau, r, K, vol)) * St;

      }


      /**
       * @brief Black-Scholes implied volatility. To be called by 'BlackScholes' class
       * @tparam T          The template type. Should support cmath math operations
       * @param Vt          The market price of the option
       * @param St          The market price of the underlying
       * @param tau         The time to maturity in years
       * @param r           The (continuously compounding) risk-free rate
       * @param K           The strike price
       * @param isCall      Boolean flag telling if the option is a call or a put. Defaults to 'true'
       * @param lowerBound  The lower bound for the volatility. Defaults to 1e-6
       * @param upperBound  The upper bound for the volatility. Defaults to 10
       * @returns           The implied volatility of the option
       */
      template <typename T>
      T __BlackScholesImpliedVol(T Vt, T St, T tau, T r, T K, bool isCall = true, T lowerBound = (T)1e-6, T upperBound = (T)10.) {

        auto targetFunc = [=](T vol) {
          return Vt - __BlackScholesPrice(St, tau, r, K, vol, isCall);
        };

        return quantpy::math::optimization::rootFinding::bisectionMethod<T>(targetFunc, lowerBound, upperBound);

      }


      /**
       * @brief Simple pricer based on the analytical solution to the Black-Scholes equation
       * @tparam T  The template type. Should be some floating point number type
       */
      template <typename T>
      class BlackScholes : public BaseEuropeanOptionPricer<T> {

        public:

          /**
           * @brief Default constructor
           * @returns  Uninitialized BlackScholes object
           */
          BlackScholes() {  }


          /**
           * @brief Main constructor
           * @param r       The (continuously compounding) risk-free rate
           * @param K       The strike price 
           * @param vol     The volatility
           * @param isCall  Boolean flag telling if the option is a call or a put. Defaults to 'true'
           * @returns       The pricer object
           */
          BlackScholes(T r, T K, T vol, bool isCall = true) {

            this->_r = r;
            this->_K = K;
            this->_vol = vol;
            this->_isCall = isCall;

          }


          /**
           * @brief Market price constructor. Calculates the implied volatility for the given market price
           * @param Vt      The market price of the option
           * @param St      The market price of the underlying
           * @param tau     The time to maturity in years
           * @param r       The (continuously compounding) risk-free rate
           * @param K       The strike price 
           * @param isCall  Boolean flag telling if the option is a call or a put. Defaults to 'true'
           */
          BlackScholes(T Vt, T St, T tau, T r, T K, bool isCall = true) {

            this->_r = r;
            this->_K = K;
            this->_isCall = isCall;
            this->_vol = __BlackScholesImpliedVol<T>(Vt, St, tau, this->_r, this->_K, this->_isCall, (T)1e-6, (T)10.);

          }


          /**
           * @brief The pricing method
           * @param St   The market price of the underlying
           * @param tau  The time to maturity in years
           * @param r    The risk-free rate. Defaults to the one used to initialize the object
           * @param vol  The volatility. Defaults to the one used to initialize the object
           * @returns    The price of the option
           */
          T operator() (T St, T tau, T r = (T)-1., T vol = (T)-1.) const override {

            if ( r != (T)-1. && vol != (T)-1. ) {
              return __BlackScholesPrice<T>(St, tau, r, this->_K, vol, this->_isCall);
            }

            if ( r != (T)-1. ) {
              return __BlackScholesPrice<T>(St, tau, r, this->_K, this->_vol, this->_isCall);
            }

            if ( vol != (T)-1. ) {
              return __BlackScholesPrice<T>(St, tau, this->_r, this->_K, vol, this->_isCall);
            }

            return __BlackScholesPrice<T>(St, tau, this->_r, this->_K, this->_vol, this->_isCall);

          }


          /**
           * @brief Method for calculating the implied volatility. Doesn't overwrite the existing volatility value
           * @param Vt          The market price of the option
           * @param St          The market price of the underlying
           * @param tau         The time to maturity in years
           * @param lowerBound  The lower bound for the volatility. Defaults to 1e-6
           * @param upperBound  The upper bound for the volatility. Defaults to 10
           * @returns           The implied volatility of the option
           */
          const T impliedVol(T Vt, T St, T tau, T lowerBound = (T)1e-6, T upperBound = (T)10.) const {

            return __BlackScholesImpliedVol<T>(Vt, St, tau, this->_r, this->_K, this->_isCall, lowerBound, upperBound);

          }

      };


    }

  }
  
} 


#endif