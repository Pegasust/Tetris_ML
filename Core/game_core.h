#pragma once
#include <queue>
#include <vector>
#include <stdint.h>
#include "core_timing.h"
#include "tetris_piece.h"
using namespace GameFeatures;
namespace GameCore
{
	struct GameInfo
	{
		typedef unsigned long Level;

		GameRule* rule;
		std::queue<TetrisPiece*> next_pieces;
		TetrisPiece* current_piece;
		std::vector<TetrisPiece*> built_pieces;
		Level current_level;
	};
	struct GameRule
	{
		//The number of pieces shown on the top right corner
		int n_pieces_knowahead;
		//length or width of the game
		int max_x, max_y;
	};
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
		static RNGSeed generate_random_seed()
		{
			//Get time now
			auto invoke_time = RNGClock::now();
			//Hardcoded because 0xDEADBEEF is a valid number : ^)
			RNGSeed x[1] = { 0xDEADBEEF };
			//Have it undergo some task to eliminate the upcoming RNGClock::now() the exact same
			x[0] = xorshift64(x);
			//Get the time span (this is pseudo-random due to the various clock speed)
			auto time_span = RNGClock::now() - invoke_time;
			//In nanosecond => It is actually pretty hard to recreate this randomness
			RNGSeed y = time_span.count();
			//Divide by two on each because the two values might be too big and makes it out of 
			//64 bit boundary.
			//Moreover, since there is a very small chance, but is likely to happen, that y = 0.
			//We don't want that to happen since the xorshift will produce only 0s if
			//we throw a 0 as the seed.
			return (x[0] / 2) + (y / 2);
		}
		static bool get_bool(RNGSeed state[1], double percentage)
		{
			bool val;
			if (percentage >= 1.0) val = true;
			else
			{
				val = (static_cast<double>(state[0]) / static_cast<double>(RNGSEED_MAX) > percentage);
			}
			state[0] = xorshift64(state);
			return val;
		}
	};

	//Game engine
	class GameModule
	{
	public:
		GameInfo* info;
		//Initialize
		GameModule(char args[]);
		//Main loop; return false if there is a certain error
		bool try_update();
		//Start game with a pre-existed seed. Returns false if seed is zero.
		bool start_game(GameRNG::RNGSeed seed);
		//Start game with a randomly generated seed.
		GameRNG::RNGSeed start_game(void *);
		
	private:
		bool first_start{ true };
		void start_game_no_check(GameRNG::RNGSeed seed);
	};
}