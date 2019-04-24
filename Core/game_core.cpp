#include "game_core.h"

GameCore::GameModule::GameModule(char args[])
{
	//Normal initialization
	//Classic tetris
	GameRule* classic_rule;
	classic_rule->max_x = 10;
	classic_rule->max_y = 20;
	classic_rule->n_pieces_knowahead = 1;
	info->rule = classic_rule;
}
bool GameCore::GameModule::try_update()
{

}

void GameCore::GameModule::start_game(GameRNG::RNGSeed seed)
{

}

GameRNG::RNGSeed GameCore::GameModule::start_game()
{
	GameRNG::RNGSeed initial_seed = GameRNG::generate_random_seed();
	start_game(initial_seed);
	return initial_seed;
}
