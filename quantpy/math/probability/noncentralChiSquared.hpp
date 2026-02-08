/**
 * @file noncentralChiSquared.hpp
 * @author Kasper Rantamäki
 * @date 2026-02-08
 */

#ifndef NONCENTRAL_CHI_SQUARED_HPP
#define NONCENTRAL_CHI_SQUARED_HPP


#ifndef MAX_J
  #define MAX_J 100
#endif


#include <cmath>
#include <vector>
#include <random>

#include "../../cpp/logging.hpp"
#include "../special/hypergeometric.hpp"
#include "../special/incompleteGamma.hpp"
#include "../special/factorial.hpp"


namespace quantpy {

  namespace math {

    namespace probability {

      namespace noncentralChiSquared {


        /**
         * @brief The probability density function for the noncentral chi-squared distribution
         * @details The probability density can be defined either via infinite sum of chi-squared density functions
         * evaluated at the wanted point or via the modified Bessel function. This implementation uses the relation 
         * between Bessel functions and hypergeometric functions to define the PDF [1].
         * [1]: See e.g. https://en.wikipedia.org/wiki/Noncentral_chi-squared_distribution
         * @tparam T      The template type. Should be some floating point type
         * @param k       The degrees of freedom
         * @param lambda  The non-centrality parameter
         * @param x       The point where the PDF is evaluated
         * @returns       The probability density at the point 'x'
         */
        template <typename T>
        T pdf(T k, T lambda, T x) {

          T half_k = k / (T)2.;

          return exp(-(lambda + x) / (T)2.) * special::hypergeometric::hyp0F1(lambda * x / (T)4., half_k) / (pow((T)2., half_k) * tgamma(half_k)) * pow(x, half_k - (T)1.);

        }


        /**
         * @brief The cumualative density function for the noncentral chi-squared distribution
         * @tparam T      The template type. Should be some floating point type
         * @param k       The degrees of freedom
         * @param lambda  The non-centrality parameter
         * @param x       The point where the CDF is evaluated
         * @param max_j   The termination point for the sum. Defaults to 100
         * @returns       The cumulative density at the point 'x'
         */
        template <typename T>
        T cdf(T x, T k, T lambda, int max_j = MAX_J) {

          T sum = (T)0.;

          T half_x = x / (T)2.;
          T half_k = k / (T)2.;

          for (int j = 0; j <= max_j; j++) {
            sum += (pow(lambda / (T)2., (T)j) / special::factorial(j)) * (special::lowerIncompleteGamma(half_k + j, half_x) / tgamma(half_k + j));
          }


          return exp(-lambda / (T)2.) * sum;

        }


        /**
         * @brief Calculates the raw moments for the given noncentral chi-squared distribution
         * @tparam T     The template type. Should support cmath math operations
         * @param k       The degrees of freedom
         * @param lambda  The non-centrality parameter
         * @param p       The moment
         * @returns       The raw moment
         */
        template <typename T>
        T moment(T k, T lambda, int p) {

          return exp(lambda * p / ((T)1. - (T)2. * p)) / pow((T)1. - (T)2. * p, k / (T)2.);

        }


        /**
         * 
         */
        //template <typename T>
        //std::vector<T> sample() {
        //
        //}


      }

    }

  }

}


#endif