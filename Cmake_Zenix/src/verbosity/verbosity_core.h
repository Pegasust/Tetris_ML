#pragma once

#include <fstream>
#include <string>
#include "../common/file_managing.hpp"
#include <iostream>
#include "../common/assertion.hpp"
#include <cstdarg>

namespace Verbosity {
#define RELATIVE_LOG_ROOT "./logs/"
#define LOG_ROOT(x) (RELATIVE_LOG_ROOT x)
template <bool try_append_to_file = true> // If false, and name already exists, try to manipulate
                                          // the file name.
class VerbosityWriter {
public:
    typedef std::string (*Log_On_Destruct)();
    VerbosityWriter(const std::string& log_rooted_relative_file_path);
    VerbosityWriter(
        const std::string& log_rooted_relative_file_path, const Log_On_Destruct dying_log);
    ~VerbosityWriter();
    std::fstream output_stream;
    static constexpr std::size_t fmt_buffer_size = 64;

protected:
    const Log_On_Destruct dying_log;
    char format_out_str[fmt_buffer_size];

public:
    VerbosityWriter(const VerbosityWriter&) = delete;
    void operator=(const VerbosityWriter&) = delete;
    inline int format(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        int retval = vsnprintf(format_out_str, fmt_buffer_size, fmt, args);
        return retval;
    }
    inline int printf(const char* fmt, ...) { 
        va_list args;
        va_start(args, fmt);
        int retval = vsnprintf(format_out_str, fmt_buffer_size, fmt, args);
        output_stream << format_out_str;
    }
};
struct VerbosityCore {
public:
    static VerbosityWriter<false>& get_rt_logger();
    // static VerbosityWriter<false>& get_perf_logger()
    //{
    //	static VerbosityWriter<false> perf_log = VerbosityWriter<false>(LOG_ROOT("perf"));
    //	return perf_log;
    //}
};
inline void next_available_filename(const std::string& fname, std::string& new_path)
{
    if (std_filesystem::exists(fname) || std_filesystem::exists(fname + ".txt")) {
        unsigned int i = 0;
        while (std_filesystem::exists((new_path = fname + std::to_string(i++)) + ".txt")) {}
    }
    else {
        new_path = fname;
    }
    new_path += ".txt";
}

} // namespace Verbosity

#define GET_VERBOSITY_LOGGER Verbosity::VerbosityCore::get_rt_logger()
#define INITIALIZE_VERBOSITY_LOGGER GET_VERBOSITY_LOGGER
//#ifdef DEBUG_DEFINED
#define VERBOSITY_LOG(string)                                                           \
    do {                                                                                \
        Verbosity::VerbosityCore::get_rt_logger().output_stream << string << std::endl; \
    } while (0)
//#else
//#define VERBOSITY_LOG(string) \
//    Verbosity::VerbosityCore::get_rt_logger().output_stream << string << "\n";
//#endif
//#define PERFORMANCE_LOG(string)
// Verbosity::VerbosityCore::get_perf_logger().output_stream<<string<<std::endl

#include "verbosity_core.ipp"