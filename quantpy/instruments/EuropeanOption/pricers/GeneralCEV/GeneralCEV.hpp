/**
 * @file CEV.hpp
 * @details Constant elasticity of volatility (CEV) family of stochastic processes was first considered
 * by Cox in 1975 [1]. Beckers [2] provided closed form solutions for the general CEV process 
 * 
 * \f$ dS_t = \mu(t) S_t dt + \sigma(t) S_t^{\alpha / 2} dW_t , \f$
 * 
 * where \f$ \mu(t) \f$ is the time-dependent mean, \f$ \sigma(t) \f$ is the time-dependent volatility, 
 * \f$ \alpha \f$ is the elasticity factor and \f$ dW_t \f$ is the Brownian motion.
 * 
 * The pricing formula for general CEV process contains an infinite sum, but according to [2] the sum
 * converges quite quickly making the formula efficient to calculate.
 * 
 * [1]: J. Cox. "Notes on Option Pricing I: Constant Elasticity of Variance Diffusions." 
 * Working Paper Stanford University, 1975.
 * [2]: S. Beckers, “The constant elasticity of variance model and its implications for option pricing,”
 * The Journal of Finance, vol. 35, no. 3, pp. 661–673, 1980.
 * @author Kasper Rantamäki
 * @date 2026-01-20
 */

#ifndef GENERAL_CEV_HPP
#define GENERAL_CEV_HPP


#include <cmath>
#include <omp.h>

#include "../../../../cpp/logging.hpp"
#include "../../../../math/probability/gamma.hpp"
#include "../../../../math/optimization/rootFinding.hpp"
#include "../BaseEuropeanOptionPricer.hpp"


namespace quantpy {

  namespace instruments {

    namespace EuropeanOption {


      /**
       * @brief The complement of the cumulative gamma density function
       * @details Beckers [1] defines the general CEV formula using a function \f$ G \f$, which he calls the 
       * complimentary gamma distribution. It is assumed that this means \f$ 1 - F(x, \alpha, \lambda) \f$, where
       * \f$ F \f$ is the cumulative density function for a gamma distribution.
       * [1]: S. Beckers, “The constant elasticity of variance model and its implications for option pricing,”
       * The Journal of Finance, vol. 35, no. 3, pp. 661–673, 1980.
       * @tparam T     The template type. Should be some floating point type
       * @param shape  The scale parameter for the gamma distribution
       * @param rate   The rate parameter for the gamma distribution. Note that the rate parameter
       * is the inverse of the scale parameter, which could also be used to define the distribution
       * @param x      The point for which the density is evaluated
       * @returns      The complimentary gamma distribution evaluated at point 'x'
       */
      template <typename T>
      T __complimentaryGammaDist(T shape, T rate, T x) {

        return (T)1. - quantpy::math::probability::gamma::cdf(shape, rate, x);

      }


      /**
       * @brief The helper variable \f$ k \f$ used in the CEV formula
       * @tparam T     The template type. Should be some floating point type
       * @param tau    The time to maturity in years
       * @param r      The (continuously compounding) risk-free rate
       * @param vol    The volatility
       * @param alpha  The elasticity factor
       * @returns      The value of the helper variable 'k'
       */
      template <typename T>
      T __GeneralCEVk(T tau, T r, T vol, T alpha) {

        return ((T)2. * r) / (vol * vol * ((T)2. - alpha) * (exp(r * ((T)2. - alpha) * tau) - 1));

      }


      /**
       * @brief The helper variable \f$ x \f$ used in the CEV formula
       * @tparam T     The template type. Should be some floating point type
       * @param St     The market price of the undertlying
       * @param tau    The time to maturity in years
       * @param r      The (continuously compounding) risk-free rate
       * @param vol    The volatility
       * @param alpha  The elasticity factor
       * @returns      The value of the helper variable 'x'
       */
      template <typename T>
      T __GeneralCEVx(T St, T tau, T r, T vol, T alpha) {

        return __GeneralCEVk<T>(tau, r, vol, alpha) * pow(St, (T)2. - alpha) * exp(r * ((T)2. - alpha) * tau);

      }


