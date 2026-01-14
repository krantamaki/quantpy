/**
 * @file ExtendedVasicek.hpp
 * @details Both Hull and White [1] as well as Brigo and Mercurio [2] found the same extension of the 
 * Vasicek [3] model. This model is colloquially known as the extended Vasicek model. Under extended
 * Vasicek model the term structure is modelled by a stochastic process
 * 
 * \f$ dr_t = \theta (\mu - r_t) dt + \sigma \dW_t , \f$
 * 
 * where \f$ \theta \f$ is the rate of mean-reversion, \f$ \mu \f$ is the long term mean, \f$ \sigma \f$ is the 
 * volatility and \f$ dW_t \f$ is the Brownian motion.
 * 
 * The extended Vasicek model has a closed form solution for the price of a zero-coupon bond
 * 
 * [1]: J. Hull and A. White, “Pricing interest-rate-derivative securities,” The Review of Financial Studies,
 * vol. 3, no. 4, pp. 573–592, 1990
 * [2]: D. Brigo and F. Mercurio, “A deterministic–shift extension of analytically–tractable and time–homogeneous 
 * short–rate models,” Finance and Stochastics, vol. 5, no. 3, pp. 369–387, 2001
 * [3]: O. Vasicek, “An equilibrium characterization of the term structure,” Journal of Financial Economics,
 * vol. 5, no. 2, pp. 177–188, 1977
 * @todo Implement the pricer
 * @author Kasper Rantamäki
 * @date -
 */

#ifndef EXTENDED_VASICEK_HPP
#define EXTENDED_VASICEK_HPP


#include <cmath>
#include <tuple>


namespace quantpy {

  namespace instruments {

    namespace ZeroCouponBond {

      namespace ExtendedVasicek {

        // TODO

      }

    }

  }

}


#endif