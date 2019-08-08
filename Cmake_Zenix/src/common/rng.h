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
}
}