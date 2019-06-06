#pragma once
#include "BodyType.h"
namespace LGEngine
{
	class CorrectTetrisBody
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
		static constexpr int get_min_index(BodyType type);
	public:
		BodyType type;
		TetrisCollider collider;
		Rotation current_rot;
		Position2D current_position;
		void rotate();
		static void rotate(TetrisCollider& collider, const Rotation& rot, const BodyType& type);
		CorrectTetrisBody(const BodyType& type, const Position2D& initial_pos, const Rotation& initial_rot);
		CorrectTetrisBody(const BodyType& type);
	};
}