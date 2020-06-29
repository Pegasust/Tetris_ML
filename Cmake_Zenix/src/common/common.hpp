// The 1-header that includes everything required from the common library.
#pragma once
#include <array>
#include <list>
#include <memory>
// Undefine the following if user does not want to using namespace of sub-namespaces of Common.
// #define _COMMON_NO_USING_NAMESPACE_

#ifdef DEBUG_DEFINED
#define __CL_ENABLE_EXCEPTIONS
#endif
#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif
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
#include "path_consts.hpp"
#include "string_manip.h"
#include "zmath.h"
namespace Common {
#define Z_NULL_PTR nullptr
using SynchronousKeyboard = Common::KeyboardListener<false, 0, std::chrono::seconds>;
using BufferedKeyboard = Common::KeyboardListener<true, 1000 / 240, std::chrono::milliseconds>;
// template<typename NumberType, typename InType>
// constexpr NumberType array_size(InType arr[]) {
//    return static_cast<NumberType>(sizeof(arr) / sizeof(InType));
//}
// Improved version from:
// https://stackoverflow.com/questions/49454005/how-to-get-an-array-size-at-compile-time
template <std::size_t N, class T>
constexpr std::size_t array_size(T (&)[N]) {
    return N;
}
namespace TemplateArgs {
const int NO_ARGS = 0;
// using empty_collection = std::array<int,1>;
// using empty_collection = std::list<int>;
template <typename T = int>
class empty_collection_ {
public:
    using value_type = T;
    using allocator_type = std::allocator<T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = T*;
    using const_reverse_iterator = const T*;
    inline constexpr const_reference at(const size_type size) const {
        return static_cast<T>(NO_ARGS);
    }
    inline constexpr reference at(const size_type size) {
        return static_cast<T>(NO_ARGS);
    }
    inline constexpr reference operator[](const size_type size) {
        return at(size);
    }
    inline constexpr const_reference operator[](const size_type size) const {
        return at(size);
    }
    inline constexpr const_reference front() const {
        return at(0);
    }
    inline constexpr reference front() {
        return at(0);
    }
    inline constexpr const_reference back() const {
        return at(0);
    }
    inline constexpr reference back() {
        return at(0);
    }
    inline constexpr iterator begin() {
        return &static_cast<T>(NO_ARGS);
    }
    inline constexpr iterator end() {
        return begin();
    }
    inline constexpr bool empty() const {
        return true;
    }
    inline constexpr size_type size() const {
        return 0;
    }
    inline constexpr size_type max_size() const {
        return 0;
    }
    inline constexpr void reserve() const {}
    inline constexpr size_type capacity() const {
        return 0;
    }
    inline constexpr void shrink_to_fit() const {}
    inline constexpr void clear() const {}
    inline constexpr iterator insert() const {
        return begin();
    }
    template <typename... Args>
    inline constexpr iterator emplace(const_iterator pos, Args&... args) {
        return insert();
    }
    inline constexpr void push_back(const_reference value) {}
    inline constexpr void push_back(value_type&& value) {}
    template <typename... Args>
    inline constexpr reference emplace_back(Args&&... args) {
        return back();
    }

};
using empty_collection = empty_collection_<>;
template <typename T = empty_collection>
T& no_args(void) {
    return *(T*)(&NO_ARGS);
}
} // namespace TemplateArgs
#ifndef _COMMON_NO_USING_NAMESPACE_
using namespace ZMath;
using namespace GameClock;
using namespace PathConsts;
using namespace TemplateArgs;
#endif
} // namespace Common