/**
 * @file BaseStochasticProcess.hpp
 * @brief Base class for stochastic processes
 * @author Kasper Rantamäki
 * @date 2026-01-17
 */

#ifndef BASE_STOCHASTIC_PROCESS
#define BASE_STOCHASTIC_PROCESS


#include <functional>


namespace quantpy {

  namespace math {

    namespace stochasticProcesses {


      /**
       * @brief Virtual base class for stochastic processes
       * @tparam T  The template type. Should be some floating point number type
       */
      template <typename T>
      class BaseStochasticProcess {

        protected:

          std::function<T (T)> _rts;    /**< The term-structure for the risk-free rate */
          std::function<T (T)> _qts;    /**< The term-structure for the dividend yield */
          std::function<T (T)> _volts;  /**< The term-structure for the volatility */

        public:

          /**
           * @brief Calls the stored function for term-structure of risk-free rates
           * @returns The risk-free rate for a given maturity
           */
          virtual T rts(T tau) const { return this->_rts(tau); }


          /**
           * @brief Calls the stored function for term-structure of volatilities
           * @returns The volatility for a given maturity
           */
          virtual T volts(T tau) const { return this->_volts(tau); }


          /**
           * @brief Calls the stored function for term-structure of dividend yields
           * @returns The dividend yield for a given maturity
           */
          virtual T qts(T tau) const { return this->_qts(tau); }


          /**
           * @brief Pure virtual function for sampling a path starting at value 'v0' and going forward 'tau' amount of time in 'n' steps
           * @param v0   The initial value for the process
           * @param tau  The time to maturity from the the initial point
           * @param n    The number of steps taken
           * @returns    A std::vector object containing the path of the process. Note that the path consists of n + 1 values
           */
          virtual std::vector<T> sample(T v0, T tau, int n) const = 0;

      };


    }

  }
  
}


#endif