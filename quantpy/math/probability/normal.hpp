/**
 * @file normal.hpp
 * @author Kasper Rantamäki
 * @date 2026-01-11
 */

#ifndef NORMAL_HPP
#define NORMAL_HPP


#include <cmath>
#include <vector>
#include <random>

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
          return (T)1. / sqrt((T)2. * PI * var) * exp(-pow(x - mean, (T)2.) / ((T)2. * var));

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

          return (T)1. / (T)2. * ((T)1. + erf((x - mean) / (std * sqrt((T)2.))));

        }


        /**
         * @brief Calculates the plain central moments for the given normal distribution
         * @tparam T    The template type. Should support cmath math operations
         * @param mean  The mean of the normal distribution
         * @param std   The standard deviation of the normal distribution
         * @param p     The moment
         * @returns     The plain central moment
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


        /**
         * @brief Samples from a given normal distribution
         * @tparam T    The template type
         * @param mean  The mean of the normal distribution
         * @param std   The standard deviation of the normal distribution
         * @param n     The number of observations sampled
         * @returns     A std::vector object with the samples
         */
        template <typename T>
        std::vector<T> sample(T mean, T std, int n) {

          if ( n < 0 ) {
            ERROR("The number of samples can not be negative! (", n, " < 0)");
          }

          std::vector<T> samples;
          samples.reserve(n);

          std::random_device rd{};
          std::mt19937 generator{rd()};

          std::normal_distribution<T> distribution(mean, std);

          for (int i = 0; i < n; i++) {
            samples.push_back(distribution(generator));
          }

          return samples;

        }


      }

    }

  }

}


#endif