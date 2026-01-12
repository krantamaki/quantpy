/**
 * @file rootFinding_tests.cpp
 * @brief Definition of tests for root finding functions
 * @details Compile in the main quantpy directory with:
 * > g++ -std=c++17 -mavx -fopenmp -Wall quantpy/math/optimization/tests/rootFinding_tests.cpp -lm -o rootFinding_tests.o
 * Run with:
 * > ./rootFinding_tests.o
 * @author Kasper Rantamäki
 * @date 2026-01-12
 */


#include <cmath>

#include "../rootFinding.hpp"
#include "../../../cpp/testing.hpp"
#include "../../../cpp/logging.hpp"


#ifndef TEST_TOL
  #define TEST_TOL 1e-6
#endif


namespace quantpy {

  namespace math {

    namespace optimization {

      namespace rootFinding_tests {


        // Test 1
        bool test_bisectionMethod1() {

          auto testFunc = [](double x) {
            return 2. * x - 5.;
          };

          double correctRoot = 2.5;
          double foundRoot = rootFinding::bisectionMethod<double>(testFunc, 0., 5.);

          DEBUG("Found root: ", foundRoot);

          return ( abs(correctRoot - foundRoot) < TEST_TOL );

        }


        // Test 2
        bool test_bisectionMethod2() {

          auto testFunc = [](double x) {
            return exp(0.5 * x) - 5.;
          };

          double correctRoot = 3.2188758249;
          double foundRoot = rootFinding::bisectionMethod<double>(testFunc, 0., 5.);

          DEBUG("Found root: ", foundRoot);

          return ( abs(correctRoot - foundRoot) < TEST_TOL );
          
        }


        // Test 3
        bool test_bisectionMethod3() {

          auto testFunc = [](double x) {
            return sqrt(2 * x) - 2.;
          };

          double correctRoot = 2.;
          double foundRoot = rootFinding::bisectionMethod<double>(testFunc, 0., 4.);

          DEBUG("Found root: ", foundRoot);

          return ( abs(correctRoot - foundRoot) < TEST_TOL );
          
        }


      }

    }

  }

}


int main() {

  quantpy::cpp::logging::verbosity(3);

  quantpy::cpp::testing::TestSuite rootFindingTests = quantpy::cpp::testing::TestSuite("rootFinding.hpp");

  rootFindingTests.addTest(quantpy::math::optimization::rootFinding_tests::test_bisectionMethod1);
  rootFindingTests.addTest(quantpy::math::optimization::rootFinding_tests::test_bisectionMethod2);
  rootFindingTests.addTest(quantpy::math::optimization::rootFinding_tests::test_bisectionMethod3);

  return (int)rootFindingTests.runTests();

}