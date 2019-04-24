#include "tetris_piece.h"
using namespace GameFeatures;
TetrisPiece::Tetrimino* TetrisPiece::rotate_piece(TetrisPiece::Tetrimino* default_piece, TetrisPiece::Rotation rot)
{
	TetrisPiece::Tetrimino* new_piece;
	rot = rot % 4;
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
		TetrisPiece::i2xy(i, &x, &y); 
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