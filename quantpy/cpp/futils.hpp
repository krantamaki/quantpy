/**
 * @file futils.hpp
 * @brief File utility functions. 
 * @details Dependent on the 'logging.hpp' and 'sutils.hpp' header files
 * @author Kasper Rantamäki
 * @date 2026-01-10
 */

#ifndef FILE_UTILS_HPP
#define FILE_UTILS_HPP


#include <iostream>
#include <string>
#include <sstream>

#include "logging.hpp"
#include "sutils.hpp"


namespace quantpy {

  namespace cpp {

    namespace futils {


      /**
       * @brief Function for reading the last line of a given text file
       * @details For more discussion on this approach see 
       * https://stackoverflow.com/questions/11876290/c-fastest-way-to-read-only-last-line-of-text-file
       * @param filepath  Constant string pointer with the path to the wanted file
       * @returns         The last line as a std::stringstream object
       */
      inline std::stringstream lastLine(const std::string& filepath) {

        std::ifstream _file(filepath);

        if ( !_file ) {
          ERROR("Couldn't open the given file!");
        }

        char c;
        _file.seekg(-1, std::ios_base::end);  // Go to last non EOF char

        _file.get(c);

        if ( c == '\n' ) {  // File ends with a newline char. Ignore it
          _file.seekg(-2, std::ios_base::cur);
          _file.get(c);
        }

        bool cont = true;
        while ( cont ) {

          if ( c == '\n' ) {  // End of the last line found
            _file.seekg(1, std::ios_base::cur);
            cont = false; 
          }

          // Continue
          _file.seekg(-2, std::ios_base::cur);
          _file.get(c);
        }

        std::string lastLine;
        std::getline(_file, lastLine);

        std::stringstream ret(lastLine);

        _file.close();

        return ret;

      }


      /**
       * @brief Function that defines and returns the path to the log file. 
       * @details If no path for a logfile is passed it will not change the stdout. 
       * In such cases the the function just returns the string "stdout". 
       * Note that the logfile can only be set once during the program execution.
       * @param _logfilePath  Constant string pointer with the path to the logfile. Defaults to empty string (stdout used)
       * @returns             The active path to the logfile
       */
      inline std::string logfile(const std::string& _logfilePath = "") {

        static bool firstCall = true;
        std::string logfilePath = "undefined";

        if ( firstCall ) {
          firstCall = false;

          if ( logfilePath == "" ) {
            logfilePath = "stdout";
            return logfilePath;
          }

          // Check if the directory in which the log is to be stored exists. If not create it
          std::vector<std::string> path_vector = sutils::split(_logfilePath, '/');
          std::string dir_path = sutils::join(std::vector<std::string>(path_vector.begin(), path_vector.end() - 1), '/');
          DEBUG(dir_path);
          
          //if ( !std::filesystem::exists(dir_path) ) {
          //  std::filesystem::create_directories(dir_path);
          //}

          // Write output into a log file
          freopen(_logfilePath.c_str(), "w", stdout);

          logfilePath = _logfilePath;
        }

        return logfilePath;

      }

      
    }    

  }

}


#endif
