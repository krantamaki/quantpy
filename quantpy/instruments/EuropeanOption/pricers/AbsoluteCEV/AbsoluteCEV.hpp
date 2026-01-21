/**
 * @file AbsoluteCEV.hpp
 * @details Constant elasticity of volatility (CEV) family of stochastic processes was first considered
 * by Cox in 1975 [1]. Beckers [2] provided closed form solutions for the general CEV process
 * 
 * \f$ dS_t = \mu(t) S_t dt + \sigma(t) S_t^{\alpha / 2} dW_t , \f$
 * 
 * where \f$ \mu(t) \f$ is the time-dependent mean, \f$ \sigma(t) \f$ is the time-dependent volatility, 
 * \f$ \alpha \f$ is the elasticity factor and \f$ dW_t \f$ is the Brownian motion.
 * 
 * The absolute model is the CEV process with elasticity factor \f$ \alpha = 0 \f$ for which the a 
 * closed form solution is known.
 * 
 * [1]: J. Cox. "Notes on Option Pricing I: Constant Elasticity of Variance Diffusions." 
 * Working Paper Stanford University, 1975.
 * [2]: S. Beckers, “The constant elasticity of variance model and its implications for option pricing,”
 * The Journal of Finance, vol. 35, no. 3, pp. 661–673, 1980.
 * @author Kasper Rantamäki
 * @date 2026-01-21
 */

#ifndef ABSOLUTE_CEV_HPP
#define ABSOLUTE_CEV_HPP


#include <cmath>

#include "../../../../math/probability/normal.hpp"
#include "../../../../math/optimization/rootFinding.hpp"
#include "../BaseEuropeanOptionPricer.hpp"


namespace quantpy {

  namespace instruments {

    namespace EuropeanOption {


      /**
       * 
       */
      template <typename T>
      T __AbsoluteCEVv(T tau, T r, T vol) {

        return vol * sqrt(((T)1. - exp(-(T)2. * r * tau)) / ((T)2. * r));

      }


      /**
       * 
       */
      template <typename T>
      T __AbsoluteCEVy1(T St, T tau, T r, T K, T vol, T v = (T)0.) {

        if ( v == (T)0. ) {
          v = __AbsoluteCEVv(tau, r, vol);
        }

        return (St - K * exp(-r * tau)) / v;

      }


      /**
       * 
       */
      template <typename T>
      T __AbsoluteCEVy2(T St, T tau, T r, T K, T vol, T v = (T)0.) {

        if ( v == (T)0. ) {
          v = __AbsoluteCEVv(tau, r, vol);
        }

        return (-St - K * exp(-r * tau)) / v;

      }


      /**
       * @brief Absolute CEV pricing function. To be called by the 'AbsoluteCEV' class
       * @tparam T      The template type. Should be some floating point type
       * @param St      The market price of the underlying
       * @param tau     The time to maturity in years
       * @param r       The (continuously compounding) risk-free rate
       * @param K       The strike price
       * @param vol     The volatility 
       * @param isCall  Boolean flag telling if the option is a call or a put. Defaults to 'true'
       * @returns       The price of a call option under the absolute CEV model
       */
      template <typename T>
      T __AbsoluteCEVPrice(T St, T tau, T r, T K, T vol, bool isCall = true) {

        T v  = __AbsoluteCEVv(tau, r, vol);
        T y1 = __AbsoluteCEVy1(St, tau, r, K, vol, v);
        T y2 = __AbsoluteCEVy2(St, tau, r, K, vol, v);

        T callPrice = (St - K * exp(-r * tau)) * quantpy::math::probability::normal::cdf((T)0., (T)1., y1)
                    + (St + K * exp(-r * tau)) * quantpy::math::probability::normal::cdf((T)0., (T)1., y2)
                    + v * (quantpy::math::probability::normal::pdf((T)0., (T)1., y1) - quantpy::math::probability::normal::pdf((T)0., (T)1., y2));

        if ( isCall ) {
          return callPrice;
        }

        return callPrice - St + K * exp(-((T)1. + r) * tau);

      }


      /**
       * @brief Volatility implied by the absolute CEV formula. To be called by the 'AbsoluteCEV' class
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
      T __AbsoluteCEVImpliedVol(T Vt, T St, T tau, T r, T K, bool isCall = true, T lowerBound = (T)1e-6, T upperBound = (T)10.) {

        auto targetFunc = [=](T vol) {
          return Vt - __AbsoluteCEVPrice(St, tau, r, K, vol, isCall);
        };

        return quantpy::math::optimization::rootFinding::bisectionMethod<T>(targetFunc, lowerBound, upperBound);

      }


      /**
       * 
       */
      template <typename T>
      class AbsoluteCEV : public BaseEuropeanOptionPricer<T> {

        public:

          /**
           * @brief Default constructor
           * @returns  Uninitialized AbsoluteCEV object
           */
          AbsoluteCEV() {  }


          /**
           * @brief Main constructor
           * @param r       The (continuously compounding) risk-free rate
           * @param K       The strike price 
           * @param vol     The volatility
           * @param isCall  Boolean flag telling if the option is a call or a put. Defaults to 'true'
           * @returns       Inititalized AbsoluteCEV object
           */
          AbsoluteCEV(T r, T K, T vol, bool isCall = true) {

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
           * @returns       Inititalized AbsoluteCEV object
           */
          AbsoluteCEV(T Vt, T St, T tau, T r, T K, bool isCall = true) {

            this->_r = r;
            this->_K = K;
            this->_isCall = isCall;
            this->_vol = __AbsoluteCEVImpliedVol<T>(Vt, St, tau, this->_r, this->_K, this->_isCall, (T)1e-6, (T)10.);

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
              return __AbsoluteCEVPrice<T>(St, tau, r, this->_K, vol, this->_isCall);
            }

            if ( r != (T)-1. ) {
              return __AbsoluteCEVPrice<T>(St, tau, r, this->_K, this->_vol, this->_isCall);
            }

            if ( vol != (T)-1. ) {
              return __AbsoluteCEVPrice<T>(St, tau, this->_r, this->_K, vol, this->_isCall);
            }

            return __AbsoluteCEVPrice<T>(St, tau, this->_r, this->_K, this->_vol, this->_isCall);

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

            return __AbsoluteCEVImpliedVol<T>(Vt, St, tau, this->_r, this->_K, this->_isCall, lowerBound, upperBound);

          }

      };


    }

  }

}


#endif