      /**
       * @brief General CEV pricing function. To be called by the 'GeneralCEV' class
       * @tparam T      The template type. Should be some floating point type
       * @param St      The market price of the underlying
       * @param tau     The time to maturity in years
       * @param r       The (continuously compounding) risk-free rate
       * @param K       The strike price
       * @param vol     The volatility 
       * @param alpha   The elasticity factor
       * @param isCall  Boolean flag telling if the option is a call or a put. Defaults to 'true'
       * @param n       The upper bound for the infinite sum. Defaults to 10000 (should be enough according to Beckers (1980))
       * @returns       The price of a call option under the CEV model
       */
      template <typename T>
      T __GeneralCEVPrice(T St, T tau, T r, T K, T vol, T alpha, bool isCall = true, int n = 10000) {

        if ( n < 1 ) {
          ERROR("The upper bound for the sum needs to be positive! (", n, " < 1)");
        }

        T k = __GeneralCEVk<T>(tau, r, vol, alpha); 
        T x = __GeneralCEVx<T>(St, tau, r, vol, alpha);

        DEBUG("k = ", k, " | x = ", x)

        // The total value of the sums in the formula
        T firstSum  = (T)0.;
        T secondSum = (T)0.;

        #pragma omp parallel
        {

          // The sums accumulated within a single thread
          T firstThreadSum  = (T)0.;
          T secondThreadSum = (T)0.;

          #pragma omp for nowait
          for (int i = 0; i < n; i ++) {

            // Note that with the gamma distribution functions the rate parameter is assumed to be 1
            // This is in line with how Beckers (1980) defined the functions
            firstThreadSum  += quantpy::math::probability::gamma::pdf<T>((T)(i + 1), (T)1., x) * __complimentaryGammaDist<T>((T)(i + 1) + (T)1. / ((T)2. - alpha), (T)1., k * pow(K, (T)2. - alpha));
            secondThreadSum += quantpy::math::probability::gamma::pdf<T>((T)(i + 1) + (T)1. / ((T)2. - alpha), (T)1., x) * __complimentaryGammaDist<T>((T)(i + 1), (T)1., k * pow(K, (T)2. - alpha));

          }

          #pragma omp critical
          {
            // Accumulate the total sums without race conditions
            firstSum  += firstThreadSum;
            secondSum += secondThreadSum;
          }
        }

        T callPrice = St * firstSum - K * exp(-r * tau) * secondSum;

        if ( isCall ) {
          return callPrice;
        }

        return callPrice - St + K * exp(-r * tau);

      }


      /**
       * @brief Volatility implied by the general CEV formula. To be called by the 'GeneralCEV' class
       * @tparam T          The template type. Should support cmath math operations
       * @param Vt          The market price of the option
       * @param St          The market price of the underlying
       * @param tau         The time to maturity in years
       * @param r           The (continuously compounding) risk-free rate
       * @param K           The strike price
       * @param alpha       The elasticity factor
       * @param isCall      Boolean flag telling if the option is a call or a put. Defaults to 'true'
       * @param n           The upper bound for the infinite sum. Defaults to 10000 (should be enough according to Beckers (1980))
       * @param lowerBound  The lower bound for the volatility. Defaults to 1e-6
       * @param upperBound  The upper bound for the volatility. Defaults to 10
       * @returns           The implied volatility of the option
       */
      template <typename T>
      T __GeneralCEVImpliedVol(T Vt, T St, T tau, T r, T K, T alpha, bool isCall = true, int n = 10000, T lowerBound = (T)1e-6, T upperBound = (T)10.) {

        if ( n < 1 ) {
          ERROR("The upper bound for the sum needs to be positive! (", n, " < 1)");
        }

        auto targetFunc = [=](T vol) {
          return Vt - __GeneralCEVPrice(St, tau, r, K, vol, alpha, isCall, n);
        };

        return quantpy::math::optimization::rootFinding::bisectionMethod<T>(targetFunc, lowerBound, upperBound);

      }


