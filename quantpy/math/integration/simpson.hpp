/**
 * @file simpson.hpp
 * @brief Implementation of numerical approximation of definite integrals using the Simson's rule
 * @author Kasper Rantamäki
 * @date 2026-01-19
 */

#ifndef SIMPSON_HPP
#define SIMPSON_HPP


#include <functional>
#include <omp.h>

#include "../../cpp/logging.hpp"


namespace quantpy {

  namespace math {

    namespace integration {


      /**
       * @brief Calculates a numerical estimate to definite integrals by the Simpson's rule
       * @details Based on the implementation given in Chapter 4.2 in Numerical Recipes [1]
       * The Simpson's rule has an error of order \f$ O(n^{-4}) \f$
       * [1]: W. Press et. al., "Numerical Recipes: The Art of Scientific Computing", Cambridge University Press, 2007
       * @tparam T    The template type. Should be some floating point type
       * @param func  The function which integral is to be evaluated
       * @param a     The start point of the integral
       * @param b     The end point of the integral
       * @param n     The number of interior points used
       */
      template <typename T>
      T simpson(std::function<T (T)> func, T a, T b, int n) {

        if ( a >= b ) {
          ERROR("The start point needs to be smaller than the end point! (", a, " >= ", b, ")");
        }

        if ( n < 1 ) {
          ERROR("The number of interior points needs to be positive! (", n, " < 1)");
        }

        // The total integral sum
        T intSum = (T)1./(T)3. * (func(a) + func(b));
        
        // The finite difference is discerned from the start and end point and the number of interior points
        T h = (b - a) / (T)n;

        // Predefined to avoid divison in the main loop
        T twoThirds  = (T)2. / (T)3.;
        T fourThirds = (T)2. * twoThirds;

        #pragma omp parallel
        {
          // The integral sum within a single thread
          T intThreadSum = (T)0.;

          #pragma omp for nowait 
          for (int i = 1; i < n - 1; i++) {

            if ( i % 2 == 0 ) {
              intThreadSum += twoThirds * func(a + i * h);
            }
            else {
              intThreadSum += fourThirds * func(a + i * h);
            }

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