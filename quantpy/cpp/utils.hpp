/**
 * @file utils.hpp
 * @brief Generic C++ utility functions. 
 * @details Independent of other utility headers
 * @author Kasper Rantamäki
 * @date 2026-01-10
 */

#ifndef UTILS_HPP
#define UTILS_HPP


#include <string>
#include <sstream>
#include <vector>
#include <map>


namespace utils {


  /**
   * @brief Function that returns the used C++ version
   * @returns The C++ version as a std::string object
   */
  inline std::string getcppStandard() {

    if (__cplusplus == 202101L) return "C++23";
    else if (__cplusplus == 202002L) return "C++20";
    else if (__cplusplus == 201703L) return "C++17";
    else if (__cplusplus == 201402L) return "C++14";
    else if (__cplusplus == 201103L) return "C++11";
    else if (__cplusplus == 199711L) return "C++98";
    else return "C++??";

  }


  /**
   * @brief Function that forms a string from the given (arbitrary) arguments. 
   * @details Note that the function does not add any whitespaces so these are up to the user
   * @tparam Args...  Variadic argument type
   * @param args      The arguments from which the string is formed
   * @returns         A std::string object formed from the given arguments
   */
  template<class... Args>
  std::string formString(Args... args) {

    std::ostringstream msg;
    (msg << ... << args);

    return msg.str();

  }


  /**
   * @brief Function that counts the number of tokens in a string as divided by a given (character) delimeter
   * @param str    The string from which the tokens are to be counted
   * @param delim  The delimeter used to separate the tokens. Defaults to a single space ' '
   * @returns      The number of tokens as an integer
   */
  inline int numTokens(const std::string& str, char delim = ' ') {

    std::istringstream stream(str);
    std::string token;
    int count = 0;

    while (std::getline(stream, token, delim)) {
      if (token != "") {
        count++;
      }
    }

    return count;

  }


  /**
   * @brief Template function that retrieves all keys in a std::map
   * @tparam keyType  The type for the keys in the map
   * @tparam valType  The type for the values in the map
   * @param map       The map from the keys are to be retrieved
   * @returns         The keys in a std::vector
   */ 
  template<typename keyType, typename valType>
  std::vector<keyType> mapKeys(std::map<keyType, valType> map) {

    std::vector<keyType> ret;
    for (const auto& [key, val] : map) {
        ret.push_back(key);
    }

    return ret;

  }


  /**
   * @brief Template function that retrieves all values in a std::map
   * @tparam keyType  The type for the keys in the map
   * @tparam valType  The type for the values in the map
   * @param map       The map from the keys are to be retrieved
   * @returns         The values in a std::vector
   */ 
  template<typename keyType, typename valType>
  std::vector<valType> mapVals(std::map<keyType, valType> map) {

    std::vector<valType> ret;
    for (const auto& [key, val] : map) {
        ret.push_back(val);
    }

    return ret;

  }


  /** 
   * @brief Function for "dividing up" two integers
   * @param numerator    The numerator
   * @param denominator  The denominator
   * @returns            The fraction rounded up to the nearest integer value
   */
  inline int ceil(int numerator, int denominator) {

    return (numerator + denominator - 1) / denominator;

  }


}


#endif