#pragma once
#include <vector>
#include "rounding.h"
#include <limits>
#include "game_rng.h"
namespace LiteGameEngine
{
	const unsigned char TETRIS_COLLIDER_LENGTH = 16;
	typedef bool TetrisCollider[TETRIS_COLLIDER_LENGTH];	
	struct Vector2D_FP64
	{
		double x, y;
	};
	typedef Vector2D_FP64 Position2D;
	typedef Vector2D_FP64 Velocity2D;
	enum BodyType :unsigned char
	{
		BLANK,
		I,
		Z,
		S,
		O,
		T,
		L,
		J,
		BORDER
	};
	BodyType rng_seed2bodytype(const TMath::GameRNG::RNGSeed& input)
	{
		//TODO: FIX THIS TO A LESS BIASED ALGORITHM
		return (BodyType) ((input % 7)+1);
	}
	template<unsigned char max_x, unsigned char max_y>
	struct TetrisBody
	{
#pragma region Defs
	public:
		typedef unsigned char Rotation;		
		//0 deg
		static const Rotation UP = 0;
		//90 deg
		static const Rotation RIGHT = 1;
		//180 deg
		static const Rotation DOWN = 2;
		//270 deg
		static const Rotation LEFT = 3;
#pragma endregion

#pragma region Helpers
		static constexpr double initial_x = (max_x/2) - 2.0;
		static constexpr double initial_y = -1.0;
		static constexpr unsigned char xy2i(const unsigned char& x, const unsigned char& y);
		static constexpr void i2xy(const unsigned char& i, unsigned char& x, unsigned char& y);
#pragma endregion

	public:
		BodyType type; //can access default collider
		TetrisCollider collider;
		Rotation current_rot;
		Position2D current_position;
		void rotate();
		void reassign(const BodyType& type);
		static void rotate(TetrisCollider& collider, const Rotation& rot, const BodyType& type);
		TetrisBody(const BodyType  & type, const Position2D & initial_pos, const Rotation & initial_rot);
		TetrisBody(const BodyType& type);
	};
	const TetrisCollider colliders[] =
	{
#define Y true
#define N false
				//blank
				{
					N, N, N, N,
					N, N, N, N,
					N, N, N, N,
					N, N, N, N
				},
				//I
				{
					N, N, Y, N,
					N, N, Y, N,
					N, N, Y, N,
					N, N, Y, N
				},
				//Z
				{
					N, N, Y, N,
					N, Y, Y, N,
					N, Y, N, N,
					N, N, N, N
				},
				//S
				{
					N, Y, N, N,
					N, Y, Y, N,
					N, N, Y, N,
					N, N, N, N
				},
				//O
				{
					N, N, N, N,
					N, Y, Y, N,
					N, Y, Y, N,
					N, N, N, N
				},

				//T
				{
					N, N, Y, N,
					N, Y, Y, N,
					N, N, Y, N,
					N, N, N, N
				},
				// L
				{
					N, N, N, N,
					N, Y, Y, N,
					N, N, Y, N,
					N, N, Y, N
				},
				// J
				{
					N, N, N, N,
					N, Y, Y, N,
					N, Y, N, N,
					N, Y, N, N
				}
#undef Y
#undef N
	};
	template<unsigned char width = 10, unsigned char height = 20>
	struct TetrisField
	{
		static const unsigned char WIDTH = width + 2;
		static const unsigned char HEIGHT = height + 1;
		static const unsigned char LENGTH = WIDTH * HEIGHT;
		typedef BodyType FieldCollider[LENGTH];
		FieldCollider collider;
		//std::vector<TetrisBody<width, height>*> all_colliders; //All of the BUILT tetris pieces
		TetrisField();
	public:
		//void update_collider(const TetrisCollider& tetris_col, const BodyType& t, const Position2D& pos); //update collider from all_colliders
		//return the # of rows burned
		unsigned char update_collider(const TetrisBody<width, height>& body); //Trust me and just update without check.
		static void assign_empty_field(FieldCollider& col);
		static void assign_border(FieldCollider& col);
		static constexpr unsigned char xy2i(const unsigned char & x, const unsigned char & y)
		{
			return y * WIDTH + x;
		}
		static constexpr void i2xy(const unsigned char& i, unsigned char& x, unsigned& y)
		{
			y = i / WIDTH;
			x = i - (y * WIDTH);
		}
		//bool try_add_body(const TetrisBody<width, height>& body);
	};
	template<unsigned char w= 10, unsigned char h =20>
	bool collider_fit(const TetrisCollider& col, const Position2D& new_position, TetrisField<w, h>& field);
	template<unsigned char w = 10, unsigned char h = 20>
	bool try_rotate(TetrisBody<w, h>& body, TetrisField<w, h>& field);
}