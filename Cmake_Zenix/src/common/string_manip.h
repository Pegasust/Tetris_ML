/*
 * Contains functions that converts strings to double and
 * double to strings.
 */

#pragma once
#include <cstring>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <type_traits>
#include "zmath.h"

namespace Common {
// constexpr int RYU_BUFFER_SIZE = 20;
template <typename Float_Type = double>
constexpr std::size_t ryu_buffer()
{
    return std::is_same<Float_Type, float>::value ? 16 : 22;
}
// The max length that a strcpy operation would handle before it attempts to timeout.
// This prevents memory overflow/corruption.
constexpr int MAX_STR_CPY = 2048;
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
inline const char* ryu_d2s_buffered(const double& d, char* s)
{
    d2s_buffered(d, s);
    return s;
}

// Returns the length of the written string without null terminator.
// The returning cstring is null terminated
inline int ryu_d2s_buffered_n(const double& d, char* s)
{
    int retval = d2s_buffered_n(d, s);
    s[retval] = '\0';
    return retval;
}
template <typename Float_Type>
int ryu_float_2_cstr(const Float_Type f, char* const buffer);
//{
//    static_assert(
//        std::numeric_limits<Float_Type>::is_specialized, "Float_Type must be specialized");
//    static_assert(false, "No implementation for Float_Type defined in ryu");
//    return -1;
//}

// Returns a char buffer. Does allocate more memory
// but does not deallocate.
inline char* ryu_d2cstr(const double& d)
{
    return d2s(d);
}

inline double ryu_s2d(const std::string& str)
{
    double retval;
    s2d(str.data(), &retval);
    return retval;
}

inline double ryu_cstr2d(char* str)
{
    double retval;
    s2d(str, &retval);
    return retval;
}
template <typename IntegralType>
std::stringstream decimal2hex_sstream(const IntegralType& num)
{
    static_assert(std::numeric_limits<IntegralType>::is_integer, "IntegralType must be integer");
    std::stringstream sstream;
    sstream << std::hex << num;
    return sstream;
}
template <typename IntegralType>
inline std::string decimal2hex_str(const IntegralType& num)
{
    return decimal2hex_sstream<IntegralType>(num).str();
}

template <typename IntegralType>
IntegralType hex_str2decimal(const char* const string)
{
    IntegralType retval;
    std::stringstream sstream;
    sstream << std::hex << string;
    sstream >> retval;
    return retval;
}

template <typename IntegralType>
IntegralType hex_str2decimal(const std::string& string)
{
    return hex_str2decimal<IntegralType>(string.c_str());
}

// crack_atof gotten from http://coliru.stacked-crooked.com/a/2e28f0d71f47ca5e
// Turned out, crack_atof is too low in precision and has too many
// abstraction through double operator*.
constexpr double pow10(int n)
{
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

// Returns number of chars written (without null terminator) (>0) if successful, parses @param input
// into string at @param buffer.
template <typename Int_Type, typename Decayed = std::decay<Int_Type>::type>
int int_2_cstr(const Int_Type input, char* const buffer, const std::size_t buff_size = 0)
{
    static_assert(std::numeric_limits<Decayed>::is_integer, "Int_Type must be integral");
    constexpr const char* fmt_str = std::numeric_limits<Decayed>::is_signed ? "%i" : "%u";
    if (buff_size > 0) {
        return snprintf(buffer, buff_size, fmt_str, input);
    }
    return sprintf(buffer, fmt_str, input);
}

// Returns the length of a cstr (with null terminator) required to hold Int_Type.
template <typename Int_Type = int, typename Decayed = typename std::decay<Int_Type>::type>
constexpr std::size_t int_cstr_size(const Int_Type field = static_cast<Int_Type>(0))
{
    static_assert(std::numeric_limits<Decayed>::is_integer, "Int_Type must be integral");
    return std::numeric_limits<Decayed>::digits10 + 1 + std::numeric_limits<Decayed>::is_signed;
}
template <typename Decayed_Type, bool is_int>
struct _num_cstr_size_;
template <typename Decayed_Type>
struct _num_cstr_size_<Decayed_Type, true> {
    inline static constexpr std::size_t execute() { return int_cstr_size<Decayed_Type>(); }
};
template <typename Decayed_Type>
struct _num_cstr_size_<Decayed_Type, false> {
    inline static constexpr std::size_t execute() { return ryu_buffer<Decayed_Type>(); }
};

// Return the cstr buffer size (with null terminator) required to hold Num_Type
template <typename Num_Type, typename Decayed_Type = std::decay<Num_Type>::type>
constexpr std::size_t num_cstr_size(const Num_Type num = static_cast<Num_Type>(0))
{
    static_assert(
        std::numeric_limits<Decayed_Type>::is_specialized,
        "Num_Type must be a number and specialized!");
    return _num_cstr_size_<Decayed_Type, std::numeric_limits<Decayed_Type>::is_integer>::execute();
}

template <typename Decayed_Type, bool is_int>
struct _num_2_cstr_;
template <typename Decayed_Type>
struct _num_2_cstr_<Decayed_Type, true> {
    inline static int call(const Decayed_Type num, char* const buffer)
    {
        return int_2_cstr(num, buffer);
    }
};
template <typename Decayed>
struct _num_2_cstr_<Decayed, false> {
    inline static int call(const Decayed num, char* const buffer)
    {
        return ryu_float_2_cstr(num, buffer);
    }
};

// Returns the number of chars written (without null-terminator). If returned negative value, it is
// likely that the parsing function failed. @param buffer is guaranteed to be null-terminated.
template <typename Num_Type, typename Decayed_Type = std::decay<Num_Type>::type>
inline int num_2_cstr(const Num_Type num, char* const buffer)
{
    static_assert(
        std::numeric_limits<Decayed_Type>::is_specialized,
        "Num_Type must be a number and specialized!");
    constexpr bool is_int = std::numeric_limits<Decayed_Type>::is_integer;
    return _num_2_cstr_<Decayed_Type, is_int>::call(num, buffer);
}

// https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string
/*
 * Splits string s into tokens and add it to result.
 * This method can only work with single delimiter.
 */
template <typename Out>
void split(const std::string& s, char delim, Out result)
{
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
#define STR_N_CPY(dest_ptr, start_ptr, max_len) strcpy_s(dest_ptr, max_len, start_ptr)
#else
#define STR_N_CPY(dest_ptr, start_ptr, max_len) strcpy(dest_ptr, start_ptr)
#endif
#define STR_CPY(dest_ptr, start_ptr) STR_N_CPY(dest_ptr, start_ptr, Common::MAX_STR_CPY)
//#define STR_APPEND(str1, str2) str1 str2
#define STR_CONCAT(str1, str2) str1 str2
} // namespace Common