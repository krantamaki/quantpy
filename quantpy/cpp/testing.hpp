/**
 * @file testing.hpp
 * @author Kasper Rantamäki
 * @date 2026-01-12
 */

#ifndef TESTING_HPP
#define TESTING_HPP


#include <vector>
#include <string>
#include <functional>
#include <chrono>
#include <exception>
#include <filesystem>

#include "logging.hpp"
#include "utils.hpp"


namespace quantpy {

  namespace cpp {

    namespace testing {


      /**
       * @brief Test suite for holding and running test functions
       */
      class TestSuite {

        protected:

          /**
           * @brief Alias for the test functions
           * @details Test functions are assumed to be independent of the test suite and take no parameters. The test function 
           * should return a boolean value with 'true' meaning that the test is passed and 'false' meaning the test failed
           */
          using testFunction = std::function<bool()>;


          /**
           * @struct testStruct
           * @brief Struct for holding individual test info
           * @var testStruct::test       The test function itself
           * @var testStruct::submodule  The name of the submodule
           * @var testStruct::testname   The name of the test
           */
          typedef struct {
            testFunction test;
            std::string submodule;
            std::string testname;
          } testStruct;


          std::vector<testStruct> testStructs;      /**< The list of function references */
          std::string testSuiteName = "undefined";  /**< The name of the test suite */

        
        public:

          /**
           * @brief Default constructor. Leaves the name of the test suite undefined
           */
          TestSuite(void) { }


          /**
           * @brief Constructor that allows specifying the name of the test suite
           * @param name  Constant string pointer with the name of the test suite
           */
          TestSuite(const std::string& name) {

            testSuiteName = name;

          }


          /**
           * @brief Method for adding a function pointer to the test suite
           * @param function   The test function to be added to the suite
           * @param submodule  The name of the submodule to which the test belongs. Defaults to 'undefined'
           * @param testname   The name of the test. Defaults to 'undefined'
           * @returns          True if the test was added successfully
           */
          bool addTest(testFunction function, std::string submodule = "undefined", std::string testname = "undefined") {

            testStruct newTest;

            newTest.test = function;
            newTest.submodule = submodule;
            newTest.testname = testname;

            testStructs.push_back(newTest);
            return true;

          }


          /**
           * @brief Method that gives the number of tests currently in the suite
           * @returns The number of tests in the suite
           */
          int numTests() {

            return testStructs.size();

          }


          /**
           * @brief Method for running the tests in the test suite
           * @param showTestInfo  Boolean flag telling if the test submodule and name gets printed
           * @returns             True if all of the tests pass, false otherwise
           */
          bool runTests(bool showTestInfo = false) {

            INFO("Running tests for: ", testSuiteName);

            if ( this->numTests() == 0 ) {
              INFO("No tests defined");
              return true;
            }

            int countPassed = 0;
          
            for (int i = 0; i < this->numTests(); i++) {
              testStruct test_i = testStructs[i];
              bool passed;
              int milliseconds;

              std::string infoString = "";
              if ( showTestInfo ) {
                infoString = utils::formString(" ", test_i.submodule, "|", test_i.testname);
              }

              try {
                const auto start = std::chrono::high_resolution_clock::now();
                passed = test_i.test();
                const auto end = std::chrono::high_resolution_clock::now();

                const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                milliseconds = (int)duration.count();
              }
              catch (std::exception& e) {  // Note, doesn't catch hardware exceptions like segmentation fault
                INFO("Test ", i + 1, infoString, " - ERROR (", e.what(), ")\n");
                continue;
              }

              if ( passed ) {
                countPassed++;
                INFO("Test ", i + 1, infoString, " - PASSED (time taken ", milliseconds, " ms)");
              }
              else {
                INFO("Test ", i + 1, infoString, " - FAILED (time taken ", milliseconds, " ms)");
              }
            }

            return ( countPassed == this->numTests() );

          }

      };


    }

  }

}


#endif