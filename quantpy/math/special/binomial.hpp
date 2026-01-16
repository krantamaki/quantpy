/**
 * @file binomial.hpp
 * @author Kasper Rantamäki
 * @date 2026-01-16
 */

#ifndef BINOMIAL_HPP
#define BINOMIAL_HPP


namespace quantpy {

  namespace math {

    namespace special {


      /**
       * @brief Function for computing binomial coefficients
       * @tparam T  The template type. Should be some integer type
       * @param n   The size of the full set
       * @param k   The size of the subset chosen
       * @returns   The binomial coefficient
       */
      template <typename T>
      T binomial(T n, T k) {

        T prod = (T)1.;

        for (T i = 1; i < (k < n - k ? k : n - k); i++) {
          prod *= (n + 1 - i) / i;
        }

        return prod;

      }


    }
  
  }

}


#endif