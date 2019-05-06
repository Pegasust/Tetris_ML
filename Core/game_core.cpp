#include "game_core.h"
GameCore::GameModule::GameModule(char args[]) : first_start(true)
{
	//Normal initialization
	//Classic tetris
	GameRule* classic_tetris = new GameRule(GameCore::classic_tetris);
	std::queue<TetrisPiece*> next_pieces;
	if (!next_pieces.empty())
	{
		throw(std::exception("Queue is not empty?"));
	}
	std::vector<TetrisPiece*> built_pieces;
	std::cout << built_pieces.size() << std::endl;
	GameInfo *classic_info = new GameInfo{ classic_tetris, next_pieces, nullptr, built_pieces, 0 };
	info = classic_info;
	std::cout << "module: " << this->info->rule->max_x << std::endl;
}

bool GameCore::GameModule::try_update(InputInterface::Input & input)
{
	int n_frames;
#ifdef TIMEBASED
	n_frames = tmr.get_frames_updates();
#else //defined FRAMEBASED
	n_frames = input.n_frames_update;
#endif
	for (; n_frames > 0; n_frames--)
	{
		//Physics update per call

	}
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

//const char* GameCore::GameModule::str_expr() const
//{
//	std::string str = "{ info: " + info->string_expr();
//	str += " }";
//	return str.c_str();
//}

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
