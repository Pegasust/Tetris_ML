// The 1-header that includes everything required from the common library.
#pragma once
#include "assertion.hpp"
// Usage: ASSERT(evaluation (true/false), msg)
#include "file_managing.hpp"
// Usage: namespace: std_filesystem
#include "keyboard_input.h"
/*
 * Blocked input: int KeyboardListener<false, time_out, ChronoType>::get_key():
 ChronoType example: std::chrono::microseconds
 if time_out == 0, the program is blocked until user inputs
 otherwise: the program will "rest" until time_out <ChronoType> meets

 * Threaded input: int KeyboardListener<true, time_out, ChronoType>::get_key():
 if time_out == 0, the input thread rests until the user inputs (eager),
 else, the input thread sleeps for time_out duration before
 recording all keystrokes from the OS (lazy).
 If get_key is called at the time the OS records no keystrokes, return -1
 */
#include "rng.h"
/*
 * Common::ZMath::get_random(): returns random [0,1), shared by many functions and
 threads (instance: small_prng.

 * xorshift64: UInt64RNG::xorshift64(RNGSeed* state) (64-bit integer, random)
 * UInt64RNG::to_double(RNGSeed x): returns [0, 1) double from 64-bit integer seed.
 * Uint64RNG::get_bool(RNGSeed& seed, double& percentage): returns whether this seed
 satisfy the percentage.
 * UInt64RNG(instance)::get_value(): get 64-bit integer value
 * UInt64RNG(instance)::get_double(): get a [0, 1) value
 * UInt64RNG(instance)::get_bool(double& percentage): get whether this current seed
 satisfies the percentage.

 * small_prng: xorshift256:
 * instance: operator(): returns random 64-bit integer value
 *           get_double(): returns [0,1)
 *           get_bool(const double& percentage): returns whether this seed satisfies
 percentage.
*/
#include "string_manip.h"
#include "zmath.h"

namespace Common {
//template<typename NumberType, typename InType>
//constexpr NumberType array_size(InType arr[]) {
//    return static_cast<NumberType>(sizeof(arr) / sizeof(InType));
//}
// Improved version from: https://stackoverflow.com/questions/49454005/how-to-get-an-array-size-at-compile-time
template <std::size_t N, class T>
constexpr std::size_t array_size(T (&)[N]) {
    return N;
}
}