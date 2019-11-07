#include "tetris_core.h"

Tetris::TetrisField TetrisAPI::get_copy_game_field(const TetrisEngine& src)
{
	return src.game_field;
}