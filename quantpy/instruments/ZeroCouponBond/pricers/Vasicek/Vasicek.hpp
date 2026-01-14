/**
 * @file Vasicek.hpp
 * @details Vasicek [1] modelled the term structure of interest rates with the Ornstein-Uhlenbeck [2] stochastic 
 * process 
 * 
 * \f$ dr_t = \theta (\mu - r_t) dt + \sigma \dW_t , \f$
 * 
 * where \f$ \theta \f$ is the rate of mean-reversion, \f$ \mu \f$ is the long term mean, \f$ \sigma \f$ is the 
 * volatility and \f$ dW_t \f$ is the Brownian motion.
 * 
 * Vasicek provides a closed form solution for the price of a zero-coupon bond.
 * 
 * [1]: O. Vasicek, “An equilibrium characterization of the term structure,” Journal of Financial Economics,
 * vol. 5, no. 2, pp. 177–188, 1977
 * [2]: G. E. Uhlenbeck and L. S. Ornstein, “On the theory of the Brownian motion,” Physical Review, 
 * vol. 36, no. 5, pp. 823–841, 1930
 * @todo Implement the pricer
 * @author Kasper Rantamäki
 * @date -
 */

#ifndef VASICEK_HPP
#define VASICEK_HPP


#include <cmath>
#include <tuple>


namespace quantpy {

  namespace instruments {

    namespace ZeroCouponBond {

      namespace Vasicek {

        // TODO

      }

    }

  }

}


#endif