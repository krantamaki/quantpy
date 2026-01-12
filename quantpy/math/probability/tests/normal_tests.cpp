/**
 * @file rootFinding_tests.cpp
 * @brief Definition of tests for normal density functions
 * @details Compile in the main quantpy directory with:
 * > g++ -std=c++17 -mavx -fopenmp -Wall quantpy/math/probability/tests/normal_tests.cpp -lm -o normal_tests.o
 * Run with:
 * > ./normal_tests.o
 * @author Kasper Rantamäki
 * @date 2026-01-12
 */


#include <cmath>

#include "../normal.hpp"
#include "../../../cpp/testing.hpp"
#include "../../../cpp/logging.hpp"


#ifndef TEST_TOL
  #define TEST_TOL 1e-6
#endif


namespace quantpy {

  namespace math {

    namespace probability {

      namespace normal_tests {


        // Test 1
        bool test_pdf1() {

          double correctPDF = 0.3520653267;
          double foundPDF = normal::pdf<double>(0., 1., 0.5);

          DEBUG("Found PDF: ", foundPDF);

          return ( abs(correctPDF - foundPDF) < TEST_TOL );

        } 


        // Test 2
        bool test_pdf2() {

          double correctPDF = 0.2792879016;
          double foundPDF = normal::pdf<double>(1., sqrt(2), 1.2);

          DEBUG("Found PDF: ", foundPDF);

          return ( abs(correctPDF - foundPDF) < TEST_TOL );

        } 


        // Test 3
        bool test_cdf1() {

          double correctCDF = 0.691462461274013;
          double foundCDF = normal::cdf<double>(0., 1., 0.5);

          DEBUG("Found CDF: ", foundCDF);

          return ( abs(correctCDF - foundCDF) < TEST_TOL );
          
        } 


        // Test 4
        bool test_cdf2() {

          double correctCDF = 0.556231458009143;
          double foundCDF = normal::cdf<double>(1., sqrt(2), 1.2);

          DEBUG("Found CDF: ", foundCDF);

          return ( abs(correctCDF - foundCDF) < TEST_TOL );

        } 


        // Test 5
        bool test_moment1() {

          double correctMoment = 15.;
          double foundMoment = normal::moment<double>(0., 1., 6);

          DEBUG("Found moment: ", foundMoment);

          return ( abs(correctMoment - foundMoment) < TEST_TOL );

        } 


        // Test 6
        bool test_moment2() {

          double correctMoment = 120.;
          double foundMoment = normal::moment<double>(1., sqrt(2), 6);

          DEBUG("Found moment: ", foundMoment);

          return ( abs(correctMoment - foundMoment) < TEST_TOL );

        } 


      }

    }

  }

}


int main() {

  quantpy::cpp::logging::verbosity(3);

  quantpy::cpp::testing::TestSuite normalTests = quantpy::cpp::testing::TestSuite("normal.hpp");

  normalTests.addTest(quantpy::math::probability::normal_tests::test_pdf1);
  normalTests.addTest(quantpy::math::probability::normal_tests::test_pdf2);
  normalTests.addTest(quantpy::math::probability::normal_tests::test_cdf1);
  normalTests.addTest(quantpy::math::probability::normal_tests::test_cdf2);
  normalTests.addTest(quantpy::math::probability::normal_tests::test_moment1);
  normalTests.addTest(quantpy::math::probability::normal_tests::test_moment2);

  return (int)normalTests.runTests();

}