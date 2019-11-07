#pragma once

#include <stdint.h>
#include <mutex>
#include <limits>

namespace Common
{
namespace ZMath
{
	//old structure, enclosed in class with statics to hide functions
	class UInt64RNG
	{
	public:
		static std::mutex mtx;
		typedef uint64_t RNGSeed;
		static constexpr RNGSeed RNGSEED_MAX = std::numeric_limits<RNGSeed>::max();
	
		//you are not supposed to know this, but this might just be for testing's sake
#if !defined(DEBUG_DEFINED)
	private:
#endif
		RNGSeed seed;

#if !defined(DEBUG_DEFINED)
	public:
#endif

		inline static RNGSeed xorshift64(RNGSeed* state)
		{
			mtx.lock();
			RNGSeed x = state[0];
			x ^= x << 13;
			x ^= x >> 7;
			x ^= x << 17;
			state[0] = x;
			mtx.unlock();
			return state[0];
		}

		//[0, 1)
		inline static double to_double(RNGSeed x)
		{
			const union { RNGSeed i; double d; } u = { UINT64_C(0x3FF) << 52 | x >> 12 };
			return u.d - 1.0;
		}
		
		static bool get_bool(RNGSeed& seed, double& percentage);
		UInt64RNG(const RNGSeed& seed);

		RNGSeed get_value(); //shifts
		double get_double(); //shifts
		bool get_bool(double& percentage); //shifts
	};

	class small_prng
	{
		uint64_t a;
		uint64_t b;
		uint64_t c;
		uint64_t d;

		static inline uint64_t rot(uint64_t x, uint64_t k) noexcept 
		{ return (((x) << (k)) | ((x) >> (64 - (k)))); }
	public:
		explicit small_prng(uint64_t seed = 0xdeadbeef) noexcept
		{
			a = 0xf1ea5eed;
			b = c = d = seed;
			for (size_t i = 0; i < 20; ++i)
				(*this)();
		}

		inline uint64_t operator()() noexcept
		{
			uint64_t e = a - rot(b, 7);
			a = b ^ rot(c, 13);
			b = c + rot(d,37);
			c = d + e;
			d = e + a;
			return d;
		}
		//[0,1)
		inline double get_double()
		{
			uint64_t x = (*this)();
			const union { uint64_t i; double d; } u = { UINT64_C(0x3FF) << 52 | x >> 12 };
			return u.d - 1.0;
		}	
		inline bool get_bool(const double& percentage = 0.5)
		{
			if (percentage >= 1.0) return true;
			else
			{
				double x = get_double();
				return (1.0 - x) <= percentage;
			}
		}
	};

	//[0,1)
	inline double get_random()
	{
		return small_prng(rand()).get_double();
	}
}
}