#pragma once
#include <unordered_map>
#include "game_rng.h"
#include "tetris_physics.h"
namespace GameFeatures
{
	
	class TetrisPiece
	{
	public:
		typedef unsigned char Indexor;
		enum PieceType:unsigned char
		{
			BLANK,
			I,
			J,
			L,
			O,
			S,
			Z,
			T,
		};
		static const unsigned char N_PIECE_TYPES = 7;
		static const Indexor TETRIMINO_WIDTH = 4,
			TETRIMINO_HEIGHT = 4;
		static constexpr std::uint8_t TETRIMINO_LENGTH = TETRIMINO_WIDTH * TETRIMINO_HEIGHT;
		typedef bool Tetrimino[TETRIMINO_HEIGHT*TETRIMINO_WIDTH];
		typedef unsigned char Rotation;
		//typedef const std::unordered_map<PieceType, Tetrimino> PieceMap;
		typedef const Tetrimino PieceMap[N_PIECE_TYPES];
		typedef uint16_t PositionIndex;

		//0 deg
		static const Rotation UP = 0;
		//90 deg
		static const Rotation RIGHT = 1;
		//180 deg
		static const Rotation DOWN = 2;
		//270 deg
		static const Rotation LEFT = 3;
		//Max x
		static const unsigned char WIDTH = 4;
//		struct StaticConstMap
//		{
//			static PieceMap create()
//			{
//				PieceMap m;
////				{
////#define T true
////#define F false
////					{
////						PieceType::I, 
////						{
////						F, F, T, F,
////						F, F, T, F,
////						F, F, T, F,
////						F, F, T, F
////						}
////					}
////#undef T
////#undef F
////				};
//#define Y true
//#define N false
//				m[PieceType::I] =
//				{
//					N, N, Y, N,
//					N, N, Y, N,
//					N, N, Y, N,
//					N, N, Y, N
//				};
//				m[PieceType::Z] =
//				{
//					N, N, Y, N,
//					N, Y, Y, N,
//					N, Y, N, N,
//					N, N, N, N
//				};
//				m[PieceType::S] =
//				{
//					N, Y, N, N,
//					N, Y, Y, N,
//					N, N, Y, N,
//					N, N, N, N
//				};
//				m[PieceType::O] =
//				{
//					N, N, N, N,
//					N, Y, Y, N,
//					N, Y, Y, N,
//					N, N, N, N
//				};
//				m[PieceType::T] =
//				{
//					N, N, Y, N,
//					N, Y, Y, N,
//					N, N, Y, N,
//					N, N, N, N
//				};
//				m[PieceType::L] =
//				{
//					N, N, N, N,
//					N, Y, Y, N,
//					N, N, Y, N,
//					N, N, Y, N
//				};				
//				m[ PieceType::J ] =
//				{
//					N, N, N, N,
//					N, Y, Y, N,
//					N, Y, N, N,
//					N, Y, N, N
//				};
//#undef Y
//#undef N
//				return m;
//			}
//			static const PieceMap pieces_map;
//		};
//		const PieceMap StaticConstMap::pieces_map = StaticConstMap::create();
		//static const PieceMap pieces_map;
//		struct StaticConstMap
//		{
//			static const Tetrimino pieces_map[7];
//		};
//		const Tetrimino StaticConstMap::pieces_map[7] =
//{
//#define Y true
//#define N false
//				{
//					N, N, Y, N,
//					N, N, Y, N,
//					N, N, Y, N,
//					N, N, Y, N
//				},
//				{
//					N, N, Y, N,
//					N, Y, Y, N,
//					N, Y, N, N,
//					N, N, N, N
//				},
//				{
//					N, Y, N, N,
//					N, Y, Y, N,
//					N, N, Y, N,
//					N, N, N, N
//				},
//				{
//					N, N, N, N,
//					N, Y, Y, N,
//					N, Y, Y, N,
//					N, N, N, N
//				},
//				{
//					N, N, Y, N,
//					N, Y, Y, N,
//					N, N, Y, N,
//					N, N, N, N
//				},
//				{
//					N, N, N, N,
//					N, Y, Y, N,
//					N, N, Y, N,
//					N, N, Y, N
//				},
//				{
//					N, N, N, N,
//					N, Y, Y, N,
//					N, Y, N, N,
//					N, Y, N, N
//				}
//#undef Y
//#undef N
//};
		const PieceType type;
		Tetrimino data;
		Rotation current_rot;
		Position2D current_position;

		//default_piece is *data at current_rot = UP
		static Tetrimino* lazy_rotate_piece(const Tetrimino* default_piece, Rotation rot);
		static Tetrimino* lazy_rotate_piece(PieceType type, Rotation rot);
		static TetrisPiece* generate_random_piece(TMath::GameRNG::RNGSeed* seed);
		void rotate(Rotation rot);
		TetrisPiece(PieceType type, const Position2D& initial_position);
		static void posId2xy(PositionIndex i, int& x, int& y, int max_x, int max_y);
		static void posId2xy(PositionIndex i, unsigned int& x, unsigned int& y, int max_x, int max_y);
	public:
		//In up rotation only or in default indexing
		static Indexor xy2i(Indexor x, Indexor y)
		{
			return y * WIDTH + x;
		}
		static void i2xy(Indexor i, Indexor& x, Indexor& y)
		{
			y = (i / WIDTH);
			x = i - (WIDTH * (y));
		}
	};
	const TetrisPiece::Tetrimino pieces_map[] =
	{
#define Y true
#define N false
				{
					N, N, Y, N,
					N, N, Y, N,
					N, N, Y, N,
					N, N, Y, N
				},
				{
					N, N, Y, N,
					N, Y, Y, N,
					N, Y, N, N,
					N, N, N, N
				},
				{
					N, Y, N, N,
					N, Y, Y, N,
					N, N, Y, N,
					N, N, N, N
				},
				{
					N, N, N, N,
					N, Y, Y, N,
					N, Y, Y, N,
					N, N, N, N
				},
				{
					N, N, Y, N,
					N, Y, Y, N,
					N, N, Y, N,
					N, N, N, N
				},
				{
					N, N, N, N,
					N, Y, Y, N,
					N, N, Y, N,
					N, N, Y, N
				},
				{
					N, N, N, N,
					N, Y, Y, N,
					N, Y, N, N,
					N, Y, N, N
				}
#undef Y
#undef N
	};
}
