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
 * @author Kasper Rantamäki
 * @date 2026-02-04
 */

#ifndef VASICEK_HPP
#define VASICEK_HPP


#include <cmath>


namespace quantpy {

  namespace instruments {

    namespace ZeroCouponBond {


      /**
       * @brief The Vasicek helper function \f$ B \f$. To be called by 'Vasicek' class
       * @tparam T     The template type. Should support cmath math operations
       * @param tau    The time to maturity in years
       * @param theta  The rate of mean reversion (\f$ \theta \f$)
       * @returns      The value \f$ B \f$
       */
      template <typename T>
      T __VasicekB(T tau, T theta) {

        return ((T)1. - exp(-theta * tau)) / theta;

      }


      /**
       * @brief The Vasicek helper function \f$ A \f$. To be called by 'Vasicek' class
       * @tparam T     The template type. Should support cmath math operations
       * @param tau    The time to maturity in years
       * @param theta  The rate of mean reversion (\f$ \theta \f$)
       * @param mu     The long-term mean (\f$ \mu \f$)
       * @param vol    The volatility (\f$ \sigma \f$)
       * @returns      The value \f$ A \f$
       */
      template <typename T>
      T __VasicekA(T tau, T theta, T mu, T vol) {

        T B = __VasicekB(tau, theta);

        return exp((mu - vol * vol / ((T)2. * theta * theta)) * (B - tau) - vol * vol / ((T)4. * theta) * B * B);

      }


      /**
       * @brief The Vasicek zero-coupon bond price. To be called by 'Vasicek' class
       * @tparam T     The template type. Should support cmath math operations
       * @param rt     The prevailing (continuously compounded) interest rate
       * @param tau    The time to maturity in years
       * @param theta  The rate of mean reversion (\f$ \theta \f$)
       * @param mu     The long-term mean (\f$ \mu \f$)
       * @param vol    The volatility (\f$ \sigma \f$)
       * @returns      The price of the zero-coupon bond
       */
      template <typename T>
      T __VasicekPrice(T rt, T tau, T theta, T mu, T vol) {

        return __VasicekA(tau, theta, mu, vol) * exp(-__VasicekB(tau, theta) * rt);

      }


      /**
       * @todo Implement the estimator. This requires nonlinear least-squares which is not yet implemented
       */
      template <typename T>
      void __VasicekYieldCurveCalibration() {
        return void
      }


       /**
       * @brief Simple pricer based on the analytical solution for a zero-coupon bond price found by Vasicek
       * @tparam T  The template type. Should be some floating point number type
       */
      template <typename T>
      class Vasicek : public BaseZeroCouponBondPricer<T> {

        protected:

          T _theta;  /**< The rate of mean reversion */
          T _mu;     /**< The long-term mean */

        public:

          /**
           * @brief Default constructor
           * @returns  Uninitialized Vasicek object
           */
          Vasicek() {  }


          /**
           * @brief Main constructor
           * @param theta  The rate of mean reversion
           * @param mu     The long-term mean
           * @param vol    The volatility
           * @returns      Initialized Vasicek object
           */
          Vasicek(T theta, T mu, T vol) {

            this->_theta = theta;
            this->_mu    = mu;
            this->_vol   = vol;

          }


          /**
           * @brief The pricing method
           * @param rt   The prevailing (continuously compounded) interest rate
           * @param tau  The time to maturity in years
           * @param vol  The volatility. Defaults to the one used to initialize the object
           * @returns    The price of the zero-coupon bond
           */
          T operator() (T rt, T tau, T vol = (T)-1.) const override {

            if ( vol == (T)-1. ) {
              return __VasicekPrice(rt, tau, this->_theta, this->_mu, this->_vol);
            } 
            
            return __VasicekPrice(rt, tau, this->_theta, this->_mu, vol);

          }


          /**
           * @brief Setter for the rate of mean reversion
           * @returns The rate of mean reversion
           */
          T& theta() { return this->_theta; }
          

          /**
           * @brief Getter for the rate of mean reversion
           * @returns The rate of mean reversion
           */
          const T& theta() const { return this->_theta; }


          /**
           * @brief Setter for the long-term mean
           * @returns The long-term mean
           */
          T& mu() { return this->_mu; }
          

          /**
           * @brief Getter for the long-term mean
           * @returns The long-term mean
           */
          const T& mu() const { return this->_mu; }


      };
      

    }

  }

}


#endif