      /**
       * @brief Simple pricer based on the analytical solution CEV diffusion processes
       * @tparam T  The template type. Should be some floating point number type
       */
      template <typename T>
      class GeneralCEV : public BaseEuropeanOptionPricer<T> {

        protected:
 
          T _alpha;  /**< The elasticity factor */
          int _n;    /**< The upper bound for the sum */

        public:

          /**
           * @brief Default constructor
           * @returns  Uninitialized GeneralCEV object
           */
          GeneralCEV() {  }


          /**
           * @brief Main constructor
           * @param r       The (continuously compounding) risk-free rate
           * @param K       The strike price
           * @param vol     The volatility 
           * @param alpha   The elasticity factor
           * @param isCall  Boolean flag telling if the option is a call or a put. Defaults to 'true'
           * @param n       The upper bound for the infinite sum. Defaults to 10000 (should be enough according to Beckers (1980))
           * @return        Initialized GeneralCEV object
           */
          GeneralCEV(T r, T K, T vol, T alpha, bool isCall = true, int n = 10000) {

            if ( n < 1 ) {
              ERROR("The upper bound for the sum needs to be positive! (", n, " < 1)");
            }

            this->_r = r;
            this->_K = K;
            this->_vol = vol;
            this->_isCall = isCall;
            this->_alpha = alpha;
            this->_n = n;

          }


          /**
           * @brief Market price constructor. Calculates the implied volatility for the given market price
           * @param Vt      The market price of the option
           * @param St      The market price of the underlying
           * @param tau     The time to maturity in years
           * @param r       The (continuously compounding) risk-free rate
           * @param K       The strike price 
           * @param alpha   The elasticity factor
           * @param isCall  Boolean flag telling if the option is a call or a put. Defaults to 'true'
           * @param n       The upper bound for the infinite sum. Defaults to 10000 (should be enough according to Beckers (1980))
           * @returns       Initialized GeneralCEV object
           */
          GeneralCEV(T Vt, T St, T tau, T r, T K, T alpha, bool isCall = true, int n = 10000) {

            this->_r = r;
            this->_K = K;
            this->_isCall = isCall;
            this->_alpha = alpha;
            this->_n = n;
            this->_vol = __GeneralCEVImpliedVol<T>(Vt, St, tau, this->_r, this->_K, this->_alpha, this->_isCall, this->_n, (T)1e-6, (T)10.);

          }


          /**
           * @brief Setter for the elasticity factor
           * @returns The elasticity factor
           */
          virtual T& alpha() { return this->_alpha; }
          

          /**
           * @brief Getter for the elasticity factor
           * @returns The elasticity factor
           */
          const T& alpha() const { return this->_alpha; }


          /**
           * @brief Setter for the upper bound of the infinite sum
           * @returns The upper bound of the infinite sum
           */
          virtual int& n() { return this->_n; }
          

          /**
           * @brief Getter for the upper bound of the infinite sum
           * @returns The upper bound of the infinite sum
           */
          const int& n() const { return this->_n; }


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
              return __GeneralCEVPrice<T>(St, tau, r, this->_K, vol, this->_alpha, this->_isCall, this->_n);
            }

            if ( r != (T)-1. ) {
              return __GeneralCEVPrice<T>(St, tau, r, this->_K, this->_vol, this->_alpha, this->_isCall, this->_n);
            }

            if ( vol != (T)-1. ) {
              return __GeneralCEVPrice<T>(St, tau, this->_r, this->_K, vol, this->_alpha, this->_isCall, this->_n);
            }

            return __GeneralCEVPrice<T>(St, tau, this->_r, this->_K, this->_vol, this->_alpha, this->_isCall, this->_n);

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

            return __GeneralCEVImpliedVol<T>(Vt, St, tau, this->_r, this->_K, this->_alpha, this->_isCall, this->_n, lowerBound, upperBound);

          }

      };


    }

  }

}


#endif