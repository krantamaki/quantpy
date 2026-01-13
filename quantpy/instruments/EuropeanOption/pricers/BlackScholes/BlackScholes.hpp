/**
 * @file BlackScholes.hpp
 * @brief Simple analytical Black-Scholes pricer for European options
 * @author Kasper Rantamäki
 * @date 2026-01-13
 */

#ifndef BLACK_SCHOLES_HPP
#define BLACK_SCHOLES_HPP


#include <cmath>
#include <tuple>

#include "../../../math/probability/normal.hpp"
#include "../../../math/optimization/rootFinding.hpp"


namespace quantpy {

  namespace instruments {

    namespace EuropeanOption {

      namespace BlackScholes {


        /**
         * @brief Black-Scholes helper function \f$ d_+ \f$
         * @tparam T   The template type. Should support cmath math operations
         * @param St   The value of the underlying
         * @param tau  The time to maturity in years
         * @param r    The (continuously compounding) risk-free rate
         * @param K    The strike price
         * @param vol  The volatility
         * @returns    The value \f$ d_+ \f$
         */
        template <typename T>
        T dPlus(T St, T tau, T r, T K, T vol) {

          return 1 / (vol * sqrt(tau)) * (log(St / K) + (r + vol * vol / 2) * tau);

        }


        /**
         * @brief Black-Scholes helper function \f$ d_- \f$
         * @tparam T   The template type. Should support cmath math operations
         * @param St   The value of the underlying
         * @param tau  The time to maturity in years
         * @param r    The (continuously compounding) risk-free rate
         * @param K    The strike price
         * @param vol  The volatility
         * @returns    The value \f$ d_- \f$
         */
        template <typename T>
        T dMinus(T St, T tau, T r, T K, T vol) {

          return dPlus(St, tau, r, K, vol) - vol * sqrt(tau);

        }


        /**
         * @brief Black-Scholes option price
         * @tparam T      The template type. Should support cmath math operations
         * @param St      The value of the underlying
         * @param tau     The time to maturity in years
         * @param r       The (continuously compounding) risk-free rate
         * @param K       The strike price
         * @param vol     The volatility
         * @param isCall  Boolean flag telling if the option is a call or a put. Defaults to 'true'
         * @returns       The price of the option
         */
        template <typename T>
        T price(T St, T tau, T r, T K, T vol, bool isCall = true) {

          if ( isCall ) {
            return quantpy::math::probability::normal::cdf<T>(0., 1., dPlus(St, tau, r, K, vol)) * St -
                   quantpy::math::probability::normal::cdf<T>(0., 1., dMinus(St, tau, r, K, vol)) * K * exp(-r * tau);
          }

          return quantpy::math::probability::normal::cdf<T>(0., 1., -dMinus(St, tau, r, K, vol)) * K * exp(-r * tau) -
                 quantpy::math::probability::normal::cdf<T>(0., 1., -dPlus(St, tau, r, K, vol)) * St;

        }


        /**
         * @brief The sensitivity of the Black-Scholes option price to the value of the underlying
         * @tparam T      The template type. Should support cmath math operations
         * @param St      The value of the underlying
         * @param tau     The time to maturity in years
         * @param r       The (continuously compounding) risk-free rate
         * @param K       The strike price
         * @param vol     The volatility
         * @param isCall  Boolean flag telling if the option is a call or a put. Defaults to 'true'
         * @returns       The delta of the option
         */
        template <typename T>
        T delta(T St, T tau, T r, T K, T vol, bool isCall = true) {

          if ( isCall ) {
            return quantpy::math::probability::normal::cdf<T>(0., 1., dPlus(St, tau, r, K, vol));
          }
          
          return quantpy::math::probability::normal::cdf<T>(0., 1., dPlus(St, tau, r, K, vol)) - 1;

        }


        /**
         * @brief The sensitivity of the Black-Scholes option price to the option delta
         * @tparam T      The template type. Should support cmath math operations
         * @param St      The value of the underlying
         * @param tau     The time to maturity in years
         * @param r       The (continuously compounding) risk-free rate
         * @param K       The strike price
         * @param vol     The volatility
         * @param isCall  Boolean flag telling if the option is a call or a put. Defaults to 'true'
         * @returns       The gamma of the option
         */
        template <typename T>
        T gamma(T St, T tau, T r, T K, T vol, bool isCall = true) {

          // Independent of option type
          return quantpy::math::probability::normal::pdf<T>(0., 1., dPlus(St, tau, r, K, vol)) / (St * vol * sqrt(tau));

        }


