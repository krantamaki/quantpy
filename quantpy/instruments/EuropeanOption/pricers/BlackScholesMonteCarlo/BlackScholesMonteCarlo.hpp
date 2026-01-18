/**
 * @file BlackScholesMonteCarlo.hpp
 * @brief Simple pricer based on Monte Carlo simulation under the Black-Scholes assumptions
 * @author Kasper Rantamäki
 * @date 2026-01-17
 */

#ifndef BLACK_SCHOLES_MONTE_CARLO_HPP
#define BLACK_SCHOLES_MONTE_CARLO_HPP


#include <cmath>
#include <vector>
#include <functional>

#include "../../../../math/probability/normal.hpp"
#include "../../../../math/optimization/rootFinding.hpp"
#include "../../../../math/stochasticProcesses/GeometricBrownianMotion.hpp"
#include "../BaseEuropeanOptionPricer.hpp"


namespace quantpy {

  namespace instruments {

    namespace EuropeanOption {


      /**
       * @brief Simple pricer based on Monte Carlo simulation under the Black-Scholes assumptions
       * @tparam T  The template type. Should be some floating point number type
       */
      template <typename T>
      class BlackScholesMonteCarlo : public BaseEuropeanOptionPricer<T> {

        protected:

          quantpy::math::stochasticProcesses::GeometricBrownianMotion<T> _process;  /**< The stochastic process used in the simulation */
          int _nSteps;                                                              /**< The number of steps used in the simulation */
          int _nTrials;                                                             /**< The number of trials used in the simulation */

        public:

          /**
           * @brief Default constructor
           * @returns  Uninitialized BlackScholesMonteCarlo object
           */
          BlackScholesMonteCarlo() {  }


          /**
           * @brief Main constructor
           * @param process   The (continuously compounding) risk-free rate
           * @param nTrials   The number of trials used in the simulation
           * @param nSteps    The number of steps used in the simulation. Needs to be a positive integer.
           * Defaults to 1 as European options are path independent.
           * @param isCall    Boolean flag telling if the option is a call or a put. Defaults to 'true'
           * @returns         The pricer object
           */
          BlackScholesMonteCarlo(quantpy::math::stochasticProcesses::GeometricBrownianMotion<T> process, T K, int nTrials, int nSteps = 1, bool isCall = true) {

            if ( nSteps < 1 ) {
              ERROR("The number of samples must be be positive! (", nSteps, " < 1)");
            }

            if ( nTrials < 0 ) {
              ERROR("The number of trials must be positive! (", nTrials, " < 1)");
            }

            this->_process = process;
            this->_nTrials = nTrials;
            this->_nSteps  = nSteps;
            this->_isCall  = isCall;
            this->_K = K;

          }


          /**
           * @brief The pricing method
           * @param St   The market price of the underlying
           * @param tau  The time to maturity in years
           * @param r    The risk-free rate. Not considered in the pricing
           * @param vol  The volatility. Not considered in the pricing
           * @returns    The price of the option
           */
          T operator() (T St, T tau, T r = (T)-1., T vol = (T)-1.) const override {

            T payoffSum = (T)0.;

            for (int i = 0; i < this->_nTrials; i++) {
              std::vector<T> path = this->_process.sample(St, tau, this->_nSteps);

              T finalValue = path[this->_nSteps];

              if ( this->_isCall ) {
                payoffSum += finalValue > this->_K ? finalValue - this->_K : (T)0.;
              }
              else {
                payoffSum += finalValue < this->_K ? this->_K - finalValue : (T)0.;
              }
            }

            return payoffSum / (T)this->_nTrials * exp(-this->_process.rts(tau) * tau);

          }


      };


    }

  }
  
} 


#endif