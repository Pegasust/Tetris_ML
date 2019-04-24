#pragma once
#include <queue>
#include <vector>
#include "tetris_piece.h"
using namespace GameFeatures;
namespace GameCore
{
	class GameModule
	{
	public:
		GameInfo* info;
	};
	struct GameInfo
	{
		GameRule* rule;
		std::queue<TetrisPiece> next_pieces;
		TetrisPiece* current_piece;
		std::vector<TetrisPiece> built_pieces;
	};
	struct GameRule
	{
		const int n_pieces_knowahead;
		const int max_x, max_y;
	};
}