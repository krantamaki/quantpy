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
 * @author Kasper Rantamäki
 * @date 2026-02-03
 */

#ifndef CIR_HPP
#define CIR_HPP


#include <cmath>

#include "../BaseZeroCouponBondPricer.hpp"


namespace quantpy {

  namespace instruments {

    namespace ZeroCouponBond {

      
      /**
       * @brief The CIR helper function \f$ h \f$. To be called by 'CIR' class
       * @tparam T     The template type. Should support cmath math operations
       * @param theta  The rate of mean reversion (\f$ \theta \f$)
       * @param vol    The volatility (\f$ \sigma \f$)
       * @returns      The value \f$ h \f$
       */
      template <typename T>
      T __CIRh(T theta, T vol) {

        return sqrt(theta * theta + (T)2. * vol * vol);

      }


      /**
       * @brief The CIR helper function \f$ A \f$. To be called by 'CIR' class
       * @tparam T     The template type. Should support cmath math operations
       * @param tau    The time to maturity in years
       * @param theta  The rate of mean reversion (\f$ \theta \f$)
       * @param mu     The long-term mean (\f$ \mu \f$)
       * @param vol    The volatility (\f$ \sigma \f$)
       * @returns      The value \f$ A \f$
       */
      template <typename T>
      T __CIRA(T tau, T theta, T mu, T vol) {

        T h = __CIRh(theta, vol);

        return pow(((T)2. * h * exp((theta + h) * tau / (T)2.)) / ((T)2. * h + (theta + h) * (exp(h * tau)) - (T)1.), 
                   (T)2. * theta * mu / (vol * vol));

      }


      /**
       * @brief The CIR helper function \f$ B \f$. To be called by 'CIR' class
       * @tparam T     The template type. Should support cmath math operations
       * @param tau    The time to maturity in years
       * @param theta  The rate of mean reversion (\f$ \theta \f$)
       * @param mu     The long-term mean (\f$ \mu \f$)
       * @param vol    The volatility (\f$ \sigma \f$)
       * @returns      The value \f$ B \f$
       */
      template <typename T>
      T __CIRB(T tau, T theta, T mu, T vol) {

        T h = __CIRh(theta, vol);

        return ((T)2. * h * exp((a + h) * tau / (T)2.)) / ((T)2. * h + (a + h) * (exp(h * tau) - (T)1.));

      }


      /**
       * @brief The CIR zero-coupon bond price. To be called by 'CIR' class
       * @tparam T     The template type. Should support cmath math operations
       * @param rt     The prevailing (continuously compounded) interest rate
       * @param tau    The time to maturity in years
       * @param theta  The rate of mean reversion (\f$ \theta \f$)
       * @param mu     The long-term mean (\f$ \mu \f$)
       * @param vol    The volatility (\f$ \sigma \f$)
       * @returns      The price of the zero-coupon bond
       */
      template <typename T>
      T __CIRPrice(T rt, T tau, T theta, T mu, T vol) {

        return __CIRA(tau, theta, mu, vol) * exp(-__CIRB(tau, theta, mu, vol) * rt);

      }


      /**
       * @todo Implement the estimator. This requires nonlinear least-squares which is not yet implemented
       */
      template <typename T>
      void __CIRYieldCurveCalibration() {
        return void
      }


      /**
       * @brief Simple pricer based on the analytical solution for a zero-coupon bond price found by Cox, Ingersoll and Ross
       * @tparam T  The template type. Should be some floating point number type
       */
      template <typename T>
      class CIR : public BaseZeroCouponBondPricer<T> {

        protected:

          T _theta;  /**< The rate of mean reversion */
          T _mu;     /**< The long-term mean */

        public:

          /**
           * @brief Default constructor
           * @returns  Uninitialized CIR object
           */
          CIR() {  }


          /**
           * @brief Main constructor
           * @param theta  The rate of mean reversion
           * @param mu     The long-term mean
           * @param vol    The volatility
           * @returns      Initialized CIR object
           */
          CIR(T theta, T mu, T vol) {

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
              return __CIRPrice(rt, tau, this->_theta, this->_mu, this->_vol);
            } 
            
            return __CIRPrice(rt, tau, this->_theta, this->_mu, vol);

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