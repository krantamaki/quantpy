/**
 * @file AbsoluteCEV.hpp
 * @details Constant elasticity of volatility (CEV) family of stochastic processes was first considered
 * by Cox in 1975 [1]. Beckers [2] provided closed form solutions for the general CEV process 
 * 
 * \f$ dS_t = \mu(t) S_t dt + \sigma(t) S_t^{\alpha / 2} dW_t , \f$
 * 
 * where \f$ \mu(t) \f$ is the time-dependent mean, \f$ \sigma(t) \f$ is the time-dependent volatility, 
 * \f$ \alpha \f$ is the elasticity factor and \f$ dW_t \f$ is the Brownian motion.
 * 
 * The absolute model is the CEV process with elasticity factor \f$ \alpha = 0 \f$ for which the a 
 * closed form solution is known.
 * 
 * [1]: J. Cox. "Notes on Option Pricing I: Constant Elasticity of Variance Diffusions." 
 * Working Paper Stanford University, 1975.
 * [2]: S. Beckers, “The constant elasticity of variance model and its implications for option pricing,”
 * The Journal of Finance, vol. 35, no. 3, pp. 661–673, 1980.
 * @todo Implement the pricer
 * @author Kasper Rantamäki
 * @date -
 */

#ifndef ABSOLUTE_CEV_HPP
#define ABSOLUTE_CEV_HPP


#include <cmath>
#include <tuple>


namespace quantpy {

  namespace instruments {

    namespace EuropeanOption {

      namespace AbsoluteCEV {

        // TODO

      }

    }

  }

}


#endif