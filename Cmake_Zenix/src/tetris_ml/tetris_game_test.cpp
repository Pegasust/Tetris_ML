#include "../tetris_game/tetris_game.h"

int main()
{
	auto game_obj = TetrisGame::Tetris<false, 60, 0xDEADBEEF>();
	game_obj.start();
	return 0;
}