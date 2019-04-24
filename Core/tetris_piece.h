#pragma once
#include <unordered_map>
namespace GameFeatures
{
	class TetrisPiece
	{
	public:
		enum class PieceType
		{
			I,
			J,
			L,
			O,
			S,
			Z,
			T
		};
		typedef bool Tetrimino[16];
		typedef unsigned char Rotation;
		typedef const std::unordered_map<PieceType, Tetrimino> PieceMap;

		//0  deg
		static const Rotation UP = 0;
		//90 deg
		static const Rotation RIGHT = 1;
		//180 deg
		static const Rotation DOWN = 2;
		//270 deg
		static const Rotation LEFT = 3;
		//Max x
		static const unsigned char WIDTH = 4;
		struct StaticConstMap
		{
			static PieceMap create()
			{
				PieceMap m;
//				{
//#define T true
//#define F false
//					{
//						PieceType::I, 
//						{
//						F, F, T, F,
//						F, F, T, F,
//						F, F, T, F,
//						F, F, T, F
//						}
//					}
//#undef T
//#undef F
//				};
#define Y true
#define N false
				m[PieceType::I] =
				{
					N, N, Y, N,
					N, N, Y, N,
					N, N, Y, N,
					N, N, Y, N
				};
				m[PieceType::Z] =
				{
					N, N, Y, N,
					N, Y, Y, N,
					N, Y, N, N,
					N, N, N, N
				};
				m[PieceType::S] =
				{
					N, Y, N, N,
					N, Y, Y, N,
					N, N, Y, N,
					N, N, N, N
				};
				m[PieceType::O] =
				{
					N, N, N, N,
					N, Y, Y, N,
					N, Y, Y, N,
					N, N, N, N
				};
				m[PieceType::T] =
				{
					N, N, Y, N,
					N, Y, Y, N,
					N, N, Y, N,
					N, N, N, N
				};
				m[PieceType::L] =
				{
					N, N, N, N,
					N, Y, Y, N,
					N, N, Y, N,
					N, N, Y, N
				};				
				m[ PieceType::J ] =
				{
					N, N, N, N,
					N, Y, Y, N,
					N, Y, N, N,
					N, Y, N, N
				};
#undef T
#undef F
				return m;
			}
			static const PieceMap pieces_map;
		};
		const PieceMap StaticConstMap::pieces_map = StaticConstMap::create();
		//static const PieceMap pieces_map;
		Tetrimino* data;
		Rotation current_rot;
		Tetrimino* rotate_piece(Tetrimino* default_piece, Rotation rot);
	private:
		typedef unsigned char Indexor;
		//In up rotation only or in default indexing
		static Indexor xy2i(Indexor x, Indexor y)
		{
			return y * WIDTH + x;
		}
		static void i2xy(Indexor i, Indexor* x, Indexor* y)
		{
			*y = i / WIDTH;
			*x = i - (WIDTH * (*y));
		}
	};
}
