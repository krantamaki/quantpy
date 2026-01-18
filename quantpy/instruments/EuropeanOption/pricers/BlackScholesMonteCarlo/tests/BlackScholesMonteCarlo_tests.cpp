/**
 * @file BlackScholesMonteCarlo_tests.cpp
 * @brief Definition of tests for the Black-Scholes monte carlo option pricer
 * @details Compile in the main quantpy directory with:
 * > g++ -std=c++17 -mavx -fopenmp -Wall quantpy/instruments/EuropeanOption/pricers/BlackScholesMonteCarlo/tests/BlackScholesMonteCarlo_tests.cpp -lm -o BlackScholesMonteCarlo_tests.o
 * Run with:
 * > ./BlackScholesMonteCarlo_tests.o
 * @author Kasper Rantamäki
 * @date 2026-01-18
 */

#include "../BlackScholesMonteCarlo.hpp"
#include "../../../../../math/stochasticProcesses/GeometricBrownianMotion.hpp"
#include "../../../../../cpp/testing.hpp"
#include "../../../../../cpp/logging.hpp"


using namespace quantpy::math::stochasticProcesses;


#ifndef TEST_TOL
  #define TEST_TOL 1e-1  // Note that the tolerance is lower since the simulations will give different values from one run to the other
#endif


namespace quantpy {

  namespace instruments {

    namespace EuropeanOption {

      namespace BlackScholesMonteCarlo_tests {


        // Test 1
        bool test_price1() {

          auto rts   = [](double tau) { return 0.04; };
          auto qts   = [](double tau) { return 0.00; };
          auto volts = [](double tau) { return 0.2; };

          GeometricBrownianMotion process = GeometricBrownianMotion<double>(rts, qts, volts);
          BlackScholesMonteCarlo pricer   = BlackScholesMonteCarlo<double>(process, 120., 50000, 100);

          double correctPrice = 7.0922196;
          double foundPrice   = pricer(105., 1.5);

          DEBUG("Found price: ", foundPrice);

          return ( fabs(correctPrice - foundPrice) < TEST_TOL );

        }


        // Test 2
        bool test_price2() {

          auto rts   = [](double tau) { return 0.05; };
          auto qts   = [](double tau) { return 0.00; };
          auto volts = [](double tau) { return 0.15; };

          GeometricBrownianMotion process = GeometricBrownianMotion<double>(rts, qts, volts);
          BlackScholesMonteCarlo pricer   = BlackScholesMonteCarlo<double>(process, 115, 50000, 100, false);

          double correctPrice = 3.2765242;
          double foundPrice   = pricer(120., 1.5);

          DEBUG("Found price: ", foundPrice);

          return ( fabs(correctPrice - foundPrice) < TEST_TOL );

        }


        // Test 3
        bool test_delta1() {

          auto rts   = [](double tau) { return 0.04; };
          auto qts   = [](double tau) { return 0.00; };
          auto volts = [](double tau) { return 0.2; };

          GeometricBrownianMotion process = GeometricBrownianMotion<double>(rts, qts, volts);
          BlackScholesMonteCarlo pricer   = BlackScholesMonteCarlo<double>(process, 120., 50000, 100);

          double correctDelta = 0.4294729;
          double foundDelta   = pricer.delta(105., 1.5, 0.1);

          DEBUG("Found delta: ", foundDelta);

          return ( fabs(correctDelta - foundDelta) < TEST_TOL );

        }


        // Test 4
        bool test_delta2() {

          auto rts   = [](double tau) { return 0.05; };
          auto qts   = [](double tau) { return 0.00; };
          auto volts = [](double tau) { return 0.15; };

          GeometricBrownianMotion process = GeometricBrownianMotion<double>(rts, qts, volts);
          BlackScholesMonteCarlo pricer   = BlackScholesMonteCarlo<double>(process, 115, 50000, 100, false);

          double correctDelta = -0.2321546;
          double foundDelta   = pricer.delta(120., 1.5, 0.1);

          DEBUG("Found delta: ", foundDelta);

          return ( fabs(correctDelta - foundDelta) < TEST_TOL );

        }


