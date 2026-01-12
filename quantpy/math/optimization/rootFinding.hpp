/**
 * @file rootFinding.hpp
 * @author Kasper Rantamäki
 * @date 2026-01-10
 */

#ifndef ROOT_FINDING_HPP
#define ROOT_FINDING_HPP


#include <functional>
#include <cmath>

#include "../../cpp/logging.hpp"


namespace quantpy {

  namespace math {

    namespace optimization {

      namespace rootFinding {


        /**
         * @brief Simple templated bisection method for finding the root of a function
         * @tparam T     The template type. Should support basic math operation like addition and division
         * @param func   The function of which root is to be found
         * @param start  The start point of the search interval
         * @param end    The end point of the search interval. Inequality 'start' < 'end' should hold
         * @param atol   The tolerance for stopping condition (absolute tolerance): \f$ |V_n - V_{n - 1}| < atol \f$. Defaults to 1e-6. 
         * If 'func' is a discrete function the default value doesn't compile
         * @param rtol   The tolerance for stopping condition (relative tolerance): \f$ \left| \frac{V_n - V_{n - 1}}{V_n} \right| < atol \f$. Defaults to 1e-6. 
         * If 'func' is a discrete function the default value doesn't compile
         * @returns      The root of the function
         */
        template <typename T>
        T bisectionMethod(std::function<T (T)> func, T start, T end, T atol = 1e-6, T rtol = 1e-6) {

          if ( start >= end ) {
            ERROR("Start point needs to be lower than the end point! (", start, " !< ", end, ")");
          }

          if ( func(start) >= func(end) ) {
            ERROR("Function value at start point needs to be less that at the end point! (", func(start) " !< ", func(end), ")");
          }

          T mid = (T)((end + start) / 2);
          T func_mid = func(mid);

          if ( func_mid == (T)0.0 ) {
            return mid;
          }

          T last_func_mid = func(end); 

          while ( (abs(func_mid - last_func_mid) > atol) & (abs((func_mid - last_func_mid) / func_mid) > rtol) ) {

            last_func_mid = func_mid;

            if ( func_mid * func(start) < (T)0.0 ) { 
              end = mid;
            }
            else {
              start = mid;
            }

            mid = (T)((end + start) / 2);
            func_mid = func(mid);

          }

          return mid;

        }


      }

    }

  }

}


#endif