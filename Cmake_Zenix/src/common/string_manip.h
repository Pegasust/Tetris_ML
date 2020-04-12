/*
 * Contains functions that converts strings to double and
 * double to strings.
 */

#pragma once
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include "zmath.h"

namespace Common {
constexpr int RYU_BUFFER_SIZE = 20;
// The max length that a strcpy operation would handle before it attempts to timeout.
// This prevents memory overflow/corruption.
constexpr int MAX_STR_CPY = 4096;
/*
 * Converts a double value to a very precise string
 * that contains all of possible digits. Although
 * this produces quite precise string of double,
 * it takes so much CPU cycles and sometimes
 * output unnecessarily long strings compared to
 * ryu's
 */
std::string precise_to_string(double d);

/* Allocates a static char[RYU_BUFFER_SIZE] upon first call.
 * Not thread-safe.
*/
std::string ryu_d2s(const double& d);
// Prefer this method over non-buffered one
// because this doesn't allocate new char buffers
inline const char* ryu_d2s_buffered(const double& d, char* s) {
    d2s_buffered(d, s);
    return s;
}

// Returns a char buffer. Does allocate more memory
// but does not deallocate.
inline char* ryu_d2cstr(const double& d) {
    return d2s(d);
}

inline double ryu_s2d(const std::string& str) {
    double retval;
    s2d(str.data(), &retval);
    return retval;
}

inline double ryu_cstr2d(char* str) {
    double retval;
    s2d(str, &retval);
    return retval;
}
template <typename IntegralType>
std::stringstream decimal2hex_sstream(const IntegralType& num) {
    std::stringstream sstream;
    sstream << std::hex << num;
    return sstream;
}
template <typename IntegralType>
std::string decimal2hex_str(const IntegralType& num) {
    return decimal2hex_sstream<IntegralType>(num).str();
}

template <typename IntegralType>
IntegralType hex_str2decimal(const char* const string) {
    IntegralType retval;
    std::stringstream sstream;
    sstream << std::hex << string;
    sstream >> retval;
    return retval;
}

template <typename IntegralType>
IntegralType hex_str2decimal(const std::string& string) {
    return hex_str2decimal<IntegralType>(string.c_str());
}

// crack_atof gotten from http://coliru.stacked-crooked.com/a/2e28f0d71f47ca5e
// Turned out, crack_atof is too low in precision and has too many
// abstraction through double operator*.
constexpr double pow10(int n) {
    double ret = 1.0;
    double r = 10.0;
    if (n < 0) {
        n = -n;
        r = 0.1;
    }

    while (n) {
        if (n & 1) {
            ret *= r;
        }
        r *= r;
        n >>= 1;
    }
    return ret;
}

/*
 * Hacky algorithm that parses char array to double value
 * Questionable precision. This algorithm is the bottleneck
 * for accuracy, however.
 */
double crack_atof(const char* num);

double crack_atof(const char* num, const char* const end);
// end crack_atof

// https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string

/*
 * Splits string s into tokens and add it to result.
 * This method can only work with single delimiter.
 */
template <typename Out>
void split(const std::string& s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}
/*
 * Splits string s into tokens.
 * This method can only work with single delimiter.
 */
std::vector<std::string> split(const std::string& s, char delim);
#ifdef WINDOWS_DEFINED
#define STR_N_CPY(dest_ptr, start_ptr, max_len) strcpy_s(dest_ptr,max_len,start_ptr)
#else
#define STR_N_CPY(dest_ptr, start_ptr, max_len) strcpy(dest_ptr, max_len)
#endif
#define STR_CPY(dest_ptr, start_ptr) STR_N_CPY(dest_ptr, start_ptr, Common::MAX_STR_CPY)
//#define STR_APPEND(str1, str2) str1 str2
#define STR_CONCAT(str1, str2) str1 str2
} // namespace Common