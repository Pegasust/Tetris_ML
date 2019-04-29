#include "renderer.h"
using namespace Renderer;
void draw_to_console(GameCore::GameInfo* info)
{
	std::unordered_set<GameFeatures::TetrisPiece::Indexor> occupied_indexes;
	TetrisPiece::PieceType* abstract_screen =
		(TetrisPiece::PieceType*) malloc(sizeof(TetrisPiece::PieceType) * info->rule->max_x * info->rule->max_y);

	for (unsigned char i = 0; i < info->built_pieces.size(); i++)
	{
		GameFeatures::TetrisPiece*& checking_piece = info->built_pieces[i];
		//top left position
		R_Uint x, y;
		GameFeatures::TetrisPiece::posId2xy(checking_piece->positionId, x, y, *info->rule);
		//Local loop
		for (TetrisPiece::Indexor _y = 0; _y < TetrisPiece::TETRIMINO_HEIGHT; _y++)
		{
			for (TetrisPiece::Indexor _x = 0; _x < TetrisPiece::TETRIMINO_WIDTH; _x++)
			{
				TetrisPiece::Indexor local_index = TetrisPiece::xy2i(_x, _y);
				if (checking_piece->data[local_index]) //if it occupies the space
				{
					//Assign it to the screen
					abstract_screen[xy2i(x + _x, y + _y)] = checking_piece->type;
					occupied_indexes.insert(local_index);
				}
			}
		}
	}
	//TODO: Do something with the uninitialized memory
#ifdef _WINDOWS_
	//Do something with the abstract_screen
	//...
	//Free abstract_screen mem
	HANDLE hConsole = CreateConsoleScreenBuffer();
#endif
	free(abstract_screen);
}