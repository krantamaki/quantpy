/**
 * @file incompleteGamma_tests.cpp
 * @details Compile in the main quantpy directory with:
 * > g++ -std=c++17 -mavx -fopenmp -Wall quantpy/math/special/tests/incompleteGamma_tests.cpp -lm -o incompleteGamma_tests.o
 * Run with:
 * > ./incompleteGamma_tests.o
 * @author Kasper Rantamäki
 * @date 2026-01-19
 */

#include <cmath>

#include "../incompleteGamma.hpp"
#include "../../../cpp/testing.hpp"
#include "../../../cpp/logging.hpp"


#ifndef TEST_TOL
  #define TEST_TOL 1e-2
#endif


namespace quantpy {

  namespace math {

    namespace special {

      namespace incompleteGamma_tests {


        // Test 1
        bool test_lowerIncompleteGamma1() {

          double correctValue = 0.8008517265;
          double foundValue   = lowerIncompleteGamma<double>(2., 3.);

          DEBUG("Found value: ", foundValue);

          return ( fabs(correctValue - foundValue) < TEST_TOL );

        }


        // Test 2
        bool test_upperIncompleteGamma1() {

          double correctValue = 0.1991482735;
          double foundValue   = upperIncompleteGamma<double>(2., 3.);

          DEBUG("Found value: ", foundValue);

          return ( fabs(correctValue - foundValue) < TEST_TOL );

        }


        // Test 3
        bool test_lowerIncompleteGamma2() {

          double correctValue = 5.509407502;
          double foundValue   = lowerIncompleteGamma<double>(4., 7.);

          DEBUG("Found value: ", foundValue);

          return ( fabs(correctValue - foundValue) < TEST_TOL );

        }


        // Test 4
        bool test_upperIncompleteGamma2() {

          double correctValue = 0.4905924975;
          double foundValue   = upperIncompleteGamma<double>(4., 7.);

          DEBUG("Found value: ", foundValue);

          return ( fabs(correctValue - foundValue) < TEST_TOL );

        }


      }

    }

  }

}


int main() {

  quantpy::cpp::logging::verbosity(5);

  quantpy::cpp::testing::TestSuite incompleteGammaTests = quantpy::cpp::testing::TestSuite("incompleteGamma.hpp");

  incompleteGammaTests.addTest(quantpy::math::special::incompleteGamma_tests::test_lowerIncompleteGamma1);
  incompleteGammaTests.addTest(quantpy::math::special::incompleteGamma_tests::test_upperIncompleteGamma1);
  incompleteGammaTests.addTest(quantpy::math::special::incompleteGamma_tests::test_lowerIncompleteGamma2);
  incompleteGammaTests.addTest(quantpy::math::special::incompleteGamma_tests::test_upperIncompleteGamma2);


  return (int)incompleteGammaTests.runTests();

}