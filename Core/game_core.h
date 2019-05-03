#pragma once
#include <queue>
#include <vector>
#include <iostream>
#include <string>
#include "tetris_piece.h"
#include "game_rng.h"
#include "input_interface.h"
using namespace GameFeatures;
namespace GameCore
{
	struct GameRule
	{
	public:
		//The number of pieces shown on the top right corner
		int n_pieces_knowahead;
		//length or width of the game
		int max_x, max_y;
		const char* str_expr() const
		{
			std::string str = "{ know_ahead: " + std::to_string(n_pieces_knowahead);
			str += ", max_x: " + std::to_string(max_x);
			str += ", max y: " + std::to_string(max_y);
			str += " }";
			return str.c_str();
		}
		const std::string string_expr() const
		{
			std::string str = "{ know_ahead: " + std::to_string(n_pieces_knowahead);
			str += ", max_x: " + std::to_string(max_x);
			str += ", max y: " + std::to_string(max_y);
			str += " }";
			return str;
		}
	};
	const GameRule classic_tetris = { 1, 10, 20 };
	struct GameInfo
	{
		typedef unsigned long Level;

		GameRule *rule;
		std::queue<TetrisPiece*> next_pieces;
		TetrisPiece* current_piece;
		std::vector<TetrisPiece*> built_pieces;
		Level current_level;

		const std::string string_expr() const
		{
			std::string str = "{ rule: " + rule->string_expr();
			str += ", current_level: " + std::to_string(current_level);
			str += " }";
			return str;
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
		bool try_update(Input input);
		//Start game with a pre-existed seed. Returns false if seed is zero.
		bool start_game(GameRNG::RNGSeed seed);
		//Start game with a randomly generated seed.
		GameRNG::RNGSeed start_game(void *);
		const char* str_expr() const;
	private:
		CoreTiming::TimeModule tmr;
		bool first_start{ true };
		void start_game_no_check(GameRNG::RNGSeed seed);
	};
}