/**
 * @file trapezoidal.hpp
 * @brief Implementation of numerical approximation of definite integrals using the Trapezoidal rule
 * @author Kasper Rantamäki
 * @date 2026-01-18
 */

#ifndef TRAPEZOIDAL_HPP
#define TRAPEZOIDAL_HPP


#include <functional>
#include <omp.h>

#include "../../cpp/logging.hpp"


namespace quantpy {

  namespace math {

    namespace integration {


      /**
       * @brief Calculates a numerical estimate to definite integrals by the Trapezoidal rule
       * @details Based on the implementation given in Chapter 4.2 in Numerical Recipes [1]
       * The Trapezoidal rule has an error of order \f$ O(\frac{(b - a)^3 f''}{n^2}) \f$
       * [1]: W. Press et. al., "Numerical Recipes: The Art of Scientific Computing", Cambridge University Press, 2007
       * @tparam T    The template type. Should be some floating point type
       * @param func  The function which integral is to be evaluated
       * @param a     The start point of the integral
       * @param b     The end point of the integral
       * @param n     The number of interior points used
       */
      template <typename T>
      T trapezoidal(std::function<T (T)> func, T a, T b, int n) {

        if ( a >= b ) {
          ERROR("The start point needs to be smaller than the end point! (", a, " >= ", b, ")");
        }

        if ( n < 1 ) {
          ERROR("The number of interior points needs to be positive! (", n, " < 1)");
        }

        // The total integral sum
        T intSum = (T)0.5 * func(a) + (T)0.5 * func(b);
        
        // The finite difference is discerned from the start and end point and the number of interior points
        T h = (b - a) / (T)n;

        #pragma omp parallel
        {
          // The integral sum within a single thread
          T intThreadSum = (T)0.;

          #pragma omp for nowait 
          for (int i = 1; i < n - 1; i++) {
            intThreadSum += func(a + i * h);
          }

          #pragma omp critical
          {
            // Accumulate the thread sums together
            intSum += intThreadSum;
          }
        }

        return h * intSum;

      }


    }
    
  } 

}

#endif