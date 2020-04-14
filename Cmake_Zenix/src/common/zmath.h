#pragma once
#include <algorithm>
#include <cmath>
#include <limits>
#include <math.h>
#include <stdint.h>

#include "../../third_party/ryu/ryu.h"
#include "../../third_party/ryu/ryu_parse.h"

namespace Common {
namespace ZMath {
int round_nearest(double x);
int round_nearest(float f);

// www.boost.org/doc/libs/1_72_0/libs/math/doc/html/math_toolkit/float_comparison.html
inline double relative_diff(const double a, const double b) {
    return std::fabs(a - b) / std::max(std::fabs(a), std::fabs(b));
}

/* Fast (but not exactly correct) double equals method.*/
inline bool approx_equals(const double a, const double b) {
    if (a == b) {
        return true;
    }
    return relative_diff(a, b) < std::numeric_limits<double>::epsilon();
}
/* Compile-time ceiling for 32-bit values. This is useful for
round-up static_cast.*/
constexpr int32_t compile_time_ceil(float num) {
    return (static_cast<float>(static_cast<int32_t>(num)) == num)
               ? static_cast<int32_t>(num)
               : static_cast<int32_t>(num) + ((num > 0) ? 1 : 0);
}
/*
Compile-time operation to convert 64-bit for double-floating point.
*/
constexpr double long_bits_2_double(const std::uint64_t& bits) {
    union {
        std::uint64_t b;
        double d;
    } u = {bits};
    return u.d;
}
/*
 * Returns the output of the sigmoid(x/activation_curve).
 * This function returns (0, 1) for all double x, double activation_curve.
*/
template <typename FP_Type>
FP_Type sigmoid(FP_Type x, FP_Type activation_curve=1.0) {
#define _FP_TYPE_(x) static_cast<FP_Type>(x)
    // 1/(1+e^(-x/a))
    return _FP_TYPE_(1) / (_FP_TYPE_(1) + exp(-x / activation_curve));
#undef _FP_TYPE_
}

/*
 Returns the output of sigmoid(x/activation_curve) but casts to
 integral type.
 */
template <typename Int_Type, typename FP_Type>
Int_Type sigmoid_d2i(FP_Type x, FP_Type activation_curve) {

}
/*
* Returns magnitude of (0,0) from (x,y)
*/
inline double get_magnitude(double const& x, double const& y) {
return sqrt(x * x + y * y);
}
template <typename T>
struct vector2 {
    T x, y;
    vector2 operator+(const vector2& rhs) {
        vector2 re;
        re.x = this->x + rhs.x;
        re.y = this->y + rhs.y;
        return re;
    }
    vector2 operator-(const vector2& rhs) {
        vector2 re;
        re.x = this->x - rhs.x;
        re.y = this->y - rhs.y;
        return re;
    }
    void operator+=(const vector2& rhs) {
        this->x += rhs.x;
        this->y += rhs.y;
    }
    void operator-=(const vector2& rhs) {
        this->x -= rhs.x;
        this->y -= rhs.y;
    }
    vector2 operator+() {
        return {this->x, this->y};
    }
    vector2 operator-() {
        return {-this->x, -this->y};
    }
    vector2 operator*(const int& i) {
        return {this->x * i, this->y * i};
    }
    vector2 operator*(const double& d) {
        return {this->x * d, this->y * d};
    }
    double magnitude() const {
        double d_x = (double)x;
        double d_y = (double)y;
        return get_magnitude(d_x, d_y);
    }
};
} // namespace ZMath
} // namespace Common

namespace CZMath = Common::ZMath;