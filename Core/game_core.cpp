#include "game_core.h"

GameCore::GameModule::GameModule(char args[]) : first_start(true)
{
	//Normal initialization
	////Classic tetris
	//GameRule classic_rule;
	//classic_rule.max_x = 10;
	//classic_rule.max_y = 20;
	//classic_rule.n_pieces_knowahead = 1;
	//info->rule= &classic_rule;
	info->rule->max_x = 10;
	info->rule->max_y = 20;
	info->rule->n_pieces_knowahead = 1;
}
bool GameCore::GameModule::try_update()
{

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
