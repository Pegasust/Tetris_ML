#pragma once
#include <chrono>
#include <stdint.h>
namespace GameCore
{

	//I am using the xorshift algorithm because it can be manipulated, which might be useful
	//for machine learnings that want to learn single seed run (back-tracking).
	class GameRNG
	{
	public:
		//I use 64-bit because its max value (18,446,744,073,709,551,615) is enough randomness
		typedef uint64_t RNGSeed;
#define RNGSEED_MAX 0xffffffffUL
		typedef std::chrono::steady_clock RNGClock;
		//https://en.wikipedia.org/wiki/Xorshift
		//One of the fastest non-crypto rng algorithm.
		static RNGSeed xorshift64(RNGSeed state[1])
		{
			RNGSeed x = state[0];
			x ^= x << 13;
			x ^= x >> 7;
			x ^= x << 17;
			return state[0] = x;
		}
		//Based on time
		static RNGSeed generate_random_seed();

		static bool get_bool(RNGSeed state[1], double percentage);
	};
}