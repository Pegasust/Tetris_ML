#pragma once
#include <chrono>
#include <stdint.h>
#include <limits>
#ifdef _DEBUG
#include <iostream>
#endif
namespace TMath
{

	//I am using the xorshift algorithm because it can be manipulated, which might be useful
	//for machine learnings that want to learn single seed run (back-tracking).
	class GameRNG
	{
	public:
		//I use 64-bit because its max value (18,446,744,073,709,551,615) is enough randomness
		typedef uint64_t
			RNGSeed;
		union RNGUnion
		{
			uint64_t long_expr;
			double double_expr;
		};
		static constexpr RNGSeed RNGSEED_MAX = std::numeric_limits<RNGSeed>::max();
		typedef std::chrono::steady_clock RNGClock;
		//https://en.wikipedia.org/wiki/Xorshift
		//One of the fastest non-crypto rng algorithm.
		inline static RNGSeed xorshift64(RNGSeed state[1])
		{
			RNGSeed x = state[0];
			x ^= x << 13;
			x ^= x >> 7;
			x ^= x << 17;
			return state[0] = x;
		}
		inline static RNGUnion xorshift64(RNGUnion state[1])
		{
			RNGUnion x = state[0];
			x.long_expr ^= x.long_expr << 13;
			x.long_expr ^= x.long_expr >> 7;
			x.long_expr ^= x.long_expr << 17;
			state[0] = x;
			return x;
		}
		//Based on time
		static RNGSeed generate_random_seed();

		static bool get_bool(RNGSeed state[1], double percentage);

		static bool get_bool(RNGUnion state[1], double percentage);

		//Exclusive
		static double get_value(RNGUnion state[1], double const& min, double const& max);
	};
}