        /**
         * @brief The sensitivity of the Black-Scholes option price to volatility
         * @tparam T      The template type. Should support cmath math operations
         * @param St      The value of the underlying
         * @param tau     The time to maturity in years
         * @param r       The (continuously compounding) risk-free rate
         * @param K       The strike price
         * @param vol     The volatility
         * @param isCall  Boolean flag telling if the option is a call or a put. Defaults to 'true'
         * @returns       The vega of the option
         */
        template <typename T>
        T vega(T St, T tau, T r, T K, T vol, bool isCall = true) {

          // Independent of option type
          return quantpy::math::probability::normal::pdf<T>(0., 1., dPlus(St, tau, r, K, vol)) * St * sqrt(tau);

        }


        /**
         * @brief The sensitivity of the Black-Scholes option price to the risk-free rate
         * @tparam T   The template type. Should support cmath math operations
         * @param St   The value of the underlying
         * @param tau  The time to maturity in years
         * @param r    The (continuously compounding) risk-free rate
         * @param K    The strike price
         * @param vol  The volatility
         * @param isCall  Boolean flag telling if the option is a call or a put. Defaults to 'true'
         * @returns    The rho of the option
         */
        template <typename T>
        T rho(T St, T tau, T r, T K, T vol, bool isCall = true) {

          if ( isCall ) {
            return quantpy::math::probability::normal::cdf<T>(0., 1., dMinus(St, tau, r, K, vol)) * K * tau * exp(-r * tau);
          }

          return -quantpy::math::probability::normal::cdf<T>(0., 1., -dMinus(St, tau, r, K, vol)) * K * tau * exp(-r * tau);

        }


        /**
         * @brief The sensitivity of the Black-Scholes option price to time
         * @tparam T      The template type. Should support cmath math operations
         * @param St      The value of the underlying
         * @param tau     The time to maturity in years
         * @param r       The (continuously compounding) risk-free rate
         * @param K       The strike price
         * @param vol     The volatility
         * @param isCall  Boolean flag telling if the option is a call or a put. Defaults to 'true'
         * @returns       The theta of the option
         */
        template <typename T>
        T theta(T St, T tau, T r, T K, T vol, bool isCall = true) {

          if ( isCall ) {
            return -(St * vol * quantpy::math::probability::normal::pdf<T>(0., 1., dPlus(St, tau, r, K, vol))) / (2 * sqrt(tau)) -
                   r * K * exp(-r * tau) * quantpy::math::probability::normal::cdf<T>(0., 1., dMinus(St, tau, r, K, vol));
          }

          return -(St * vol * quantpy::math::probability::normal::pdf<T>(0., 1., dPlus(St, tau, r, K, vol))) / (2 * sqrt(tau)) +
                 r * K * exp(-r * tau) * quantpy::math::probability::normal::cdf<T>(0., 1., -dMinus(St, tau, r, K, vol));

        }


        /**
         * @brief The volatility implied by the market price \f$ V_t \f$
         * @tparam T      The template type. Should support cmath math operations
         * @param Vt      The market price of the option
         * @param St      The value of the underlying
         * @param tau     The time to maturity in years
         * @param r       The (continuously compounding) risk-free rate
         * @param K       The strike price
         * @param isCall  Boolean flag telling if the option is a call or a put. Defaults to 'true'
         * @param bounds  The bounds of the implied volatility search region. Defaults to {1e-6, 10}
         * @returns       The market implied volatility
         */
        template <typename T>
        T impliedVol(T Vt, T St, T tau, T r, T K, bool isCall = true, std::tuple<T, T> bounds = {(T)1e-6, (T)10.}) {

          auto targetFunc = [](T vol) {
            return Vt - price(St, tau, r, K, vol, isCall);
          }

          return quantpy::math::optimization::rootFinding::bisectionMethod(targetFunc, bounds[0], bounds[1]);

        }


      }

    }

  }
  
} 


#endif