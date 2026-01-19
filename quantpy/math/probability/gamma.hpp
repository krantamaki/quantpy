/**
 * @file gamma.hpp
 * @author Kasper Rantamäki
 * @date 2026-01-19
 */

#ifndef GAMMA_DISTRIBUTION_HPP
#define GAMMA_DISTRIBUTION_HPP


#include <cmath>
#include <vector>
#include <random>

#include "../../cpp/logging.hpp"
#include "../special/incompleteGamma.hpp"


namespace quantpy {

  namespace math {

    namespace probability {

      namespace gamma {
      

        /**
         * @brief The general gamma distribution probability density function 
         * @tparam T     The template type. Should support cmath math operations
         * @param shape  The scale parameter for the gamma distribution
         * @param rate   The rate parameter for the gamma distribution. Note that the rate parameter
         * is the inverse of the scale parameter, which could also be used to define the distribution
         * @param x      The point for which the density is evaluated
         * @returns      The probability density at the point 'x'
         */
        template <typename T>
        T pdf(T shape, T rate, T x) {

          return pow(rate, shape) / tgamma(shape) * pow(x, shape - (T)1.) * exp(-rate * x);

        }


        /**
         * @brief The general gamma distribution cumulative density function 
         * @tparam T     The template type. Should support cmath math operations
         * @param shape  The scale parameter for the gamma distribution
         * @param rate   The rate parameter for the gamma distribution
         * @param x      The point for which the density is evaluated
         * @returns      The cumulative density at the point 'x'
         */
        template <typename T>
        T cdf(T shape, T rate, T x) {

          return (T)1. / tgamma(shape) * special::lowerIncompleteGamma(shape, rate * x);

        }


        /**
         * @brief Calculates the raw moments for the given gamma distribution
         * @tparam T     The template type. Should support cmath math operations
         * @param shape  The scale parameter for the gamma distribution
         * @param rate   The rate parameter for the gamma distribution. Note that the rate parameter
         * is the inverse of the scale parameter, which could also be used to define the distribution
         * @param p      The moment
         * @returns      The raw moment
         */
        template <typename T>
        T moment(T shape, T rate, int p) {

          if ( p < 0 ) {
            ERROR("The moment can not be negative! (", p, " < 0)");
          }

          return (T)1. / rate * tgamma(shape + (T)p) / tgamma(shape);

        }


        /**
         * @brief Samples from a given gamma distribution
         * @tparam T     The template type
         * @param shape  The scale parameter for the gamma distribution
         * @param rate   The rate parameter for the gamma distribution. Note that the rate parameter
         * is the inverse of the scale parameter, which could also be used to define the distribution
         * @param n      The number of observations sampled
         * @returns      A std::vector object with the samples
         */
        template <typename T>
        std::vector<T> sample(T shape, T rate, int n) {

          if ( n < 0 ) {
            ERROR("The number of samples can not be negative! (", n, " < 0)");
          }

          std::vector<T> samples;
          samples.reserve(n);

          std::random_device rd{};
          std::mt19937 generator{rd()};

          std::gamma_distribution<T> distribution(shape, (T)1. / rate);

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