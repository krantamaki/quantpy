/**
 * @file gamma_tests.cpp
 * @brief Definition of tests for the gamma density functions
 * @details Compile in the main quantpy directory with:
 * > g++ -std=c++17 -mavx -fopenmp -Wall quantpy/math/probability/tests/gamma_tests.cpp -lm -o gamma_tests.o
 * Run with:
 * > ./gamma_tests.o
 * @author Kasper Rantamäki
 * @date 2026-01-20
 */


#include <cmath>

#include "../gamma.hpp"
#include "../../../cpp/testing.hpp"
#include "../../../cpp/logging.hpp"


#ifndef TEST_TOL
  #define TEST_TOL 1e-3
#endif


namespace quantpy {

  namespace math {

    namespace probability {

      namespace gamma_tests {


        // Test 1
        bool test_pdf1() {

          double correctPDF = 0.1403738958;
          double foundPDF = gamma::pdf<double>(4., 1., 5.);

          DEBUG("Found PDF: ", foundPDF);

          return ( fabs(correctPDF - foundPDF) < TEST_TOL );

        } 


        // Test 2
        bool test_pdf2() {

          double correctPDF = 0.3553412695;
          double foundPDF = gamma::pdf<double>(20., 4., 5.);

          DEBUG("Found PDF: ", foundPDF);

          return ( fabs(correctPDF - foundPDF) < TEST_TOL );

        } 


        // Test 3
        bool test_cdf1() {

          double correctCDF = 0.7349740847;
          double foundCDF = gamma::cdf<double>(4., 1., 5.);

          DEBUG("Found CDF: ", foundCDF);

          return ( fabs(correctCDF - foundCDF) < TEST_TOL );
          
        } 


        // Test 4
        bool test_cdf2() {

          double correctCDF = 0.5297427332;
          double foundCDF = gamma::cdf<double>(20., 4., 5.);

          DEBUG("Found CDF: ", foundCDF);

          return ( fabs(correctCDF - foundCDF) < TEST_TOL );

        } 


      }

    }

  }

}


int main() {

  quantpy::cpp::logging::verbosity(3);

  quantpy::cpp::testing::TestSuite gammaTests = quantpy::cpp::testing::TestSuite("gamma.hpp");

  gammaTests.addTest(quantpy::math::probability::gamma_tests::test_pdf1);
  gammaTests.addTest(quantpy::math::probability::gamma_tests::test_pdf2);
  gammaTests.addTest(quantpy::math::probability::gamma_tests::test_cdf1);
  gammaTests.addTest(quantpy::math::probability::gamma_tests::test_cdf2);

  return (int)gammaTests.runTests();

}