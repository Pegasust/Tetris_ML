#include "../tetris_game/tetris_game.h"

int main()
{
	auto game_obj = TetrisGame::Tetris<true, 120, 0x0508DEED>();
	game_obj.start();
	return 0;
}