        // Test 5
        bool test_gamma1() {

          auto rts   = [](double tau) { return 0.04; };
          auto qts   = [](double tau) { return 0.00; };
          auto volts = [](double tau) { return 0.2; };

          GeometricBrownianMotion process = GeometricBrownianMotion<double>(rts, qts, volts);
          BlackScholesMonteCarlo pricer   = BlackScholesMonteCarlo<double>(process, 120., 50000, 100);

          double correctGamma = 0.0152682;
          double foundGamma   = pricer.gamma(105., 1.5, 0.1);

          DEBUG("Found gamma: ", foundGamma);

          return ( fabs(correctGamma - foundGamma) < TEST_TOL );

        }


        // Test 6
        bool test_gamma2() {

          auto rts   = [](double tau) { return 0.05; };
          auto qts   = [](double tau) { return 0.00; };
          auto volts = [](double tau) { return 0.15; };

          GeometricBrownianMotion process = GeometricBrownianMotion<double>(rts, qts, volts);
          BlackScholesMonteCarlo pricer   = BlackScholesMonteCarlo<double>(process, 115, 50000, 100, false);

          double correctGamma = 0.0138456;
          double foundGamma   = pricer.gamma(120., 1.5, 0.1);

          DEBUG("Found gamma: ", foundGamma);

          return ( fabs(correctGamma - foundGamma) < TEST_TOL );

        }


        // Test 7
        bool test_vega1() {

          auto rts   = [](double tau) { return 0.04; };
          auto qts   = [](double tau) { return 0.00; };
          auto volts = [](double tau) { return 0.2; };

          GeometricBrownianMotion process = GeometricBrownianMotion<double>(rts, qts, volts);
          BlackScholesMonteCarlo pricer   = BlackScholesMonteCarlo<double>(process, 120., 50000, 100);

          double correctVega = 50.4994706;
          double foundVega   = pricer.vega(105., 1.5, 0.1);

          DEBUG("Found vega: ", foundVega);

          return ( fabs(correctVega - foundVega) < TEST_TOL );

        }


        // Test 8
        bool test_vega2() {

          auto rts   = [](double tau) { return 0.05; };
          auto qts   = [](double tau) { return 0.00; };
          auto volts = [](double tau) { return 0.15; };

          GeometricBrownianMotion process = GeometricBrownianMotion<double>(rts, qts, volts);
          BlackScholesMonteCarlo pricer   = BlackScholesMonteCarlo<double>(process, 115, 50000, 100, false);

          double correctVega = 44.8598224;
          double foundVega   = pricer.vega(120., 1.5, 0.1);

          DEBUG("Found vega: ", foundVega);

          return ( fabs(correctVega - foundVega) < TEST_TOL );

        }


        // Test 9
        bool test_rho1() {

          auto rts   = [](double tau) { return 0.04; };
          auto qts   = [](double tau) { return 0.00; };
          auto volts = [](double tau) { return 0.2; };

          GeometricBrownianMotion process = GeometricBrownianMotion<double>(rts, qts, volts);
          BlackScholesMonteCarlo pricer   = BlackScholesMonteCarlo<double>(process, 120., 50000, 100);

          double correctRho = 57.0036600;
          double foundRho   = pricer.rho(105., 1.5, 0.1);

          DEBUG("Found rho: ", foundRho);

          return ( fabs(correctRho - foundRho) < TEST_TOL );

        }


