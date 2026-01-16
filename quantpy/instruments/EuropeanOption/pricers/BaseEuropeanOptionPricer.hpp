/**
 * @file BaseEuropeanOptionPricer.hpp
 * @brief Base class for European option pricers
 * @author Kasper Rantamäki
 * @date 2026-01-16
 */

#ifndef BASE_EUROPEAN_OPTION_PRICER_HPP
#define BASE_EUROPEAN_OPTION_PRICER_HPP


#include "../../../math/special/differentiation.hpp"


namespace quantpy {

  namespace instruments {

    namespace EuropeanOption {


      template<typename T>
      class BaseEuropeanOptionPricer {

        protected:

          T _r;          /**< The risk-free rate */
          T _vol;        /**< The volatility */
          T _K;          /**< The strike price */
          bool _isCall;  /**< Boolean flag telling the option type */

        public:

          /**
           * @brief Virtual function for pricing the option. Needs to be overriden
           * @param St   The price of the underlying
           * @param tau  The time to maturity in years
           * @param r    The (continuously compounding) risk-free rate. Optional and defaults
           * to the one used to initialize the object
           * @param vol  The volatility. Optional and defaults to the one used to initialize the object
           * @returns    The price of the option
           */
          virtual T operator() (T St, T tau, T r = (T)-1., T vol = (T)-1.) const {

            return (T)0.;

          };


          /**
           * @brief Setter for the risk-free rate
           * @returns The risk-free rate
           */
          virtual T& r() { return this->_r; }
          

          /**
           * @brief Getter for the risk-free rate
           * @returns The risk-free rate
           */
          const T& r() const { return this->_r; }


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
           * @brief Virtual function for calculating the option delta via finite differences
           * @param St   The price of the underlying
           * @param tau  The time to maturity in years
           * @returns    The delta of the option
           */
          virtual const T delta(T St, T tau) const {

            auto deltaFunc = [=, this](T S) {
              return this->operator() (S, tau);
            };

            return quantpy::math::special::differentiation::firstCentralDiff<T>(deltaFunc, St);

          }


          /**
           * @brief Virtual function for calculating the option gamma via finite differences
           * @param St   The price of the underlying
           * @param tau  The time to maturity in years
           * @returns    The gamma of the option
           */
          virtual const T gamma(T St, T tau) const {

            auto gammaFunc = [=, this](T S) {
              return this->operator() (S, tau);
            };

            return quantpy::math::special::differentiation::secondCentralDiff<T>(gammaFunc, St);

          }


          /**
           * @brief Virtual function for calculating the option vega via finite differences
           * @param St   The price of the underlying
           * @param tau  The time to maturity in years
           * @returns    The vega of the option
           */
          virtual const T vega(T St, T tau) const {

            auto vegaFunc = [=, this](T vol) {
              return this->operator() (St, tau, this->r(), vol);
            };

            return quantpy::math::special::differentiation::firstCentralDiff<T>(vegaFunc, this->_vol);

          }


          /**
           * @brief Virtual function for calculating the option rho via finite differences
           * @param St   The price of the underlying
           * @param tau  The time to maturity in years
           * @returns    The rho of the option
           */
          virtual const T rho(T St, T tau) const {

            auto rhoFunc = [=, this](T r) {
              return this->operator() (St, tau, r);
            };

            return quantpy::math::special::differentiation::firstCentralDiff<T>(rhoFunc, this->_r);

          }


          /**
           * @brief Virtual function for calculating the option theta via finite differences
           * @param St   The price of the underlying
           * @param tau  The time to maturity in years
           * @returns    The theta of the option
           */
          virtual const T theta(T St, T tau) const {

            auto thetaFunc = [=, this](T t) {
              return this->operator() (St, t);
            };

            return quantpy::math::special::differentiation::firstCentralDiff<T>(thetaFunc, tau);

          }

      };


    }

  }

}


#endif