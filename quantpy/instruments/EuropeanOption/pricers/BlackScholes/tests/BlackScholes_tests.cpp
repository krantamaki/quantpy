/**
 * @file BlackScholes_tests.cpp
 * @brief Definition of tests for the Black-Scholes option pricer
 * @details Compile in the main quantpy directory with:
 * > g++ -std=c++17 -mavx -fopenmp -Wall quantpy/instruments/EuropeanOption/pricers/BlackScholes/tests/BlackScholes_tests.cpp -lm -o BlackScholes_tests.o
 * Run with:
 * > ./BlackScholes_tests.o
 * @author Kasper Rantamäki
 * @date 2026-01-16
 */

#include "../BlackScholes.hpp"
#include "../../../../../cpp/testing.hpp"
#include "../../../../../cpp/logging.hpp"


#ifndef TEST_TOL
  #define TEST_TOL 1e-3
#endif


namespace quantpy {

  namespace instruments {

    namespace EuropeanOption {

      namespace BlackScholes_tests {


        // Test 1
        bool test_price1() {

          BlackScholes pricer = BlackScholes<double>(0.04, 120., 0.2, true);

          double correctPrice = 7.0922196;
          double foundPrice   = pricer(105., 1.5);

          DEBUG("Found price: ", foundPrice);

          return ( fabs(correctPrice - foundPrice) < TEST_TOL );

        }


        // Test 2
        bool test_price2() {

          BlackScholes pricer = BlackScholes<double>(0.05, 115., 0.15, false);

          double correctPrice = 3.2765242;
          double foundPrice   = pricer(120., 1.5);

          DEBUG("Found price: ", foundPrice);

          return ( fabs(correctPrice - foundPrice) < TEST_TOL );

        }


        // Test 3
        bool test_delta1() {

          BlackScholes pricer = BlackScholes<double>(0.04, 120., 0.2, true);

          double correctDelta = 0.4294729;
          double foundDelta   = pricer.delta(105., 1.5);

          DEBUG("Found delta: ", foundDelta);

          return ( fabs(correctDelta - foundDelta) < TEST_TOL );

        }


        // Test 4
        bool test_delta2() {

          BlackScholes pricer = BlackScholes<double>(0.05, 115., 0.15, false);

          double correctDelta = -0.2321546;
          double foundDelta   = pricer.delta(120., 1.5);

          DEBUG("Found delta: ", foundDelta);

          return ( fabs(correctDelta - foundDelta) < TEST_TOL );

        }


        // Test 5
        bool test_gamma1() {

          BlackScholes pricer = BlackScholes<double>(0.04, 120., 0.2, true);

          double correctGamma = 0.0152682;
          double foundGamma   = pricer.gamma(105., 1.5);

          DEBUG("Found gamma: ", foundGamma);

          return ( fabs(correctGamma - foundGamma) < TEST_TOL );

        }


        // Test 6
        bool test_gamma2() {

          BlackScholes pricer = BlackScholes<double>(0.05, 115., 0.15, false);

          double correctGamma = 0.0138456;
          double foundGamma   = pricer.gamma(120., 1.5);

          DEBUG("Found gamma: ", foundGamma);

          return ( fabs(correctGamma - foundGamma) < TEST_TOL );

        }


        // Test 7
        bool test_vega1() {

          BlackScholes pricer = BlackScholes<double>(0.04, 120., 0.2, true);

          double correctVega = 50.4994706;
          double foundVega   = pricer.vega(105., 1.5);

          DEBUG("Found vega: ", foundVega);

          return ( fabs(correctVega - foundVega) < TEST_TOL );

        }


        // Test 8
        bool test_vega2() {

          BlackScholes pricer = BlackScholes<double>(0.05, 115., 0.15, false);

          double correctVega = 44.8598224;
          double foundVega   = pricer.vega(120., 1.5);

          DEBUG("Found vega: ", foundVega);

          return ( fabs(correctVega - foundVega) < TEST_TOL );

        }


