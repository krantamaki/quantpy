/**
 * @file hypergeometric.hpp
 * @author Kasper Rantamäki
 * @date 2026-02-08
 */

#ifndef HYPERGEOMETRIC_HPP
#define HYPERGEOMETRIC_HPP


#ifndef MAX_K
  #define MAX_K 100
#endif


#include <cmath>

#include "factorial.hpp"


namespace quantpy {

  namespace math {

    namespace special {

      namespace hypergeometric {


        /**
         * @brief Confluent hypergeometric limit function 0F1
         * @details The confluent hypergeometric is defined by the infinite sum \f$ 0F1(;b,z) = \sum_{k=0}^{\infty} \frac{z^k}{(b)_k k!} \f$.
         * The sum doesn't have a formulaic expression. However, it can be assumed to converge quite rapidly.
         * 
         * The Bessel function can be represented via confluent hypergeometric limit function as shown in Example 3.3.5 in [1].
         * [1]: M. Petkovšek H. Wilf, D. Zeilberger, “A = B,” A K Peters Ltd, ISBN 1-56881-063-6, 1996
         * @tparam T  The template type. Should be some floating point type
         * @param z      The point where the sum is evaluated
         * @param b      The sum parameter
         * @param max_k  The termination point for the sum. Defaults to 100
         * @returns      The value of the confluent hypergeometric limit function
         */
        template <typename T>
        T hyp0F1(T z, T b, int max_k = MAX_K) {

          T sum = (T)0.;

          for (int k = 0; k <= max_k; k++) {
            sum += pow(z, (T)k) / (pochhammer(b, (T)k) * factorial(k))
          }

          return sum;

        }


      }

    }
  
  }

}


#endif