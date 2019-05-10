#pragma once
#include <vector>
#include "rounding.h"
#include <limits>
#include "game_rng.h"

namespace LGEngine
{
	typedef unsigned char Rotation;
	//0 deg
	static const Rotation UP = 0;
	//90 deg
	static const Rotation RIGHT = 1;
	//180 deg
	static const Rotation DOWN = 2;
	//270 deg
	static const Rotation LEFT = 3;
	const unsigned char FIELD_WIDTH = 10;
	const unsigned char FIELD_HEIGHT = 20;
	const unsigned char T_COLLIDER_LEN = 16;
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
	BodyType body_type_val<TMath::GameRNG::RNGSeed>(TMath::GameRNG::RNGSeed input);

	class TetrisBody
	{
	public:
#pragma endregion

#pragma region Helpers
		static constexpr double initial_x = ((double) FIELD_WIDTH / 2)-1.0;
		static constexpr double initial_y = 0.0;
		static constexpr unsigned char xy2i(const unsigned char& x, const unsigned char& y)
		{
			return y * 4 + x;
		}
		static constexpr void i2xy(const unsigned char& i, unsigned char& x, unsigned char& y)
		{
			y = i / 4;
			x = i - (y * 4);
		}
		static const TetrisCollider colliders[8];
#pragma endregion

	public:
		BodyType type; //can access default collider
		TetrisCollider collider;
		Rotation current_rot;
		Position2D current_position;
		void rotate();
		void reassign(const BodyType& type);
		static void rotate(TetrisCollider& collider, const Rotation& rot, const BodyType& type);
		TetrisBody(const BodyType& type, const Position2D& initial_pos, const Rotation& initial_rot);
		TetrisBody(const BodyType& type);
	};

}