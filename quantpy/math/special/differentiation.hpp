/**
 * @file differentiation.hpp
 * @brief Collection of finite difference functions for numerically evaluating the derivative
 * at a given point
 * @author Kasper Rantamäki
 * @date 2026-01-16
 */

#ifndef DIFFERENTIATION_HPP
#define DIFFERENTIATION_HPP


#ifndef DEFAULT_DIFF
  #define DEFAULT_DIFF 1e-6
#endif


#include <functional>

#include "binomial.hpp"
#include "../../cpp/logging.hpp"


namespace quantpy {

  namespace math {

    namespace special {

      namespace differentiation {


        /**
         * @brief The first-order forward difference evaluated at point 'x'. Has an error of order \f$ o(h) \f$.
         * @tparam T    The template type. Should support basic math operation like addition and division
         * @param func  The function of which derivative is approximated
         * @param x     The point at which the derivative is evaluated
         * @param h     The finite difference. Defaults to 1e-6
         * @returns     The approximated first-order derivative at the given point
         */
        template <typename T>
        T firstForwardDiff(std::function<T (T)> func, T x, T h = DEFAULT_DIFF) {

          return (func(x + h) - func(x)) / h;

        }


        /**
         * @brief The first-order backward difference evaluated at point 'x'. Has an error of order \f$ o(h) \f$.
         * @tparam T    The template type. Should support basic math operation like addition and division
         * @param func  The function of which derivative is approximated
         * @param x     The point at which the derivative is evaluated
         * @param h     The finite difference. Defaults to 1e-6
         * @returns     The approximated first-order derivative at the given point
         */
        template <typename T>
        T firstBackwardDiff(std::function<T (T)> func, T x, T h = DEFAULT_DIFF) {

          return (func(x) - func(x - h)) / h;

        }


        /**
         * @brief The first-order central difference evaluated at point 'x'. Has an error of order \f$ o(h^2) \f$.
         * @tparam T    The template type. Should support basic math operation like addition and division
         * @param func  The function of which derivative is approximated
         * @param x     The point at which the derivative is evaluated
         * @param h     The finite difference. Defaults to 1e-6
         * @returns     The approximated first-order derivative at the given point
         */
        template <typename T>
        T firstCentralDiff(std::function<T (T)> func, T x, T h = DEFAULT_DIFF) {

          DEBUG(func(x + h / (T)2.), " ", func(x - h / (T)2.));
          return (func(x + h / (T)2.) - func(x - h / (T)2.)) / h;

        }


        /**
         * @brief The second-order forward difference evaluated at point 'x'
         * @tparam T    The template type. Should support basic math operation like addition and division
         * @param func  The function of which derivative is approximated
         * @param x     The point at which the derivative is evaluated
         * @param h     The finite difference. Defaults to 1e-6
         * @returns     The approximated second-order derivative at the given point
         */
        template <typename T>
        T secondForwardDiff(std::function<T (T)> func, T x, T h = DEFAULT_DIFF) {

          return (func(x + (T)2. * h) - (T)2. * func(x + h) + func(x)) / (h * h);

        }


        /**
         * @brief The second-order backward difference evaluated at point 'x'
         * @tparam T    The template type. Should support basic math operation like addition and division
         * @param func  The function of which derivative is approximated
         * @param x     The point at which the derivative is evaluated
         * @param h     The finite difference. Defaults to 1e-6
         * @returns     The approximated second-order derivative at the given point
         */
        template <typename T>
        T secondBackwardDiff(std::function<T (T)> func, T x, T h = DEFAULT_DIFF) {

          return (func(x) + func(x - (T)2. * h) - (T)2. * func(x - h)) / (h * h);

        }


        /**
         * @brief The second-order central difference evaluated at point 'x'
         * @tparam T    The template type. Should support basic math operation like addition and division
         * @param func  The function of which derivative is approximated
         * @param x     The point at which the derivative is evaluated
         * @param h     The finite difference. Defaults to 1e-6
         * @returns     The approximated second-order derivative at the given point
         */
        template <typename T>
        T secondCentralDiff(std::function<T (T)> func, T x, T h = DEFAULT_DIFF) {

          return (func(x + h) - (T)2. * func(x) + func(x - h)) / (h * h);

        }


        /**
         * @brief The nth-order forward difference evaluated at point 'x'
         * @tparam T    The template type. Should support basic math operation like addition and division
         * @param func  The function of which derivative is approximated
         * @param n     The order of the derivative
         * @param x     The point at which the derivative is evaluated
         * @param h     The finite difference. Defaults to 1e-6
         * @returns     The approximated nth-order derivative at the given point
         */
        template <typename T>
        T nthForwardDiff(std::function<T (T)> func, int n, T x, T h = DEFAULT_DIFF) {

          if ( n < 1 ) {
            ERROR("The order of the derivative has to be at least 1! (", n, " < 1)");
          }

          T sum = 0;

          for (int i = 0; i <= n; i++) {
            sum += ((n - i % 2 == 0) ? (T)1. : (T)-1.) * (T)binomial<int>(n, i) * func(x + (T)i * h);
          }

          return sum;

        }


        /**
         * @brief The nth-order backward difference evaluated at point 'x'
         * @tparam T    The template type. Should support basic math operation like addition and division
         * @param func  The function of which derivative is approximated
         * @param n     The order of the derivative
         * @param x     The point at which the derivative is evaluated
         * @param h     The finite difference. Defaults to 1e-6
         * @returns     The approximated nth-order derivative at the given point
         */
        template <typename T>
        T nthBackwardDiff(std::function<T (T)> func, int n, T x, T h = DEFAULT_DIFF) {

          if ( n < 1 ) {
            ERROR("The order of the derivative has to be at least 1! (", n, " < 1)");
          }

          T sum = 0;

          for (int i = 0; i <= n; i++) {
            sum += ((i % 2 == 0) ? (T)1. : (T)-1.) * (T)binomial<int>(n, i) * func(x - (T)i * h);
          }

          return sum;

        }


        /**
         * @brief The nth-order central difference evaluated at point 'x'
         * @tparam T    The template type. Should support basic math operation like addition and division
         * @param func  The function of which derivative is approximated
         * @param n     The order of the derivative
         * @param x     The point at which the derivative is evaluated
         * @param h     The finite difference. Defaults to 1e-6
         * @returns     The approximated nth-order derivative at the given point
         */
        template <typename T>
        T nthCentralDiff(std::function<T (T)> func, int n, T x, T h = DEFAULT_DIFF) {

          if ( n < 1 ) {
            ERROR("The order of the derivative has to be at least 1! (", n, " < 1)");
          }

          T sum = 0;

          for (int i = 0; i <= n; i++) {
            sum += ((i % 2 == 0) ? (T)1. : (T)-1.) * (T)binomial<int>(n, i) * func(x + ((T)n / (T)2. - (T)i) * h);
          }

          return sum;

        }


      }

    }

  }

}


#endif