/**
 * @file CIR.hpp
 * @details Cox, Ingersoll and Ross [1] modelled the term structure of interest rates with the Feller square
 * root process [2] 
 * 
 * \f$ dr_t = \theta (\mu - r_t) dt + \sigma \sqrt{r_t} \dW_t , \f$
 * 
 * where \f$ \theta \f$ is the rate of mean-reversion, \f$ \mu \f$ is the long term mean, \f$ \sigma \f$ is the 
 * volatility and \f$ dW_t \f$ is the Brownian motion.
 * 
 * Cox, Ingersoll and Ross provide a closed form solution for the price of a zero-coupon bond.
 * 
 * [1]: J. C. Cox, J. E. Ingersoll, and S. A. Ross, “A theory of the term structure of interest rates,”
 * Econometrica, vol. 53, no. 2, pp. 385–407, 1985
 * [2]: W. Feller, “Two singular diffusion problems,” Annals of Mathematics, vol. 54, no. 1, pp. 173–182, 1951
 * @todo Implement the pricer
 * @author Kasper Rantamäki
 * @date -
 */

#ifndef CIR_HPP
#define CIR_HPP


#include <cmath>
#include <tuple>


namespace quantpy {

  namespace instruments {

    namespace ZeroCouponBond {

      namespace CIR {

        // TODO

      }

    }

  }

}


#endif