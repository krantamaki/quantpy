/**
 * @file CIROption.hpp
 * @details Cox, Ingersoll and Ross [1] modelled the term structure of interest rates with the Feller square
 * root process [2] 
 * 
 * \f$ dr_t = \theta (\mu - r_t) dt + \sigma \sqrt{r_t} \dW_t , \f$
 * 
 * where \f$ \theta \f$ is the rate of mean-reversion, \f$ \mu \f$ is the long term mean, \f$ \sigma \f$ is the 
 * volatility and \f$ dW_t \f$ is the Brownian motion.
 * 
 * Cox, Ingersoll and Ross provide a closed form solution for the price of a zero-coupon bond, which in turn can be 
 * used to price an option on the price of the zero-coupon bond.
 * 
 * [1]: J. C. Cox, J. E. Ingersoll, and S. A. Ross, “A theory of the term structure of interest rates,”
 * Econometrica, vol. 53, no. 2, pp. 385–407, 1985
 * [2]: W. Feller, “Two singular diffusion problems,” Annals of Mathematics, vol. 54, no. 1, pp. 173–182, 1951
 * @author Kasper Rantamäki
 * @date 2026-02-08
 */

#ifndef CIR_OPTION_HPP
#define CIR_OPTION_HPP


#include <cmath>

#include "../BaseZeroCouponBondOptionPricer.hpp"
#include "../../../ZeroCouponBond/pricers/CIR/CIR.hpp"
#include "../../../../math/probability/noncentralChiSquared.hpp"


namespace quantpy {

  namespace instruments {

    namespace ZeroCouponBondOption {


      /**
       * @brief The CIR helper function \f$ \gamma \f$. To be called by 'CIROption' class
       * @details Note that \f$ \gamma \f$ means the same as \f$ h \f$ in 'CIR.hpp' file
       * @tparam T     The template type. Should support cmath math operations
       * @param theta  The rate of mean reversion (\f$ \theta \f$)
       * @param vol    The volatility (\f$ \sigma \f$)
       * @returns      The value \f$ \gamma \f$
       */
      template <typename T>
      T __CIROptionGamma(T theta, T vol) {

        return sqrt(theta * theta + (T)2. * vol * vol);

      }


      /**
       * @brief The CIR helper function \f$ \phi \f$. To be called by 'CIROption' class
       * @tparam T     The template type. Should support cmath math operations
       * @param tau    The time to maturity in years
       * @param theta  The rate of mean reversion (\f$ \theta \f$)
       * @param vol    The volatility (\f$ \sigma \f$)
       * @returns      The value \f$ \phi \f$
       */
      template <typename T>
      T __CIROptionPhi(T tau, T theta, T vol) {

        T gamma = __CIROptionGamma<T>(theta, vol);

        return (T)2. * gamma / (vol * vol * (exp(gamma * tau) - (T)1.));

      }


      /**
       * @brief The CIR helper function \f$ \psi \f$. To be called by 'CIROption' class
       * @tparam T     The template type. Should support cmath math operations
       * @param theta  The rate of mean reversion (\f$ \theta \f$)
       * @param vol    The volatility (\f$ \sigma \f$)
       * @returns      The value \f$ \psi \f$
       */
      template <typename T>
      T __CIROptionPsi(T theta, T vol) {

        T gamma = __CIROptionGamma<T>(theta, vol);

        return (theta + gamma) / (vol * vol);

      }


      /**
       * @brief The CIR helper function \f$ r^* \f$. To be called by 'CIROption' class
       * @tparam T     The template type. Should support cmath math operations
       * @param tau    The time to maturity in years
       * @param K      The strike price
       * @param theta  The rate of mean reversion (\f$ \theta \f$)
       * @param mu     The long-term mean (\f$ \mu \f$)
       * @param vol    The volatility (\f$ \sigma \f$)
       * @returns      The value \f$ r^* \f$
       */
      template <typename T>
      T __CIROptionR(T tau, T K, T theta, T mu, T vol) {

        T A = instruments::ZeroCouponBond::__CIRA<T>(tau, theta, mu, vol);
        T B = instruments::ZeroCouponBond::__CIRB<T>(tau, theta, mu, vol)

        return log(A / K) / B;

      }


      /**
       * @brief The CIR zero-coupon bond option price. To be called by 'CIROption' class
       * @tparam T      The template type. Should support cmath math operations
       * @param rt      The prevailing (continuously compounded) interest rate
       * @param tau     The time to maturity in years
       * @param K       The strike price
       * @param theta   The rate of mean reversion (\f$ \theta \f$)
       * @param mu      The long-term mean (\f$ \mu \f$)
       * @param vol     The volatility (\f$ \sigma \f$)
       * @param isCall  Boolean flag telling if the option is a call or a put. Defaults to 'true'
       * @returns       The price of the zero-coupon bond option
       */
      template <typename T>
      T __CIROptionPrice(T rt, T tau, T theta, T mu, T vol, bool isCall = true) {

        T gamma = __CIROptionGamma(theta, vol);
        T P     = instruments::ZeroCouponBond::__CIRPrice<T>(rt, tau, theta, mu, vol);
        T B     = instruments::ZeroCouponBond::__CIRB<T>(tau, theta, mu, vol);
        T phi   = __CIROptionPhi<T>(tau, theta, vol);
        T psi   = __CIROptionPsi<T>(theta, vol);
        T R     = __CIROptionR<T>(tau, K, theta, mu, vol);

        T lambda = (T)4. * theta * mu / (vol * vol);
        T numer  = (T)2. * phi * phi * rt * exp(gamma * tau);

        T callPrice = P * quantpy::math::probability::noncentralChiSquared::pdf<T>((T)2. * R * (phi + psi + B), lambda, numer / (phi + psi + B))
                    - K * P * quantpy::math::probability::noncentralChiSquared::pdf<T>((T)2. * R * (phi + psi), lambda, numer / (phi + psi));

        if ( isCall ) {
          return callPrice;
        }

        return callPrice - P + K * exp(-((T)1. + r) * tau);
               
      }


      /**
       * @brief Simple pricer based on the analytical solution for a zero-coupon bond option price found by Cox, Ingersoll and Ross
       * @tparam T  The template type. Should be some floating point number type
       */
      template <typename T>
      class CIROption : public BaseZeroCouponBondOptionPricer<T> {

        protected:

          T _theta;  /**< The rate of mean reversion */
          T _mu;     /**< The long-term mean */

        public:

          /**
           * @brief Default constructor
           * @returns  Uninitialized CIROption object
           */
          CIROption() {  }


          /**
           * @brief Main constructor
           * @param theta  The rate of mean reversion
           * @param mu     The long-term mean
           * @param vol    The volatility
           * @returns      Initialized CIROption object
           */
          CIROption(T theta, T mu, T vol) {

            this->_theta = theta;
            this->_mu    = mu;
            this->_vol   = vol;

          }


          /**
           * @brief The pricing method
           * @param rt   The prevailing (continuously compounded) interest rate
           * @param tau  The time to maturity in years
           * @param vol  The volatility. Defaults to the one used to initialize the object
           * @returns    The price of the zero-coupon bond option
           */
          T operator() (T rt, T tau, T vol = (T)-1.) const override {

            if ( vol == (T)-1. ) {
              return __CIROptionPrice<T>(rt, tau, this->_theta, this->_mu, this->_vol, this->_isCall);
            } 
            
            return __CIROptionPrice<T>(rt, tau, this->_theta, this->_mu, vol, this->_isCall);

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