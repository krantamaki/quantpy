/**
 * @file factorial.hpp
 * @author Kasper Rantamäki
 * @date 2026-01-12
 */

#ifndef FACTORIAL_HPP
#define FACTORIAL_HPP


#include <cmath>

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


      /**
       * @brief The general Pochhammer function
       * @details The Pochhammer function is defined by the rising factorial \f$ (z)_n = z(z + 1) ... (z + n - 1) \f$.
       * The function utilizes the extension for complex values \f$ (z)_a = \Gamma(z + a) / \Gamma(z) \f$ [1].
       * [1]: See e.g. https://www.maplesoft.com/support/help/Maple/view.aspx?path=pochhammer
       * @tparam T  The template type. Should be some floating point type
       * @param z   The point where the Pochhammer function is evaluated
       * @param b   The number of factors
       * @returns   The value of the Pochhammer function
       * 
       */
      template <typename T>
      T pochhammer(T z, T b) {

        if ( a == (T)0. ) {
          return (T)1.;
        } 

        return tgamma(z + a) / tgamma(z);

      }


    }
  
  }

}


#endif