        // Test 9
        bool test_rho1() {

          BlackScholes pricer = BlackScholes<double>(0.04, 120., 0.2, true);

          double correctRho = 57.0036600;
          double foundRho   = pricer.rho(105., 1.5);

          DEBUG("Found rho: ", foundRho);

          return ( fabs(correctRho - foundRho) < TEST_TOL );

        }


        // Test 10
        bool test_rho2() {

          BlackScholes pricer = BlackScholes<double>(0.05, 115., 0.15, false);

          double correctRho = -46.7026152;
          double foundRho   = pricer.rho(120., 1.5);

          DEBUG("Found rho: ", foundRho);

          return ( fabs(correctRho - foundRho) < TEST_TOL );

        }


        // Test 11
        bool test_theta1() {

          BlackScholes pricer = BlackScholes<double>(0.04, 120., 0.2, true);

          double correctTheta = 4.8867290;
          double foundTheta   = pricer.theta(105., 1.5);

          DEBUG("Found theta: ", foundTheta);

          return ( fabs(correctTheta - foundTheta) < TEST_TOL );

        }


        // Test 12
        bool test_theta2() {

          BlackScholes pricer = BlackScholes<double>(0.05, 115., 0.15, false);

          double correctTheta = 0.6862373;
          double foundTheta   = pricer.theta(120., 1.5);

          DEBUG("Found theta: ", foundTheta);

          return ( fabs(correctTheta - foundTheta) < TEST_TOL );

        }


        // Test 13
        bool test_impliedVol1() {

          BlackScholes pricer = BlackScholes<double>(8.3268554, 104., 1.5, .06, 120., true);

          double correctVol = 0.21;
          double foundVol   = pricer.vol();

          DEBUG("Found vol: ", foundVol);

          return ( fabs(correctVol - foundVol) < TEST_TOL );

        }


        // Test 14
        bool test_impliedVol2() {

          BlackScholes pricer = BlackScholes<double>(5.1058384, 122., 1.2, .03, 115., false);

          double correctVol = 0.19;
          double foundVol   = pricer.vol();

          DEBUG("Found vol: ", foundVol);

          return ( fabs(correctVol - foundVol) < TEST_TOL );

        }


      }

    }

  }

}


int main() {

  quantpy::cpp::logging::verbosity(3);

  quantpy::cpp::testing::TestSuite BlackScholesTests = quantpy::cpp::testing::TestSuite("BlackScholes.hpp");

  BlackScholesTests.addTest(quantpy::instruments::EuropeanOption::BlackScholes_tests::test_price1);
  BlackScholesTests.addTest(quantpy::instruments::EuropeanOption::BlackScholes_tests::test_price2);
  BlackScholesTests.addTest(quantpy::instruments::EuropeanOption::BlackScholes_tests::test_delta1);
  BlackScholesTests.addTest(quantpy::instruments::EuropeanOption::BlackScholes_tests::test_delta2);
  BlackScholesTests.addTest(quantpy::instruments::EuropeanOption::BlackScholes_tests::test_gamma1);
  BlackScholesTests.addTest(quantpy::instruments::EuropeanOption::BlackScholes_tests::test_gamma2);
  BlackScholesTests.addTest(quantpy::instruments::EuropeanOption::BlackScholes_tests::test_vega1);
  BlackScholesTests.addTest(quantpy::instruments::EuropeanOption::BlackScholes_tests::test_vega2);
  BlackScholesTests.addTest(quantpy::instruments::EuropeanOption::BlackScholes_tests::test_rho1);
  BlackScholesTests.addTest(quantpy::instruments::EuropeanOption::BlackScholes_tests::test_rho2);
  BlackScholesTests.addTest(quantpy::instruments::EuropeanOption::BlackScholes_tests::test_theta1);
  BlackScholesTests.addTest(quantpy::instruments::EuropeanOption::BlackScholes_tests::test_theta2);
  BlackScholesTests.addTest(quantpy::instruments::EuropeanOption::BlackScholes_tests::test_impliedVol1);
  BlackScholesTests.addTest(quantpy::instruments::EuropeanOption::BlackScholes_tests::test_impliedVol2);

  return (int)BlackScholesTests.runTests();

}