        // Test 10
        bool test_rho2() {

          auto rts   = [](double tau) { return 0.05; };
          auto qts   = [](double tau) { return 0.00; };
          auto volts = [](double tau) { return 0.15; };

          GeometricBrownianMotion process = GeometricBrownianMotion<double>(rts, qts, volts);
          BlackScholesMonteCarlo pricer   = BlackScholesMonteCarlo<double>(process, 115, 50000, 100, false);

          double correctRho = -46.7026152;
          double foundRho   = pricer.rho(120., 1.5, 0.1);

          DEBUG("Found rho: ", foundRho);

          return ( fabs(correctRho - foundRho) < TEST_TOL );

        }


        // Test 11
        bool test_theta1() {

          auto rts   = [](double tau) { return 0.04; };
          auto qts   = [](double tau) { return 0.00; };
          auto volts = [](double tau) { return 0.2; };

          GeometricBrownianMotion process = GeometricBrownianMotion<double>(rts, qts, volts);
          BlackScholesMonteCarlo pricer   = BlackScholesMonteCarlo<double>(process, 120., 50000, 100);

          double correctTheta = 4.8867290;
          double foundTheta   = pricer.theta(105., 1.5, 0.1);

          DEBUG("Found theta: ", foundTheta);

          return ( fabs(correctTheta - foundTheta) < TEST_TOL );

        }


        // Test 12
        bool test_theta2() {

          auto rts   = [](double tau) { return 0.05; };
          auto qts   = [](double tau) { return 0.00; };
          auto volts = [](double tau) { return 0.15; };

          GeometricBrownianMotion process = GeometricBrownianMotion<double>(rts, qts, volts);
          BlackScholesMonteCarlo pricer   = BlackScholesMonteCarlo<double>(process, 115, 50000, 100, false);

          double correctTheta = 0.6862373;
          double foundTheta   = pricer.theta(120., 1.5, 0.1);

          DEBUG("Found theta: ", foundTheta);

          return ( fabs(correctTheta - foundTheta) < TEST_TOL );

        }


      }

    }

  }

}


int main() {

  quantpy::cpp::logging::verbosity(3);

  quantpy::cpp::testing::TestSuite BlackScholesMonteCarloTests = quantpy::cpp::testing::TestSuite("BlackScholesMonteCarlo.hpp");

  BlackScholesMonteCarloTests.addTest(quantpy::instruments::EuropeanOption::BlackScholesMonteCarlo_tests::test_price1);
  BlackScholesMonteCarloTests.addTest(quantpy::instruments::EuropeanOption::BlackScholesMonteCarlo_tests::test_price2);
  BlackScholesMonteCarloTests.addTest(quantpy::instruments::EuropeanOption::BlackScholesMonteCarlo_tests::test_delta1);
  BlackScholesMonteCarloTests.addTest(quantpy::instruments::EuropeanOption::BlackScholesMonteCarlo_tests::test_delta2);
  BlackScholesMonteCarloTests.addTest(quantpy::instruments::EuropeanOption::BlackScholesMonteCarlo_tests::test_gamma1);
  BlackScholesMonteCarloTests.addTest(quantpy::instruments::EuropeanOption::BlackScholesMonteCarlo_tests::test_gamma2);
  BlackScholesMonteCarloTests.addTest(quantpy::instruments::EuropeanOption::BlackScholesMonteCarlo_tests::test_vega1);
  BlackScholesMonteCarloTests.addTest(quantpy::instruments::EuropeanOption::BlackScholesMonteCarlo_tests::test_vega2);
  BlackScholesMonteCarloTests.addTest(quantpy::instruments::EuropeanOption::BlackScholesMonteCarlo_tests::test_rho1);
  BlackScholesMonteCarloTests.addTest(quantpy::instruments::EuropeanOption::BlackScholesMonteCarlo_tests::test_rho2);
  BlackScholesMonteCarloTests.addTest(quantpy::instruments::EuropeanOption::BlackScholesMonteCarlo_tests::test_theta1);
  BlackScholesMonteCarloTests.addTest(quantpy::instruments::EuropeanOption::BlackScholesMonteCarlo_tests::test_theta2);

  return (int)BlackScholesMonteCarloTests.runTests();

}