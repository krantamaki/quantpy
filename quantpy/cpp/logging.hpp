/**
 * @file logging.hpp
 * @brief Logging utility functions and macros
 * @details Dependent on the 'utils.hpp' header file
 * @author Kasper Rantamäki
 * @date 2026-01-10
 */

#ifndef LOGGING_HPP
#define LOGGING_HPP


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "utils.hpp"


/**
 * @brief The base verbosity level
 * @todo Instead of hardcoding this it should be alterable at least at compile time
 */
#define BASE_VERBOSITY 3


// Define macros for easier usage of the logging functions

/**
 * @brief Error message macro
 * @details Note that the function uses the __PRETTY_FUNCTION__ macro which is GCC specific.
 * Unlike other logging macros the error message throws an exception
 * @see _errorMsg() for more info
 * @param ...  Variadic arguments from which the error message is formed
 * @returns    Void
 */
#define ERROR(...) { logging::_errorMsg(__FILE__, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__); }


/**
 * @brief Warning message macro
 * @see _warningMsg() for more info
 * @param ...  Variadic arguments from which the warning message is formed
 * @returns    Void
 */
#define WARNING(...) { logging::_warningMsg(__func__, __VA_ARGS__); }


/**
 * @brief Info message macro
 * @see _infoMsg() for more info
 * @param ...  Variadic arguments from which the info message is formed
 * @returns    Void
 */
#define INFO(...) { logging::_infoMsg(__func__, __VA_ARGS__); }


/**
 * @brief Low priority info message macro
 * @see _lowPriorityMsg() for more info
 * @param ...  Variadic arguments from which the low priority info message is formed
 * @returns    Void
 */
#define LOWPRIORITY(...) { logging::_lowPriorityMsg(__func__, __VA_ARGS__); }


/**
 * @brief Debug message macro
 * @see _debugMsg() for more info
 * @param ...  Variadic arguments from which the debug message is formed
 * @returns    Void
 */
#define DEBUG(...) { logging::_debugMsg(__func__, __VA_ARGS__); }


namespace quantpy {

  namespace cpp {

    namespace logging {


      /**
       * @brief Function that defines and returns the verbosity level
       * @details Verbosity has 5 levels:
       *   1: Error messages
       *   2: 1 and warning messages
       *   3: 2 and base info messages
       *   4: 3 and low priority info messages
       *   5: Everything (i.e. 4 and debug messages)
       * If an improper level is passed will default to level 5.
       * Note that the verbosity can only be set once during the program execution
       * @param _verbosity  The wanted verbosity level. Defaults to the defined base verbosity level
       * @returns           The active verbosity level
       */
      inline int verbosity(int _verbosity = BASE_VERBOSITY) {

        static bool firstCall = true;
        static int set_verbosity = BASE_VERBOSITY;

        if ( firstCall ) {
          firstCall = false;

          if ( _verbosity < 0 || _verbosity > 5 ) {
            set_verbosity = 5;
            return set_verbosity;
          }
          set_verbosity = _verbosity;
        }

        return set_verbosity;

      }


      /**
       * @brief Function that generates and throws a more descriptive (runtime) error
       * @details The error message consists of the filename, function name, linenumber and the message itself
       * @see ERROR() macro for usage
       * @tparam Args  The variadic argument type
       * @param file   Pointer to a filename
       * @param func   Pointer to a function name
       * @param line   The line number
       * @param args   The variadic arguments
       * @returns      Void
       */
      template <class... Args>
      void _errorMsg(const char* file, const char* func, int line, Args... args) {

        std::ostringstream msgStream;

        msgStream << "\n" << "ERROR: In file " << file << " at function " << func << " on line " << line << " : " << utils::formString(args);
        std::string msgString = msgStream.str();
        std::cout << msgString << "\n\n" << std::endl;

        throw std::runtime_error(msgString);

      }


      /**
       * @brief Function that forms and prints a warning message
       * @details The warning is printed only if the verbosity level is set to greater than 1
       * @see WARNING() macro for usage
       * @tparam Args  The variadic argument type
       * @param func   Pointer to a function name
       * @param args   The variadic arguments
       * @returns      Void
       */
      template <class... Args>
      void _warningMsg(const char* func, Args... args) {

        if (verbosity() > 1) {
          std::cout << func << ": " << "WARNING! " << utils::formString(args) << std::endl;
        }
        
      }


      /**
       * @brief Function that forms and prints an information message
       * @details The info message is printed only if the verbosity level is set to greater than 2
       * @see INFO() macro for usage
       * @tparam Args  The variadic argument type
       * @param func   Pointer to a function name
       * @param args   The variadic arguments
       * @returns      Void
       */
      template <class... Args>
      void _infoMsg(const char* func, Args... args) {

        if (verbosity() > 2) {
          std::cout << func << ": " << utils::formString(args) << std::endl;
        }

      }


      /**
       * @brief Function that forms and prints a lower priority information message
       * @details The low priority message is printed only if the verbosity level is set to greater than 3
       * @see LOWPRIORITY() macro for usage
       * @tparam Args  The variadic argument type
       * @param func   Pointer to a function name
       * @param args   The variadic arguments
       * @returns      Void
       */
      template <class... Args>
      void _lowPriorityMsg(const char* func, Args... args) {

        if (verbosity() > 3) {
          std::cout << func << ": " << utils::formString(args) << std::endl;
        }

      }


      /**
       * @brief Function that forms and prints a debug message
       * @details The debug message is printed only if the verbosity level is set to greater than 4
       * @see DEBUG() macro for usage
       * @tparam Args  The variadic argument type
       * @param func   Pointer to a function name
       * @param args   The variadic arguments
       * @returns      Void
       */
      template <class... Args>
      void _debugMsg(const char* func, Args... args) {

        if (verbosity() > 4) {
          std::cout << func << ": " << "DEBUG - " << utils::formString(args) << std::endl;
        }

      }


    }

  }

}


#endif