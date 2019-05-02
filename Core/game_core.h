#pragma once
#include <queue>
#include <vector>
#include "core_timing.h"
#include "tetris_piece.h"
#include "game_rng.h"
using namespace GameFeatures;
namespace GameCore
{
	struct Input
	{
		double x, y;
	};
	struct GameRule
	{
	public:
		//The number of pieces shown on the top right corner
		int n_pieces_knowahead;
		//length or width of the game
		int max_x, max_y;
		static const GameRule get_classic_tetris_rule()
		{
			return { 1, 10, 20 };
		}
	};
	struct GameInfo
	{
		typedef unsigned long Level;

		GameRule *rule;
		std::queue<TetrisPiece*> next_pieces;
		TetrisPiece* current_piece;
		std::vector<TetrisPiece*> built_pieces;
		Level current_level;
	};
	//Game engine
	class GameModule
	{
	public:
		GameInfo* info;
		//Initialize
		GameModule(char args[]);
		//Main loop; return false if there is a certain error
		bool try_update(Input input);
		//Start game with a pre-existed seed. Returns false if seed is zero.
		bool start_game(GameRNG::RNGSeed seed);
		//Start game with a randomly generated seed.
		GameRNG::RNGSeed start_game(void *);
		
	private:
		bool first_start{ true };
		void start_game_no_check(GameRNG::RNGSeed seed);
	};
}