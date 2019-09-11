#include "../tetris_game/tetris_game.h"

int main()
{
	auto game_obj = TetrisGame::Tetris<false, 60, 0x0508DEED>();
	game_obj.start();
	return 0;
}