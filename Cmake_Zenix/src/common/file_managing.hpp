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
namespace Common {
namespace PathConsts{
/* Gets wanted fname, spit out new_path such that it is similar to fname.
 * This implementation keeps adding number to the back until there is no
 * longer such file exists. Also appends .txt at the back.
 * Passed in fname must not have extension.
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

inline bool create_directory_if_not_existed(const std::string& directory_path) {
    return std_filesystem::create_directory(directory_path);
}

inline std::fstream create_file(const std::string& path) {
    return std::fstream(path, std::ios::in | std::ios::out | std::ios::trunc);
}

} // namespace PathConsts
} // namespace Common::Paths