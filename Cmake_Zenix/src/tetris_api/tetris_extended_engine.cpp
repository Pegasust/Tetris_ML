#include "tetris_extended_engine.h"

TetrisAPI::TetrisExtendedEngine::TetrisExtendedEngine():
	TetrisAPI::TetrisExtendedEngine::TetrisExtendedEngine(rand())
{
}

TetrisAPI::TetrisExtendedEngine::TetrisExtendedEngine(const unsigned long long& seed) :
	engine(seed), game_clock(false), time_initialized(false)
{
}

Tetris::TetrisField& TetrisAPI::TetrisExtendedEngine::update(const Tetris::Input& input, bool& update_staticized_last_piece, TetrisBody& controlling_piece, unsigned char burn_y[4], unsigned char& n_burned, double& delta_time)
{
	//Handle time initilization
	if (!time_initialized)
	{
		delta_time = 0.0;
		time_initialized = true; //time will be initilized after the
		// if statement
	}
	else
	{
		delta_time = game_clock.fp_diff_seconds_micro();
	}
	game_clock.reset_then();
	TetrisAPI::update(engine, input, delta_time, update_staticized_last_piece,
		controlling_piece, burn_y, n_burned);
	return engine.game_field;
}

void TetrisAPI::TetrisExtendedEngine::reset(const unsigned long long& seed)
{
	new_game(seed, engine);
	time_initialized = false;
}
