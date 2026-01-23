/**
 * @file AbsoluteCEV_tests.cpp
 * @brief Definition of tests for the Absolute CEV option pricer
 * @details These tests are based on the values in Table II in Beckers (1980). Thus, the
 * volatility value used in the tests is the local volatility defined as \f$ \sigma^2 / S^{2 - \alpha} \f$.
 * 
 * Compile in the main quantpy directory with:
 * > g++ -std=c++17 -mavx -fopenmp -Wall quantpy/instruments/EuropeanOption/pricers/AbsoluteCEV/tests/AbsoluteCEV_tests.cpp -lm -o AbsoluteCEV_tests.o
 * Run with:
 * > ./AbsoluteCEV_tests.o
 * @author Kasper Rantamäki
 * @date 2026-01-23
 */

#include <cmath>

#include "../AbsoluteCEV.hpp"
#include "../../../../../cpp/testing.hpp"
#include "../../../../../cpp/logging.hpp"


#ifndef TEST_TOL
  #define TEST_TOL 1e-1
#endif


#ifndef S
  #define S 20.
#endif


#ifndef r
  #define r 0.05
#endif


namespace quantpy {

  namespace instruments {

    namespace EuropeanOption {

      namespace AbsoluteCEV_tests {


        // Test 1
        bool test_price1() {

          double sigma = 0.2;
          double vol = sigma * S;


          AbsoluteCEV pricer = AbsoluteCEV<double>(r, 20., vol, true);

          double correctPrice = 1.101;
          double foundPrice   = pricer(S, 4. / 12.);

          DEBUG("Found price: ", foundPrice);

          return ( fabs(correctPrice - foundPrice) < TEST_TOL );

        }


        // Test 2
        bool test_price2() {

          double sigma = 0.3;
          double vol = sigma * S;


          AbsoluteCEV pricer = AbsoluteCEV<double>(r, 20., vol, true);

          double correctPrice = 1.581;
          double foundPrice   = pricer(S, 4. / 12.);

          DEBUG("Found price: ", foundPrice);

          return ( fabs(correctPrice - foundPrice) < TEST_TOL );

        }


        // Test 3
        bool test_price3() {

          double sigma = 0.4;
          double vol = sigma * S;


          AbsoluteCEV pricer = AbsoluteCEV<double>(r, 20., vol, true);

          double correctPrice = 2.084;
          double foundPrice   = pricer(S, 4. / 12.);

          DEBUG("Found price: ", foundPrice);

          return ( fabs(correctPrice - foundPrice) < TEST_TOL );

        }


        // Test 4
        bool test_price4() {

          double sigma = 0.2;
          double vol = sigma * S;


          AbsoluteCEV pricer = AbsoluteCEV<double>(r, 15., vol, true);

          double correctPrice = 5.472;
          double foundPrice   = pricer(S, 7. / 12.);

          DEBUG("Found price: ", foundPrice);

          return ( fabs(correctPrice - foundPrice) < TEST_TOL );

        }


        // Test 5
        bool test_price5() {

          double sigma = 0.3;
          double vol = sigma * S;


          AbsoluteCEV pricer = AbsoluteCEV<double>(r, 15., vol, true);

          double correctPrice = 5.725;
          double foundPrice   = pricer(S, 7. / 12.);

          DEBUG("Found price: ", foundPrice);

          return ( fabs(correctPrice - foundPrice) < TEST_TOL );

        }


        // Test 6
        bool test_price6() {

          double sigma = 0.4;
          double vol = sigma * S;


          AbsoluteCEV pricer = AbsoluteCEV<double>(r, 15., vol, true);

          double correctPrice = 6.194;
          double foundPrice   = pricer(S, 7. / 12.);

          DEBUG("Found price: ", foundPrice);

          return ( fabs(correctPrice - foundPrice) < TEST_TOL );

        }


      }

    }

  }

}


int main() {

  quantpy::cpp::logging::verbosity(5);

  quantpy::cpp::testing::TestSuite AbsoluteCEVTests = quantpy::cpp::testing::TestSuite("AbsoluteCEV.hpp");

  AbsoluteCEVTests.addTest(quantpy::instruments::EuropeanOption::AbsoluteCEV_tests::test_price1);
  AbsoluteCEVTests.addTest(quantpy::instruments::EuropeanOption::AbsoluteCEV_tests::test_price2);
  AbsoluteCEVTests.addTest(quantpy::instruments::EuropeanOption::AbsoluteCEV_tests::test_price3);
  AbsoluteCEVTests.addTest(quantpy::instruments::EuropeanOption::AbsoluteCEV_tests::test_price4);
  AbsoluteCEVTests.addTest(quantpy::instruments::EuropeanOption::AbsoluteCEV_tests::test_price5);
  AbsoluteCEVTests.addTest(quantpy::instruments::EuropeanOption::AbsoluteCEV_tests::test_price6);

  return (int)AbsoluteCEVTests.runTests();

}