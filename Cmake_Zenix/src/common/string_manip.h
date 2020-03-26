/*
 * Contains functions that converts strings to double and
 * double to strings.
 */

#pragma once
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include "zmath.h"

namespace Common {
constexpr int RYU_BUFFER_SIZE = 20;
/*
 * Converts a double value to a very precise string
 * that contains all of possible digits. Although
 * this produces quite precise string of double,
 * it takes so much CPU cycles and sometimes
 * output unnecessarily long strings compared to
 * ryu's
 */
std::string precise_to_string(double d);

// Allocates a static char[RYU_BUFFER_SIZE] upon first call.
std::string ryu_d2s(const double& d);
// Prefer this method over non-buffered one
// because this doesn't allocate new char buffers
inline void ryu_d2s_buffered(const double& d, char* s) {
    d2s_buffered(d, s);
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

//https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string

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

} // namespace Common