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
#include "zmath.h"

namespace Common {
/*
 * Converts a double value to a very precise string
 * that contains all of possible digits.
 */
std::string precise_to_string(double d);

inline std::string ryu_d2s(const double& d) {
    return std::string(d2s(d));
}

inline double ryu_s2d(const std::string& str) {
    double retval;
    s2d(str.data(), &retval);
    return retval;
}

// crack_atof gotten from http://coliru.stacked-crooked.com/a/2e28f0d71f47ca5e
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

} // namespace Common