#include "game_core.h"

GameCore::GameModule::GameModule(char args[]) : first_start(true)
{
	//Normal initialization
	//Classic tetris
	GameRule classic_tetris = GameRule::get_classic_tetris_rule();
	std::queue<TetrisPiece*> next_pieces;
	std::vector<TetrisPiece*> built_pieces;
	GameInfo::Level current_level = 0;
	GameInfo classic_info = { &classic_tetris, next_pieces, nullptr, built_pieces, current_level };
	info = &classic_info;
}
bool GameCore::GameModule::try_update(Input input)
{
	//Update physics
	return false;
}

bool GameCore::GameModule::start_game(GameRNG::RNGSeed seed)
{
	if (seed == 0) return false;
	start_game_no_check(seed);
	return true;
}

GameCore::GameRNG::RNGSeed GameCore::GameModule::start_game(void *)
{
	GameRNG::RNGSeed initial_seed = GameRNG::generate_random_seed();
	start_game(initial_seed);
	return initial_seed;
}

void GameCore::GameModule::start_game_no_check(GameCore::GameRNG::RNGSeed seed)
{
	if (!first_start)
	{
		//Restart
		//SAVE

		//DELETE
		info->next_pieces = {};
		info->built_pieces.clear();
	}
	else
	{
		//First start
		first_start = false;
	}
	info->current_piece = GameFeatures::TetrisPiece::generate_random_piece(&seed);
	//fill the next pieces with tetris pieces
	for (int i = 0; i < info->rule->n_pieces_knowahead; i++)
	{
		info->next_pieces.push(GameFeatures::TetrisPiece::generate_random_piece(&seed));
	}
}
