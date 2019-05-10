#include "lite_tetris_body.h"
const LGEngine::TetrisCollider LGEngine::TetrisBody::colliders[] =
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


template<typename T>
LGEngine::BodyType LGEngine::body_type_val<T>(T input)
{
	return (LGEngine::BodyType) ((input % 7) + 1);
}

template<>
LGEngine::BodyType LGEngine::body_type_val(TMath::GameRNG::RNGSeed input)
{
	//TODO: can definitely make this a less biased algorithm
	return (LGEngine::BodyType) ((input % 7) + 1);
}

LGEngine::TetrisBody::TetrisBody(const BodyType& type, const Position2D& initial_pos, const Rotation& initial_rot) :
	type(type), current_position(initial_pos), current_rot(initial_rot), collider()
{
	memcpy(collider, colliders[this->type], sizeof(TetrisCollider));
}
LGEngine::TetrisBody::TetrisBody(const BodyType& type) :
	type(type), current_position({ initial_x, initial_y }), current_rot(UP), collider()
{
	memcpy(collider, colliders[this->type], sizeof(TetrisCollider));
}

//constexpr unsigned char LGEngine::TetrisBody::xy2i(const unsigned char& x, const unsigned char& y)
//
//constexpr void LGEngine::TetrisBody::i2xy(const unsigned char & i, unsigned char& x, unsigned char& y)
//

void LGEngine::TetrisBody::rotate()
{
	current_rot = (current_rot + 1) % 4;
	LGEngine::TetrisBody::rotate(this->collider, this->current_rot, this->type);
}

void LGEngine::TetrisBody::reassign(const BodyType& type)
{
	this->type = type;
	//assign collider
	for (unsigned char i = 0; i < T_COLLIDER_LEN; i++)
	{
		collider[i] = (colliders[type])[i];
	}
	current_position = { initial_x, initial_y };
	current_rot = UP;
}

void LGEngine::TetrisBody::rotate(TetrisCollider& collider, const Rotation& rot, const BodyType& type)
{
	for (unsigned char i = 0; i < T_COLLIDER_LEN; i++)
	{
		unsigned char x, y;
		LGEngine::TetrisBody::i2xy(i, x, y);
		unsigned char j = i;
#define WIDTH 4
		switch (rot)
		{
		case UP:
			break;
		case RIGHT:
			j = (WIDTH * (WIDTH - 1)) + y - (WIDTH * x);
			break;
		case DOWN:
			j = (WIDTH * WIDTH) - 1 - (WIDTH * y) - x;
			break;
		case LEFT:
			//Darn sign.
			j = (WIDTH - 1) - y + (WIDTH * x);
			break;
		}
#undef WIDTH
		collider[i] = (colliders[type])[j];
	}
}