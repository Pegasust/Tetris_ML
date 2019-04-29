#include "tetris_piece.h"
using namespace GameFeatures;
TetrisPiece::Tetrimino* TetrisPiece::lazy_rotate_piece(const TetrisPiece::Tetrimino* default_piece, TetrisPiece::Rotation rot)
{
	TetrisPiece::Tetrimino* new_piece;
	//Initializing by x, y will not be pausible because new_piece is not initialized.
	/*for (unsigned char x = 0; x < 4; x++)
	{
		for (unsigned char y = 0; y < 4; y++)
		{
			unsigned char j;
			unsigned char i = xy2i(x, y);
			switch (rot)
			{
			case TetrisPiece::UP:
				j = i;
				break;
			case TetrisPiece::RIGHT:
				j = (WIDTH * (WIDTH - 1)) + y - (WIDTH * x);
				break;
			case TetrisPiece::DOWN:
				j = (WIDTH * WIDTH) - 1 - (WIDTH * y) - x;
				break;
			case TetrisPiece::LEFT:
				j = WIDTH - 1 + y + (WIDTH * x);
				break;
			}
			*new_piece[i] = *default_piece[j];
		}
	}*/
	//Initializing by increment of i will ensure memory gets expanded step by step
	//The correct way to initialize an array
	for (Indexor i = 0; i < 16; i++)
	{
		Indexor x, y;
		TetrisPiece::i2xy(i, x, y); 
		Indexor j;
		switch (rot)
		{
		case TetrisPiece::UP:
			j = i;
			break;
		case TetrisPiece::RIGHT:
			j = (WIDTH * (WIDTH - 1)) + y - (WIDTH * x);
			break;
		case TetrisPiece::DOWN:
			j = (WIDTH * WIDTH) - 1 - (WIDTH * y) - x;
			break;
		case TetrisPiece::LEFT:
			j = WIDTH - 1 + y + (WIDTH * x);
			break;
		}
		*new_piece[i] = *default_piece[j];
	}
	return new_piece;
}

TetrisPiece::Tetrimino* TetrisPiece::lazy_rotate_piece(TetrisPiece::PieceType type, TetrisPiece::Rotation rot)
{
	return lazy_rotate_piece(TetrisPiece::StaticConstMap::pieces_map[type], rot);
}
//Inputing UP or DOWN will not do anything. Filter before inputting.
void GameFeatures::TetrisPiece::rotate(Rotation rot)
{
	//prevent negativity on unsigned
	if (rot == LEFT)
	{
		if (current_rot == 0) current_rot = LEFT;
		else current_rot--;
	}
	//prevent further modulo
	else if (rot == RIGHT)
	{
		if (current_rot == 3) current_rot = UP;
		else current_rot++;
	}
	*data = lazy_rotate_piece(type, current_rot);
}

TetrisPiece* TetrisPiece::generate_random_piece(GameCore::GameRNG::RNGSeed* seed)
{
	unsigned char piece_id = *seed % StaticConstMap::pieces_map.size();
	PieceType key = static_cast<PieceType>(piece_id);
	TetrisPiece return_piece = TetrisPiece(key);
	*seed = GameCore::GameRNG::xorshift64(seed);
	return &return_piece;
}

GameFeatures::TetrisPiece::TetrisPiece(PieceType assigning_type):type(assigning_type)
{
	*data = pieces_map[type];
	current_rot = UP;
}

void GameFeatures::TetrisPiece::posId2xy(PositionIndex i, int &x, int &y, GameCore::GameRule rule)
{
#define CONVERT y = i / rule.max_x; x = i - (rule.max_x) * y;
	CONVERT
}

void GameFeatures::TetrisPiece::posId2xy(PositionIndex i, unsigned int& x, unsigned int& y, GameCore::GameRule rule)
{
	CONVERT
}
#undef CONVERT
#ifdef CONVERT
this should not be included alright
#endif
