/**
 * @file utils_tests.cpp
 * @brief Definition of tests for generic utility functions
 * @details Compile in the main quantpy directory with:
 * > g++ -std=c++17 -mavx -fopenmp -Wall quantpy/cpp/tests/utils_tests.cpp -lm -o utils_tests.o
 * Run with:
 * > ./utils_tests.o
 * @author Kasper Rantamäki
 * @date 2026-01-12
 */


#include <string>
#include <map>
#include <vector>

#include "../utils.hpp"
#include "../testing.hpp"
#include "../logging.hpp"


namespace quantpy {

  namespace cpp {

    namespace utils_tests {


      // Test 1
      bool test_formString1() {

        std::string correctString = "Hello World!";
        std::string foundString = utils::formString("Hello ", "World!");

        DEBUG("Found string: ", foundString);

        bool passed = (correctString == foundString);

        return passed;

      }


      // Test 2
      bool test_formString2() {

        std::string correctString = "1 + 1 = 2";
        std::string foundString = utils::formString(1, " + ", 1, " = ", 2);

        DEBUG("Found string: ", foundString);

        bool passed = (correctString == foundString);

        return passed;

      }


      // Test 3
      bool test_numTokens1() {

        std::string testString = "   1  2  3  4  ";
        int correctNum = 4;
        int foundNum = utils::numTokens(testString);

        DEBUG("Found number: ", foundNum);

        bool passed = (correctNum == foundNum);

        return passed;

      }


      // Test 4
      bool test_numTokens2() {

        std::string testString = "|||1|2|3||4||";
        int correctNum = 4;
        int foundNum = utils::numTokens(testString, '|');

        DEBUG("Found number: ", foundNum);

        bool passed = (correctNum == foundNum);

        return passed;

      }


      // Test 5
      bool test_mapKeys() {

        std::vector<int> correctVector = {1, 2, 3, 4};
        std::map<int, std::string> testMap = {{1, "Hello"}, {2, " "}, {3, "World"}, {4, "!"}};
        std::vector<int> foundVector = utils::mapKeys(testMap);

        bool passed = true;

        if ( correctVector.size() != foundVector.size() ) {
          passed = false;
        }

        for (int i = 0; i < (int)foundVector.size(); i++) {
          if ( correctVector[i] != foundVector[i] ) {
            passed = false;
          }
        }

        return passed;

      }


      // Test 6
      bool test_mapVals() {

        std::vector<std::string> correctVector = {"Hello", " ", "World", "!"};
        std::map<int, std::string> testMap = {{1, "Hello"}, {2, " "}, {3, "World"}, {4, "!"}};
        std::vector<std::string> foundVector = utils::mapVals(testMap);

        bool passed = true;

        if ( correctVector.size() != foundVector.size() ) {
          passed = false;
        }

        for (int i = 0; i < (int)foundVector.size(); i++) {
          if ( correctVector[i] != foundVector[i] ) {
            passed = false;
          }
        }

        return passed;

      }


      // Test 7
      bool test_ceil() {

        int correctInt = 3;
        int numerator = 5;
        int denominator = 2;
        int foundInt = utils::ceil(numerator, denominator);

        DEBUG("Found number: ", foundInt);

        bool passed = (correctInt == foundInt);

        return passed;

      }


    }

  }

}


int main() {

  quantpy::cpp::logging::verbosity(3);

  quantpy::cpp::testing::TestSuite utilTests = quantpy::cpp::testing::TestSuite("utils.hpp");

  utilTests.addTest(quantpy::cpp::utils_tests::test_formString1);
  utilTests.addTest(quantpy::cpp::utils_tests::test_formString2);
  utilTests.addTest(quantpy::cpp::utils_tests::test_numTokens1);
  utilTests.addTest(quantpy::cpp::utils_tests::test_numTokens2);
  utilTests.addTest(quantpy::cpp::utils_tests::test_mapKeys);
  utilTests.addTest(quantpy::cpp::utils_tests::test_mapVals);
  utilTests.addTest(quantpy::cpp::utils_tests::test_ceil);

  return (int)utilTests.runTests();

}