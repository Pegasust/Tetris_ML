#pragma once

#ifdef WINDOWS_DEFINED
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

#ifdef WINDOWS_DEFINED
namespace std_filesystem = std::filesystem;
#else
namespace std_filesystem = std::experimental::filesystem;
#endif