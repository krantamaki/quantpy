/**
 * @file integration_tests.cpp
 * @brief Definition of tests for (all) integral functions
 * @details Compile in the main quantpy directory with:
 * > g++ -std=c++17 -mavx -fopenmp -Wall quantpy/math/integration/tests/integration_tests.cpp -lm -o integration_tests.o
 * Run with:
 * > ./integration_tests.o
 * @author Kasper Rantamäki
 * @date 2026-01-19
 */


#include <cmath>

#include "../simpson.hpp"
#include "../trapezoidal.hpp"
#include "../../../cpp/testing.hpp"
#include "../../../cpp/logging.hpp"


#ifndef TEST_TOL
  #define TEST_TOL 1e-2
#endif


namespace quantpy {

  namespace math {

    namespace integration {

      namespace integral_tests {


        // Test 1
        bool test_trapezoidal1() {

          auto func = [](double x) { return 3. * x * x + x - 3.; };

          double correctIntegrand = 11. / 2.;
          double foundIntegrand = trapezoidal<double>(func, 1., 2., 10000);

          DEBUG("Found integrand: ", foundIntegrand);

          return ( fabs(correctIntegrand - foundIntegrand) < TEST_TOL );

        } 


        // Test 2
        bool test_simpson1() {

          auto func = [](double x) { return 3. * x * x + x - 3.; };

          double correctIntegrand = 11. / 2.;
          double foundIntegrand = simpson<double>(func, 1., 2., 10000);

          DEBUG("Found integrand: ", foundIntegrand);

          return ( fabs(correctIntegrand - foundIntegrand) < TEST_TOL );

        } 


        // Test 3
        bool test_trapezoidal2() {

          auto func = [](double x) { return cos(3. * x); };

          double correctIntegrand = sin(15.) / 3.;
          double foundIntegrand = trapezoidal<double>(func, 0., 5., 10000);

          DEBUG("Found integrand: ", foundIntegrand);

          return ( fabs(correctIntegrand - foundIntegrand) < TEST_TOL );

        } 


        // Test 4
        bool test_simpson2() {

          auto func = [](double x) { return cos(3. * x); };

          double correctIntegrand = sin(15.) / 3.;
          double foundIntegrand = simpson<double>(func, 0., 5., 10000);

          DEBUG("Found integrand: ", foundIntegrand);

          return ( fabs(correctIntegrand - foundIntegrand) < TEST_TOL );

        } 


      }

    }

  }

}


int main() {

  quantpy::cpp::logging::verbosity(5);

  quantpy::cpp::testing::TestSuite integrationTests = quantpy::cpp::testing::TestSuite("integration");

  integrationTests.addTest(quantpy::math::integration::integral_tests::test_trapezoidal1);
  integrationTests.addTest(quantpy::math::integration::integral_tests::test_simpson1);
  integrationTests.addTest(quantpy::math::integration::integral_tests::test_trapezoidal2);
  integrationTests.addTest(quantpy::math::integration::integral_tests::test_simpson2);

  return (int)integrationTests.runTests();

}