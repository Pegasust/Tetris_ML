#include "lite_tetris_body.h"
#ifdef USE_LITE_BODY
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
		N, Y, Y, N,
		N, Y, Y, N,
		N, N, N, N,
		N, N, N, N,
	},

	//T
	{
		N, N, N, N,
		N, N, Y, N,
		N, Y, Y, N,
		N, N, Y, N
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


LGEngine::TetrisBody::TetrisBody(const BodyType& type, const Position2D& initial_pos, const Rotation& initial_rot) :
	type(type), current_position(initial_pos), current_rot(initial_rot), collider()
{
	std::memcpy(collider, colliders[this->type], sizeof(TetrisCollider));
}
LGEngine::TetrisBody::TetrisBody(const BodyType& type) :
	type(type), current_position({ initial_x, initial_y }), current_rot(UP), collider()
{
	std::memcpy(collider, colliders[this->type], sizeof(TetrisCollider));
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
#if USE_LEGACY
	//rotation filtering
	Rotation next_rot;
	switch (type)
	{
	case O:
		return; //O has only one configuration
	case I:
	case Z:
	case S:
		next_rot = rot % 2;
		break;
	default:
		next_rot = rot;
		break;
	}
	for (unsigned char i = 0; i < T_COLLIDER_LEN; i++)
	{
		unsigned char x, y;
		LGEngine::TetrisBody::i2xy(i, x, y);
		unsigned char j = i;
#define WIDTH 4
		switch (next_rot)
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
			//j = (WIDTH - 1) + y + (WIDTH * x);
			j = (WIDTH - 1) - y + (WIDTH * x);
			break;
		}
#undef WIDTH
		collider[i] = (colliders[type])[j];
	}
#else
	correct_rotate(collider, rot, type);
#endif
}

