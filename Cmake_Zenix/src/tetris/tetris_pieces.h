#pragma once

#pragma once
#include "game_specs.h"
#include <vector>
#include "../common/zmath.h"
#include <limits>
#include "../common/rng.h"

namespace Tetris
{
	typedef unsigned char Rotation;
	//0 deg
	static const Rotation R_UP = 0;
	//90 deg
	static const Rotation R_RIGHT = 1;
	//180 deg
	static const Rotation R_DOWN = 2;
	//270 deg
	static const Rotation R_LEFT = 3;
	const unsigned char FIELD_WIDTH = 10;
	const unsigned char FIELD_HEIGHT = 20;
	const unsigned char T_COLLIDER_WID = 4;
	const unsigned char T_COLLIDER_HEIGHT = 4;
	const unsigned char T_COLLIDER_LEN = T_COLLIDER_HEIGHT* T_COLLIDER_WID;
	typedef bool TetrisCollider[T_COLLIDER_LEN];
	struct Vector2D
	{
		double x, y;
	};
	typedef Vector2D Position2D;
	typedef Vector2D Velocity2D;
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
	template<typename T>
	BodyType body_type_val(T input);

	template<>
	BodyType body_type_val<Common::ZMath::UInt64RNG::RNGSeed>(Common::ZMath::UInt64RNG::RNGSeed input);

	class TetrisBody
	{

	public:
		static constexpr double MIN_POS_X = -1.0;
		static constexpr double MAX_POS_X = (double)FIELD_WIDTH - 2.0;
		static constexpr double MIN_POS_Y = 0.0;
		static constexpr double MAX_POS_Y = (double)FIELD_HEIGHT - 1.0;
		static constexpr double initial_x = ((double)FIELD_WIDTH / 2) - 1.0;
		static constexpr double initial_y = 0.0;
		static constexpr unsigned char xy2i(const unsigned char& x, const unsigned char& y)
		{
			return y * 4u + x;
		}
		static constexpr void i2xy(const unsigned char& i, unsigned char& x, unsigned char& y)
		{
			y = i / (unsigned char)4;
			x = i - (y * (unsigned char)4);
		}
		static const TetrisCollider colliders[];
		static const int
			BLANK_MIN = 0,
			I_MIN = 1,
			Z_MIN = 3,
			S_MIN = 5,
			O_MIN = 7,
			T_MIN = 8,
			L_MIN = 12,
			J_MIN = 16;
		static constexpr int get_min_index(BodyType type) //default rotation
		{

#define B_MIN(type) type##_MIN;
			switch (type)
			{
			case BLANK:
				return B_MIN(BLANK);

			case I:
				return B_MIN(I);

			case Z:
				return B_MIN(Z);

			case S:
				return B_MIN(S);

			case O:
				return B_MIN(O);

			case T:
				return B_MIN(T);

			case L:
				return B_MIN(L);

			case J:
				return B_MIN(J);

			default:
				return -1;
			}
#undef B_MIN
		}
	public:
		BodyType type;
		//We can actually optimize the collider out now that we have the cache of
		//all colliders dependent on their body type and rotation
		TetrisCollider collider;
		Rotation current_rot;
		Position2D current_position;
		void rotate();
		void reassign(const BodyType& type);
		static void rotate(TetrisCollider& collider, const Rotation& rot, const BodyType& type);
		TetrisBody(const BodyType& type, const Position2D& initial_pos, const Rotation& initial_rot);
		TetrisBody(const BodyType& type);
	};
	class TetrisField
	{
	public:
		static constexpr unsigned char WIDTH = Tetris::FIELD_WIDTH + 2;
		static constexpr unsigned char HEIGHT = Tetris::FIELD_HEIGHT + 1;
		static constexpr unsigned char LENGTH = WIDTH * HEIGHT;

		static constexpr unsigned char FIELD_BOTTOM = Tetris::FIELD_HEIGHT - 1;
		static constexpr unsigned char FIELD_TOP = 0;
		static constexpr unsigned char FIELD_LEFT = 1;
		static constexpr unsigned char FIELD_RIGHT = Tetris::FIELD_WIDTH;

		typedef BodyType FieldCollider[LENGTH];
		FieldCollider collider;

		TetrisField();
		unsigned char update_collider(const TetrisBody body); //Trust me and just update without check.
		//Returns the number of rows burned
		unsigned char update_collider(const TetrisBody& body, unsigned char& burn_y);
		void update_collider(const TetrisBody& body, unsigned char burn_y[4], unsigned char& n_burned);
		bool check_collider(const TetrisCollider& col, const Position2D& new_position) const;
		//bool check_collider(const TetrisCollider& col, const Position2D& new_position, const Rotation & new_rot) const;
		bool check_collider(const TetrisBody& body) const;
		static void assign_empty_field(FieldCollider& col);
		static void assign_border(FieldCollider& col);
		static constexpr unsigned char xy2i(const unsigned char& x, const unsigned char& y)
		{
			return y * Tetris::TetrisField::WIDTH + x;
		}
		static constexpr void i2xy(const unsigned char& i, unsigned char& x, unsigned char& y)
		{
			y = i / WIDTH;
			x = i - (WIDTH * i);
		}
	private:
		bool should_delete_row(const unsigned char& field_index0);
	};
	bool try_rotate(TetrisBody& collider, const Tetris::Rotation& new_rot, const TetrisField& field);

}