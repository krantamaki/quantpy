/**
 * @file GeometricBrownianMotion.hpp
 * @brief Implementation of a general geometric Brownian motion
 * @author Kasper Rantamäki
 * @date 2026-01-17
 */

#ifndef GEOMETRIC_BROWNIAN_MOTION_HPP
#define GEOMETRIC_BROWNIAN_MOTION_HPP


#include <functional>
#include <vector>
#include <cmath>

#include "../probability/normal.hpp"
#include "../../cpp/logging.hpp"
#include "BaseStochasticProcess.hpp"


namespace quantpy {

  namespace math {

    namespace stochasticProcesses {


      /**
       * @brief Implementation of a general geometric Brownian motion
       * @details Based on the Chapter 21 in Hull's book [1].
       * [1]: J. Hull. "Options, Futures and Other Derivatives", Pearson Education Limited, 2018
       */
      template <typename T>
      class GeometricBrownianMotion : public BaseStochasticProcess<T> {

        protected:

          std::function<T, (T)> _rts;    /**< The term-structure for the risk-free rate */
          std::function<T, (T)> _qts;    /**< The term-structure for the dividend yield */
          std::function<T, (T)> _volts;  /**< The term-structure for the volatility */

        public:

          /**
           * @brief Default constructor
           * @details If a constant term-structure is desired, a simple lambda function that returns a constant
           * value can be passed to the constructor
           * @param rts    The term-structure for the risk-free rate
           * @param qts    The term-structure for the dividend yield
           * @param volts  The term-structure for the volatility
           */
          GeometricBrownianMotion(std::function<T, (T)> rts, std::function<T, (T)> qts, std::function<T, (T)> volts) {

            this->_rts   = rts;
            this->_qts   = qts;
            this->_volts = volts;

          }


          /**
           * @brief Samples a path starting at value 'v0' and going forward 'tau' amount of time in 'n' steps
           * @param v0   The initial value for the process
           * @param tau  The time to maturity from the the initial point
           * @param n    The number of steps taken
           * @returns    A std::vector object containing the path of the process. Note that the path consists of n + 1 values
           */
          std::vector<T> sample(T v0, T tau, int n) const override {

            if ( n < 1 ) {
              ERROR("The number of samples must be positive! (", n, " < 1)");
            }

            std::vector<T> normalSamples = quantpy::math::probability::normal::sample(0, 1, n);
            std::vector<T> path;
            path.reserve(n + 1);

            path.push_back(v0);
            
            T dt = tau / (T)n;
            T St = v0;
            T sqrtdt = sqrt(dt);

            for (int i = 1; i <= n; i++) {
              T tau_i = tau - i * dt;
              T vol_i = this->_volts(tau_i)
              // Note that the mean is assumed to be the risk-free rate minus the dividend yield
              St = St * exp((this->_rts(tau_i) - this->_qts(tau_i) - vol_i * vol_i / (T)2.) * dt + vol_i * normalSamples[i - 1] * sqrtdt);
              path.push_back(St);
            }

            return path;

          }
        
      };


    }

  }

}


#endif