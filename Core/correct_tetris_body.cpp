#include "correct_tetris_body.h"

const LGEngine::TetrisCollider LGEngine::CorrectTetrisBody::colliders[] =
{
#define O false
#define X true
	//Blank
	{
		O, O, O, O,
		O, O, O, O,
		O, O, O, O,
		O, O, O, O
	},
	//I 0
	{
		O, O, X, O,
		O, O, X, O,
		O, O, X, O,
		O, O, X, O
	},
	//I 1
	{
		O, O, O, O,
		X, X, X, X,
		O, O, O, O,
		O, O, O, O
	},
	//Z 0 //3
	{
		O, O, X, O,
		O, X, X, O,
		O, X, O, O,
		O, O, O, O
	},
//Z 1	
	{
		O, X, X, O,
		O, O, X, X,
		O, O, O, O,
		O, O, O, O
	},
//S 0	//5
	{
		O, X, O, O,
		O, X, X, O,
		O, O, X, O,
		O, O, O, O
	},
//S 1
	{
		O, O, X, X,
		O, X, X, O,
		O, O, O, O,
		O, O, O, O
	},
//O		//7
	{
		O, X, X, O,
		O, X, X, O,
		O, O, O, O,
		O, O, O, O
	},
//T 0	//8
	{
		O, O, X, O,
		O, X, X, O,
		O, O, X, O,
		O, O, O, O
	},
//T 1
	{
		O, O, X, O,
		O, X, X, X,
		O, O, O, O,
		O, O, O, O
	},
//T 2
	{
		O, O, X, O,
		O, O, X, X,
		O, O, X, O,
		O, O, O, O,
	},
//T 3
	{
		O, O, O, O,
		O, X, X, X,
		O, O, X, O,
		O, O, O, O
	},
	//L 0	//12
	{
		O, X, X, O,
		O, O, X, O,
		O, O, X, O,
		O, O, O, O
	},
	//L 1
	{
		O, O, O, X,
		O, X, X, X,
		O, O, O, O,
		O, O, O, O
	},
	//L 2
	{
		O, O, X, O,
		O, O, X, O,
		O, O, X, X,
		O, O, O, O
	},
	//L 3
	{
		O, O, O, O,
		O, X, X, X,
		O, X, O, O,
		O, O, O, O
	},
//J 0	//16
	{
		O, X, X, O,
		O, X, O, O,
		O, X, O, O,
		O, O, O, O
	},
//J 1
	{
		O, O, O, O,
		X, X, X, O,
		O, O, X, O,
		O, O, O, O
	},
//J 2
	{
		O, X, O, O,
		O, X, O, O,
		X, X, O, O,
		O, O, O, O
	},
//J 3
	{
		X, O, O, O,
		X, X, X, O,
		O, O, O, O,
		O, O, O, O
	}
#undef X
#undef O
};
constexpr int LGEngine::CorrectTetrisBody::get_min_index(BodyType type)
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
void LGEngine::CorrectTetrisBody::rotate()
{
	switch (type)
	{
	case O:
		current_rot = UP;
		break;
	case I:
	case Z:
	case S:
		current_rot = (current_rot + 1) % 2;
		break;
	default:
		current_rot = (current_rot+1) % 4;
		break;
	}
	LGEngine::CorrectTetrisBody::rotate(this->collider, this->current_rot, this->type);
}

void LGEngine::CorrectTetrisBody::rotate(TetrisCollider& collider, const Rotation& rot, const BodyType& type)
{
	for (int i = 0; i < T_COLLIDER_LEN; i++)
	{
		collider[i] = (colliders[get_min_index(type) + rot])[i];
	}
}
LGEngine::CorrectTetrisBody::CorrectTetrisBody(const BodyType& type, const Position2D& initial_pos, const Rotation& initial_rot) :
	type(type), current_position(initial_pos), current_rot(initial_rot), collider()
{
	std::memcpy(collider, colliders[get_min_index(this->type)], sizeof(TetrisCollider));
}
LGEngine::CorrectTetrisBody::CorrectTetrisBody(const BodyType& type) :
	type(type), current_position({ initial_x, initial_y }), current_rot(UP), collider()
{
	std::memcpy(collider, colliders[get_min_index(this->type)], sizeof(TetrisCollider));
}
