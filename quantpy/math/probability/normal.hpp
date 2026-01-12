/**
 * @file normal.hpp
 * @author Kasper Rantamäki
 * @date 2026-01-11
 */

#ifndef NORMAL_HPP
#define NORMAL_HPP


#include <cmath>

#include "../../cpp/logging.hpp"
#include "../special/factorial.hpp"


#ifndef PI
  #define PI 3.141592653589793
#endif


namespace quantpy {

  namespace math {

    namespace probability {

      namespace normal {
      

        /**
         * @brief The general normal probability density function 
         * @tparam T    The template type. Should support cmath math operations
         * @param mean  The mean of the normal distribution
         * @param std   The standard deviation of the normal distribution
         * @param x     The point for which the density is evaluated
         * @returns     The probability density at the point 'x'
         */
        template <typename T>
        T pdf(T mean, T std, T x) {

          T var = std * std;
          return 1 / sqrt(2 * PI * var) * exp(-pow(x - mean, (T)2) / (2 * var));

        }


        /**
         * @brief The general normal cumulative density function 
         * @tparam T    The template type. Should support cmath math operations
         * @param mean  The mean of the normal distribution
         * @param std   The standard deviation of the normal distribution
         * @param x     The point for which the density is evaluated
         * @returns     The cumulative density at the point 'x'
         */
        template <typename T>
        T cdf(T mean, T std, T x) {

          return 1 / 2 * (1 + erf((x - mean) / (std * sqrt(2))));

        }


        /**
         * @brief Calculates the plain central moments for the given normal distribution
         * @tparam T    The template type. Should support cmath math operations
         * @param mean  The mean of the normal distribution
         * @param std   The standard deviation of the normal distribution
         * @param p     The moment
         * @returns     The cumulative density at the point 'x'
         */
        template <typename T>
        T moment(T mean, T std, int p) {

          if ( p < 0 ) {
            ERROR("The moment can not be negative! (", p, " < 0)");
          }

          if ( p % 2 == 0 ) {
            return pow(std, (T)p) * (T)special::doubleFactorial<int>(p - 1);
          }

          return (T)0.0;

        }


      }

    }

  }

}


#endif