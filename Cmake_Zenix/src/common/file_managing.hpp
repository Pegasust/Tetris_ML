#pragma once

#ifdef WINDOWS_DEFINED
#include <filesystem>
#else
#include <experimental/filesystem>
#endif
#include <fstream>
#include <string>
#include <iostream>

#ifdef WINDOWS_DEFINED
namespace std_filesystem = std::filesystem;
#else
namespace std_filesystem = std::experimental::filesystem;
#endif

/* Gets wanted fname, spit out new_path such that it is similar to fname.
 * This implementation keeps adding number to the back until there is no
 * longer such file exists 
 */
inline void next_available_filename(const std::string& fname, std::string& new_path) {
    if (std_filesystem::exists(fname) || std_filesystem::exists(fname + ".txt")) {
        unsigned int i = 0;
        while (std_filesystem::exists((new_path = fname + std::to_string(i++)) + ".txt")) {
        }
    } else {
        new_path = fname;
    }
    new_path += ".txt";
}