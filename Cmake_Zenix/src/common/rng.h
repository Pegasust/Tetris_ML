#pragma once

#include <limits>
#include <mutex>
#include <stdint.h>
#include "zmath.h"
#include "game_clock.h"
namespace Common {
namespace ZMath {
// old structure, enclosed in class with statics to hide functions
class UInt64RNG {
public:
    static std::mutex mtx;
    typedef uint64_t RNGSeed;
    static constexpr RNGSeed RNGSEED_MAX = std::numeric_limits<RNGSeed>::max();

    // you are not supposed to know this, but this might just be for testing's sake
#if !defined(DEBUG_DEFINED)
private:
#endif
    RNGSeed seed;

#if !defined(DEBUG_DEFINED)
public:
#endif

    inline static RNGSeed xorshift64(RNGSeed* state) {
        mtx.lock();
        RNGSeed x = state[0];
        x ^= x << 13;
        x ^= x >> 7;
        x ^= x << 17;
        state[0] = x;
        mtx.unlock();
        return state[0];
    }

    // Returns a double value in range[0, 1)
    // from a given integral (uint64_t, unless implemented otherwise).
    inline static double to_double(RNGSeed x) {
        const union {
            RNGSeed i;
            double d;
        } u = {UINT64_C(0x3FF) << 52 | x >> 12};
        return u.d - 1.0;
    }

    static bool get_bool(RNGSeed& seed, double& percentage);
    UInt64RNG(const RNGSeed& seed);

    RNGSeed get_value();               // shifts
    double get_double();               // shifts
    bool get_bool(double& percentage); // shifts
};

class small_prng {
    uint64_t a;
    uint64_t b;
    uint64_t c;
    uint64_t d;

    static inline uint64_t rot(uint64_t x, uint64_t k) noexcept {
        return (((x) << (k)) | ((x) >> (64 - (k))));
    }

public:
    explicit small_prng(uint64_t seed = 0xdeadbeef) noexcept {
        a = 0xf1ea5eed;
        b = c = d = seed;
        for (size_t i = 0; i < 20; ++i)
            (*this)();
    }

    inline uint64_t operator()() noexcept {
        uint64_t e = a - rot(b, 7);
        a = b ^ rot(c, 13);
        b = c + rot(d, 37);
        c = d + e;
        d = e + a;
        return d;
    }
    // Returns [0, range)
    // This does not take into account that the least significant
    // bit is intrinsicly "weak" (0 will appear less than others).
    // If range is 0, returns 0.
    template <typename Uint_Type = uint64_t>
    Uint_Type get_range(const Uint_Type range) {
        if (range == 0) {
            return 0;
        }
        uint64_t x;
        Uint_Type modder = fill_right_msb(range);
        do {
            x = (*this)();
            x &= modder;
        } while (x >= range);
        return x;
    }
    template <typename Int_Type>
    Int_Type get_int() {
        return get_range(std::numeric_limits<Int_Type>::max());
    }
    // Returns [low, high)
    // This does not take into account that the least significant
    // bit is intrinsicly "weak" (0 will appear less than others).
    // If range is 0, returns 0.
    template <typename Int_Type = int64_t>
    Int_Type get_range(const Int_Type low, const Int_Type high) {
        Int_Type length = high - low;
        return get_range(length) + low;
    }

    //[0,1)
    inline double get_double() {
        uint64_t x = (*this)();
        const union {
            uint64_t i;
            double d;
        } u = {UINT64_C(0x3FF) << 52 | x >> 12};
        return u.d - 1.0;
    }
    inline bool get_bool(const double percentage = 0.5) {
        if (percentage >= 1.0)
            return true;
        else {
            double x = get_double();
            return (1.0 - x) <= percentage;
        }
    }
    // (-1, 1)
    inline double get_cramped() {
        return get_double() - get_double();
    }
};
static small_prng rng_instance(GameClock::Clock::now().time_since_epoch().count());
//[0,1)
double get_random();
//(-1,1)
double random_cramped();
} // namespace ZMath
} // namespace Common