void LGEngine::TetrisBody::correct_rotate(TetrisCollider& collider, const Rotation& rot, const BodyType& type)
{
//	if (type == BodyType::O)
//		return; //O only have one configuration
//	Rotation next_rot;
//	char offset_x, offset_y;
//	switch (type)
//	{
//	case I:
//		next_rot = ((rot % 2) ? 3 : 0); //Map 2 configurations as Up and Left instead of Up and Right.
//		offset_x = 0, offset_y = 0;
//	case Z:
//	case S:
//		next_rot = rot % 2; //2 actual configurations
//		offset_x = 0, offset_y = (next_rot == 1? -1:0); //The second configuration shifteed up
//		break;
//	default:
//		next_rot = rot;
//		switch (next_rot)
//		{
//		case UP:
//			offset_x = 0, offset_y = -1;
//			break;
//		case RIGHT:
//			offset_x = 1, offset_y = -1;
//			break;
//		case DOWN:
//			offset_x = 1, offset_y = -1;
//			break;
//		default:
//			offset_x = 0, offset_y = 0;
//			break;
//		}
//		break;
//	}
//#define WIDTH 4
//	//reassign empty cols
//	if (offset_x < 0)
//	{
//		for (char _x = WIDTH + offset_x; _x < WIDTH; _x++)
//		{
//			for (char y = 0; y < WIDTH; y++)
//			{
//				collider[LGEngine::TetrisBody::xy2i(_x, y)] = false;
//			}
//		}
//	}
//	else if (offset_x > 0)
//	{
//		for (char _x = 0; _x < offset_x; _x++)
//		{
//			for (char y = 0; y < WIDTH; y++)
//			{
//				collider[LGEngine::TetrisBody::xy2i(_x, y)] = false;
//			}
//		}
//	}
//	//reassign empty rows
//	if (offset_y < 0)
//	{
//		for (char _y = WIDTH + offset_y; _y < WIDTH; _y++)
//		{
//			for (char x = 0; x < WIDTH; x++)
//			{
//				collider[LGEngine::TetrisBody::xy2i(x, _y)] = false;
//			}
//		}
//	}
//	else if (offset_y > 0)
//	{
//		for (char _y = 0; _y < offset_y; _y++)
//		{
//			for (char x = 0; x < WIDTH; x++)
//			{
//				collider[LGEngine::TetrisBody::xy2i(x, _y)] = false;
//			}
//		}
//	}
//	for (unsigned char i = 0; i < T_COLLIDER_LEN; i++)
//	{
//		unsigned char x, y;
//		LGEngine::TetrisBody::i2xy(i, x, y);
//		x += offset_x;
//		y -= offset_y;
//		unsigned char j = i;
//		switch (next_rot)
//		{
//		case UP:
//			break;
//		case RIGHT:
//			j = (WIDTH * (WIDTH - 1)) + y - (WIDTH * x);
//			break;
//		case DOWN:
//			j = (WIDTH * WIDTH) - 1 - (WIDTH * y) - x;
//			break;
//		case LEFT:
//			//Darn sign.
//			j = (WIDTH - 1) - y + (WIDTH * x);
//			break;
//		}
//		if (x < 0 || x > WIDTH || y < 0 || y > WIDTH)
//		{
//			collider[i] = false;
//		}
//		else
//		{
//			collider[i] = (colliders[type])[j];
//		}
//	}
//
//#undef WIDTH	

#define WIDTH 4
	if (type == BodyType::O)
	{
		for (unsigned char i = 0; i < T_COLLIDER_LEN; i++)
		{
			collider[i] = colliders[type][i];
		}
		return;
	}
	Rotation next_rot;
	char offset_x, offset_y;
	switch (type)
	{
	case I:
		next_rot = ((rot % 2) ? 3 : 0); //Map 2 configurations as Up and Left instead of Up and Right.
		offset_x = 0, offset_y = 0;
	case Z:
	case S:
		next_rot = rot % 2; //2 actual configurations
		offset_x = 0, offset_y = (next_rot == 1? -1:0); //The second configuration shifteed up
		break;
	default:
		next_rot = rot;
		switch (next_rot)
		{
		case UP:
			offset_x = 0, offset_y = -1;
			break;
		case RIGHT:
			offset_x = 1, offset_y = -1;
			break;
		case DOWN:
			offset_x = 1, offset_y = -1;
			break;
		default:
			offset_x = 0, offset_y = 0;
			break;
		}
		break;
	}
	////reassign empty cols
	//if (offset_x < 0)
	//{
	//	for (char _x = WIDTH + offset_x; _x < WIDTH; _x++)
	//	{
	//		for (char y = 0; y < WIDTH; y++)
	//		{
	//			collider[LGEngine::TetrisBody::xy2i(_x, y)] = false;
	//		}
	//	}
	//}
	//else if (offset_x > 0)
	//{
	//	for (char _x = 0; _x < offset_x; _x++)
	//	{
	//		for (char y = 0; y < WIDTH; y++)
	//		{
	//			collider[LGEngine::TetrisBody::xy2i(_x, y)] = false;
	//		}
	//	}
	//}
	////reassign empty rows
	//if (offset_y < 0)
	//{
	//	for (char _y = WIDTH + offset_y; _y < WIDTH; _y++)
	//	{
	//		for (char x = 0; x < WIDTH; x++)
	//		{
	//			collider[LGEngine::TetrisBody::xy2i(x, _y)] = false;
	//		}
	//	}
	//}
	//else if (offset_y > 0)
	//{
	//	for (char _y = 0; _y < offset_y; _y++)
	//	{
	//		for (char x = 0; x < WIDTH; x++)
	//		{
	//			collider[LGEngine::TetrisBody::xy2i(x, _y)] = false;
	//		}
	//	}
	//}
	for (unsigned char i = 0; i < T_COLLIDER_LEN; i++)
	{
		unsigned char x, y;
		LGEngine::TetrisBody::i2xy(i, x, y);
		x -= offset_x;
		y -= offset_y;
		if (x < 0 || x > WIDTH || y < 0 || y > WIDTH)
		{
			collider[i] = false;
			continue;
		}
		unsigned char j;
		switch (next_rot)
		{
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
		default: //UP
			j = LGEngine::TetrisBody::xy2i(x, y);
			break;
		}
		collider[i] = (colliders[type])[j];
		
	}


#undef WIDTH
}
#else
const LGEngine::TetrisCollider LGEngine::TetrisBody::colliders[] =
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
constexpr int LGEngine::TetrisBody::get_min_index(BodyType type)
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
void LGEngine::TetrisBody::rotate()
{
	current_rot = (current_rot + 1) % 4;

	LGEngine::TetrisBody::rotate(this->collider, this->current_rot, this->type);
}

void LGEngine::TetrisBody::rotate(TetrisCollider& collider, const Rotation& rot, const BodyType& type)
{
	Rotation current_rot;
	switch (type)
	{
	case O:
		current_rot = UP;
		break;
	case I:
	case Z:
	case S:
		current_rot = (rot) % 2;
		break;
	default:
		current_rot = rot;
		break;
	}
	for (int i = 0; i < T_COLLIDER_LEN; i++)
	{
		collider[i] = (colliders[get_min_index(type) + current_rot])[i];
	}
}
LGEngine::TetrisBody::TetrisBody(const BodyType& type, const Position2D& initial_pos, const Rotation& initial_rot) :
	type(type), current_position(initial_pos), current_rot(initial_rot), collider()
{
	std::memcpy(collider, colliders[get_min_index(this->type)], sizeof(TetrisCollider));
}
LGEngine::TetrisBody::TetrisBody(const BodyType& type) :
	type(type), current_position({ initial_x, initial_y }), current_rot(UP), collider()
{
	std::memcpy(collider, colliders[get_min_index(this->type)], sizeof(TetrisCollider));
}
void LGEngine::TetrisBody::reassign(const BodyType& type)
{
	this->type = type;
	//assign collider
	for (unsigned char i = 0; i < T_COLLIDER_LEN; i++)
	{
		collider[i] = (colliders[get_min_index(type)])[i];
	}
	current_position = { initial_x, initial_y };
	current_rot = UP;
}
#endif