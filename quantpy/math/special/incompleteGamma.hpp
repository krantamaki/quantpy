/**
 * @file incompleteGamma.hpp
 * @brief Contains implementations of upper and lower incomplete gamma functions
 * @author Kasper Rantamäki
 * @date 2026-01-19
 */

#ifndef INCOMPLETE_GAMMA_HPP
#define INCOMPLETE_GAMMA_HPP


#include <cmath>

#include "../integration/simpson.hpp"


namespace quantpy {

  namespace math {

    namespace special {

      
      /**
       * @brief Calculates a numerical approximation for the (real) lower incomplete gamma function
       * @tparam T  The template type. Should be some floating point type
       * @param s   The exponential parameter in the integral. Needs to be positive
       * @param x   The limit of the incomplete integral
       * @param n   The number of interior points used in the integral. Defaults to 1000
       * @returns   The approximate lower incomplete gamma
       */
      template <typename T>
      T lowerIncompleteGamma(T s, T x, int n = 1000) {

        if ( n < 1 ) {
          ERROR("The number of interior points needs to be positive! (", n, " < 1)");
        }

        if ( s < (T)1. ) {
          ERROR("The exponential parameter needs to be positive! (", s, " < 0)");
        }

        auto func = [=](T t) { return pow(t, s - (T)1.) * exp(-t); };

        return math::integration::simpson<T>(func, (T)0., x, n);

      }


      /**
       * @brief Calculates a numerical approximation for the (real) upper incomplete gamma function
       * @tparam T  The template type. Should be some floating point type
       * @param s   The exponential parameter in the integral. Needs to be positive
       * @param x   The limit of the incomplete integral
       * @param n   The number of interior points used in the integral. Defaults to 1000
       * @returns   The approximate upper incomplete gamma
       */
      template <typename T>
      T upperIncompleteGamma(T s, T x, int n = 1000) {

        if ( n < 1 ) {
          ERROR("The number of interior points needs to be positive! (", n, " < 1)");
        }

        if ( s < (T)1. ) {
          ERROR("The exponential parameter needs to be positive! (", s, " < 0)");
        }

        return tgamma(s) - lowerIncompleteGamma(s, x, n);

      }


    }

  }
  
} 





#endif