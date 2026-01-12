/**
 * @file factorial.hpp
 * @author Kasper Rantamäki
 * @date 2026-01-12
 */

#ifndef FACTORIAL_HPP
#define FACTORIAL_HPP


#include "../../cpp/logging.hpp"


namespace quantpy {

  namespace math {

    namespace special {

      /**
       * @brief Factorial function
       * @tparam T    The template type. Should be some integer type
       * @param n     The non-negative integer for which the factorial is evaluated
       * @returns     The factorial of argument 'n'
       */
      template <typename T>
      T factorial(T n) {

        if ( n < 0 ) {
          ERROR("The integer must be non-negative! (", n, " < 0)");
        }

        T fact = n;

        for (T i = n - 1; i > 0; i--) {
          fact *= i;
        }

        return fact;

      }


      /**
       * @brief Double factorial function
       * @details The double factorial is the product of integers with the same parity as 'n' up to 'n'
       * @tparam T    The template type. Should be some integer type
       * @param n     The non-negative integer for which the double factorial is evaluated
       * @returns     The double factorial of argument 'n'
       */
      template <typename T>
      T doubleFactorial(T n) {

        if ( n < 0 ) {
          ERROR("The integer must be non-negative! (", n, " < 0)");
        }

        T fact = n;

        for (T i = n - 2; i > 0; i -= 2) {
          fact *= i;
        }

        return fact;

      }


    }
  
  }

}


#endif