#pragma once
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
}