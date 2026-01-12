/**
 * @file sutils_tests.cpp
 * @brief Definition of tests for string utility functions
 * @details Compile in the main quantpy directory with:
 * > g++ -std=c++17 -mavx -fopenmp -Wall quantpy/cpp/tests/sutils_tests.cpp -lm -o sutils_tests.o
 * Run with:
 * > ./sutils_tests.o
 * @author Kasper Rantamäki
 * @date 2026-01-12
 */


#include <string>
#include <vector>

#include "../sutils.hpp"
#include "../testing.hpp"
#include "../logging.hpp"


namespace quantpy {

  namespace cpp {

    namespace sutils_tests {


      // Test 1
      bool test_split1() {

        std::vector<std::string> correctVector = {"Hello", "World"};
        std::string testString = "   Hello   World";
        std::vector<std::string> foundVector = sutils::split(testString);

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


      // Test 2
      bool test_split2() {

        std::vector<std::string> correctVector = {"Hello", "World"};
        std::string testString = "|Hello||World|";
        std::vector<std::string> foundVector = sutils::split(testString, '|');

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


      // Test 3
      bool test_join1() {

        std::string correctString = "Hello World";
        std::vector<std::string> testVector = {"Hello", "World"};
        std::string foundString = sutils::join(testVector);

        DEBUG("Found string: ", foundString);

        bool passed = (correctString == foundString);

        return passed;

      }


      // Test 4
      bool test_join2() {

        std::string correctString = "Hello|World";
        std::vector<std::string> testVector = {"Hello", "World"};
        std::string foundString = sutils::join(testVector, '|');

        DEBUG("Found string: ", foundString);

        bool passed = (correctString == foundString);

        return passed;

      }


      // Test 5
      bool test_toLower() {

        std::string correctString = "hello world";
        std::string testString = "HelLO WoRLd";
        std::string foundString = sutils::toLower(testString);

        DEBUG("Found string: ", foundString);

        bool passed = (correctString == foundString);

        return passed;

      }


      // Test 6
      bool test_toUpper() {

        std::string correctString = "HELLO WORLD";
        std::string testString = "HelLO WoRLd";
        std::string foundString = sutils::toUpper(testString);

        DEBUG("Found string: ", foundString);

        bool passed = (correctString == foundString);

        return passed;

      }


      // Test 7
      bool test_trim1() {

        std::string correctString = "Hello World";
        std::string testString = "      Hello World   ";
        std::string foundString = sutils::trim(testString);

        DEBUG("Found string: ", foundString);

        bool passed = (correctString == foundString);

        return passed;

      }


      // Test 8
      bool test_trim2() {

        std::string correctString = "Hello World";
        std::string testString = "\t\tHello World\n";
        std::string foundString = sutils::trim(testString);

        DEBUG("Found string: ", foundString);

        bool passed = (correctString == foundString);

        return passed;

      }


    }
    
  } 

}


int main() {

  quantpy::cpp::logging::verbosity(3);

  quantpy::cpp::testing::TestSuite sutilTests = quantpy::cpp::testing::TestSuite("sutils.hpp");

  sutilTests.addTest(quantpy::cpp::sutils_tests::test_split1);
  sutilTests.addTest(quantpy::cpp::sutils_tests::test_split2);
  sutilTests.addTest(quantpy::cpp::sutils_tests::test_join1);
  sutilTests.addTest(quantpy::cpp::sutils_tests::test_join2);
  sutilTests.addTest(quantpy::cpp::sutils_tests::test_toLower);
  sutilTests.addTest(quantpy::cpp::sutils_tests::test_toUpper);
  sutilTests.addTest(quantpy::cpp::sutils_tests::test_trim1);
  sutilTests.addTest(quantpy::cpp::sutils_tests::test_trim2);

  return (int)sutilTests.runTests();

}
