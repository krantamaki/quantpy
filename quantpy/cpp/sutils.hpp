/**
 * @file sutils.hpp
 * @brief String utility functions
 * @details Dependent on the 'logging.hpp' header file
 * @author Kasper Rantamäki
 * @date 2026-01-10
 */

#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP


#include <string>
#include <sstream>
#include <vector>
#include <cctype>

#include "logging.hpp"


namespace quantpy {

  namespace cpp {

    namespace sutils {


      /**
       * @brief Function that splits a string by a wanted (character) delimeter.
       * @details Doesn't count empty strings as tokens
       * @param str    Constant string pointer to the string to be split
       * @param delim  The delimeter by which the string is split. Defaults to a single space ' '
       * @returns      A std::vector object with the string parts
       */ 
      inline std::vector<std::string> split(const std::string& str, char delim = ' ') {

        std::istringstream stream(str);
        std::string token;

        std::vector<std::string> ret;

        while (std::getline(stream, token, delim)) {
          if (token != "") {
            ret.push_back(token);
          }
        }

        return ret;

      }


      /**
       * @brief Function that joins multiple strings together by a delimeter
       * @param strs   A std::vector of the strings to be joined
       * @param delim  The delimeter with which the string is joined. Defaults to a single space ' '
       * @returns      A std::string with the strings joined with the delimeter
       */
      inline std::string join(const std::vector<std::string> strs, char delim=' ') {

        std::ostringstream stream;
        stream << strs[0];

        std::vector<std::string> strs_slice = std::vector<std::string>(strs.begin() + 1, strs.end());

        for (const std::string& str : strs_slice) {
          stream << delim << str;
        }

        return stream.str();

      }


      /**
       * @brief Function that converts a given string to lower case
       * @param str  Constant string pointer to the string to be lower cased
       * @returns    A std::string object equivalent to the given string in lower case
       */
      inline std::string toLower(const std::string& str) {

        std::string ret;
        ret.reserve(str.size());
        for (int i = 0; i < (int)str.size(); i++) {
          char c = (char)tolower(str[i]);
          ret.push_back(c);
        }

        return ret;

      } 


      /**
       * @brief Function that converts a given string to upper case
       * @param str  Constant string pointer to the string to be upper cased
       * @returns    A std::string object equivalent to the given string in upper case
       */
      inline std::string toUpper(const std::string& str) {

        std::string ret;
        ret.reserve(str.size());
        for (int i = 0; i < (int)str.size(); i++) {
          char c = (char)toupper(str[i]);
          ret.push_back(c);
        }

        return ret;

      } 


      /**
       * @brief Function that removes the leading and trailing whitespaces from a string
       * @param str  Constant string pointer to the string to be trimmed
       * @returns    A std::string object equivalent to the given string without leading and trailing whitespaces
       */
      inline std::string trim(const std::string& str) {

        if (str == "") {
          ERROR("Cannot trim an empty string!");
        }

        // Find the index of the first non-whitespace character
        int start = -1;
        for (int i = 0; i < (int)str.size(); i++) {
          if (!isspace(str[i])) {
            start = i;
            break;
          } 
        }

        if (start == -1) {
          WARNING("Whitespace string trimmed!");
          return "";
        }

        // Find the index of the last non-whitespace character
        int end;
        for (int i = (int)str.size() - 1; i >= 0; i--) {
          if (!isspace(str[i])) {
            end = i;
            break;
          } 
        }

        return str.substr(start, end - start + 1);

      }


    }

  }

}


#endif