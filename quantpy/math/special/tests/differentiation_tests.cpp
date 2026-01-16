/**
 * @file differentiation_tests.cpp
 * @details Compile in the main quantpy directory with:
 * > g++ -std=c++17 -mavx -fopenmp -Wall quantpy/math/special/tests/differentiation_tests.cpp -lm -o differentiation_tests.o
 * Run with:
 * > ./differentiation_tests.o
 * @author Kasper Rantamäki
 * @date 2026-01-16
 */

#include <cmath>

#include "../differentiation.hpp"
#include "../../../cpp/testing.hpp"
#include "../../../cpp/logging.hpp"


#ifndef TEST_TOL
  #define TEST_TOL 1e-3
#endif


namespace quantpy {

  namespace math {

    namespace special {

      namespace differentiation_tests {


        // Test 1
        bool test_firstForwardDiff1() {
          
          auto func = [](double x) { return 2. * x * x + 2; };

          double correctDerv = 6.;
          double foundDerv   = quantpy::math::special::differentiation::firstForwardDiff<double>(func, 1.5);

          DEBUG("Found derivative: ", foundDerv);

          return ( fabs(correctDerv - foundDerv) < TEST_TOL );

        }


        // Test 2
        bool test_firstForwardDiff2() {
          
          return true;

        }


        // Test 3
        bool test_firstBackwardDiff1() {

          auto func = [](double x) { return 2. * x * x + 2; };

          double correctDerv = 6.;
          double foundDerv   = quantpy::math::special::differentiation::firstBackwardDiff<double>(func, 1.5);

          DEBUG("Found derivative: ", foundDerv);

          return ( fabs(correctDerv - foundDerv) < TEST_TOL );

        }


        // Test 4
        bool test_firstBackwardDiff2() {

          return true;

        }


        // Test 5
        bool test_firstCentralDiff1() {

          auto func = [](double x) { return 2. * x * x + 2; };

          double correctDerv = 6.;
          double foundDerv   = quantpy::math::special::differentiation::firstCentralDiff<double>(func, 1.5);

          DEBUG("Found derivative: ", foundDerv);

          return ( fabs(correctDerv - foundDerv) < TEST_TOL );

        }


        // Test 6
        bool test_firstCentralDiff2() {

          return true;

        }


        // Test 7
        bool test_secondForwardDiff1() {

          auto func = [](double x) { return 0.5 * x * x * x + 2; };

          double correctDerv = 4.5; 
          double foundDerv   = quantpy::math::special::differentiation::secondForwardDiff<double>(func, 1.5);

          DEBUG("Found derivative: ", foundDerv);

          return ( fabs(correctDerv - foundDerv) < TEST_TOL );

        }


        // Test 8
        bool test_secondForwardDiff2() {

          return true;

        }


        // Test 9
        bool test_secondBackwardDiff1() {

          auto func = [](double x) { return 0.5 * x * x * x + 2; };

          double correctDerv = 4.5; 
          double foundDerv   = quantpy::math::special::differentiation::secondBackwardDiff<double>(func, 1.5);

          DEBUG("Found derivative: ", foundDerv);

          return ( fabs(correctDerv - foundDerv) < TEST_TOL );

        }


        // Test 10
        bool test_secondBackwardDiff2() {

          return true;

        }


        // Test 11
        bool test_secondCentralDiff1() {

          auto func = [](double x) { return 0.5 * x * x * x + 2; };

          double correctDerv = 4.5; 
          double foundDerv   = quantpy::math::special::differentiation::secondCentralDiff<double>(func, 1.5);

          DEBUG("Found derivative: ", foundDerv);

          return ( fabs(correctDerv - foundDerv) < TEST_TOL );

        }


        // Test 12
        bool test_secondCentralDiff2() {

          return true;

        }


        // Test 13
        bool test_nthForwardDiff1() {

          auto func = [](double x) { return 0.1 * x * x * x * x + 2; };

          double correctDerv = 3.6;
          double foundDerv   = quantpy::math::special::differentiation::nthForwardDiff<double>(func, 3, 1.5);

          DEBUG("Found derivative: ", foundDerv);

          return ( fabs(correctDerv - foundDerv) < TEST_TOL );

        }


        // Test 14
        bool test_nthForwardDiff2() {

          return true;

        }


        // Test 15
        bool test_nthBackwardDiff1() {

          auto func = [](double x) { return 0.1 * x * x * x * x + 2; };

          double correctDerv = 3.6;
          double foundDerv   = quantpy::math::special::differentiation::nthBackwardDiff<double>(func, 3, 1.5);

          DEBUG("Found derivative: ", foundDerv);

          return ( fabs(correctDerv - foundDerv) < TEST_TOL );

        }


        // Test 16
        bool test_nthBackwardDiff2() {

          return true;

        }


        // Test 17
        bool test_nthCentralDiff1() {

          auto func = [](double x) { return 0.1 * x * x * x * x + 2; };

          double correctDerv = 3.6;
          double foundDerv   = quantpy::math::special::differentiation::nthCentralDiff<double>(func, 3, 1.5);

          DEBUG("Found derivative: ", foundDerv);

          return ( fabs(correctDerv - foundDerv) < TEST_TOL );

        }


        // Test 18
        bool test_nthCentralDiff2() {

          return true;

        }


      }

    }

  }

}


int main() {

  quantpy::cpp::logging::verbosity(3);

  quantpy::cpp::testing::TestSuite differentiationTests = quantpy::cpp::testing::TestSuite("differentiation.hpp");

  differentiationTests.addTest(quantpy::math::special::differentiation_tests::test_firstForwardDiff1);
  differentiationTests.addTest(quantpy::math::special::differentiation_tests::test_firstForwardDiff2);
  differentiationTests.addTest(quantpy::math::special::differentiation_tests::test_firstBackwardDiff1);
  differentiationTests.addTest(quantpy::math::special::differentiation_tests::test_firstBackwardDiff2);
  differentiationTests.addTest(quantpy::math::special::differentiation_tests::test_firstCentralDiff1);
  differentiationTests.addTest(quantpy::math::special::differentiation_tests::test_firstCentralDiff2);
  differentiationTests.addTest(quantpy::math::special::differentiation_tests::test_secondForwardDiff1);
  differentiationTests.addTest(quantpy::math::special::differentiation_tests::test_secondForwardDiff2);
  differentiationTests.addTest(quantpy::math::special::differentiation_tests::test_secondBackwardDiff1);
  differentiationTests.addTest(quantpy::math::special::differentiation_tests::test_secondBackwardDiff2);
  differentiationTests.addTest(quantpy::math::special::differentiation_tests::test_secondCentralDiff1);
  differentiationTests.addTest(quantpy::math::special::differentiation_tests::test_secondCentralDiff2);
  differentiationTests.addTest(quantpy::math::special::differentiation_tests::test_nthForwardDiff1);
  differentiationTests.addTest(quantpy::math::special::differentiation_tests::test_nthForwardDiff2);
  differentiationTests.addTest(quantpy::math::special::differentiation_tests::test_nthBackwardDiff1);
  differentiationTests.addTest(quantpy::math::special::differentiation_tests::test_nthBackwardDiff2);
  differentiationTests.addTest(quantpy::math::special::differentiation_tests::test_nthCentralDiff1);
  differentiationTests.addTest(quantpy::math::special::differentiation_tests::test_nthCentralDiff2);


  return (int)differentiationTests.runTests();

}