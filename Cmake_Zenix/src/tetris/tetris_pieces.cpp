#include "tetris_pieces.h"


const Tetris::TetrisCollider Tetris::TetrisBody::colliders[] =
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
			X, X, O, O,
			O, X, X, O,
			O, O, O, O,
			O, O, O, O,
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
		O, O, O, O,
		O, O, O, X,
		O, X, X, X,
		O, O, O, O,
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

template<typename T>
Tetris::BodyType Tetris::body_type_val<T>(T input)
{
	return (Tetris::BodyType) ((input % 7) + 1);
}

template<>
Tetris::BodyType Tetris::body_type_val(Common::ZMath::UInt64RNG::RNGSeed input)
{
	//TODO: can definitely make this a less biased algorithm
	return (Tetris::BodyType) ((input % 7) + 1);
}

Tetris::TetrisBody::TetrisBody(const BodyType& type, const Position2D& initial_pos, const Rotation& initial_rot) :
	type(type), current_position(initial_pos), current_rot(initial_rot), collider()
{
	memcpy(collider, colliders[get_min_index(this->type)], sizeof(TetrisCollider));
}
Tetris::TetrisBody::TetrisBody(const BodyType& type) :
	type(type), current_position({ initial_x, initial_y }), current_rot(R_UP), collider()
{
	memcpy(collider, colliders[get_min_index(this->type)], sizeof(TetrisCollider));
}

//constexpr unsigned char Tetris::TetrisBody::xy2i(const unsigned char& x, const unsigned char& y)
//
//constexpr void Tetris::TetrisBody::i2xy(const unsigned char & i, unsigned char& x, unsigned char& y)
//

void Tetris::TetrisBody::rotate()
{
	current_rot = (current_rot + 1) % 4;
	Tetris::TetrisBody::rotate(this->collider, this->current_rot, this->type);
}

void Tetris::TetrisBody::reassign(const BodyType& type)
{
	this->type = type;
	//assign collider
	for (unsigned char i = 0; i < T_COLLIDER_LEN; i++)
	{
		collider[i] = (colliders[get_min_index(type)])[i];
	}
	current_position = { initial_x, initial_y };
	current_rot = R_UP;
}

void Tetris::TetrisBody::rotate(TetrisCollider& collider, const Rotation& rot, const BodyType& type)
{
//	for (unsigned char i = 0; i < T_COLLIDER_LEN; i++)
//	{
//		unsigned char x, y;
//		Tetris::TetrisBody::i2xy(i, x, y);
//		unsigned char j = i;
//#define WIDTH 4
//		switch (rot)
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
//			//j = (WIDTH - 1) + y + (WIDTH * x);
//			j = (WIDTH - 1) - y + (WIDTH * x);
//			break;
//		}
//#undef WIDTH
//		collider[i] = (colliders[type])[j];
//	}
	Rotation current_rot;
	switch (type)
	{
	case O:
		current_rot = R_UP;
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

Tetris::TetrisField::TetrisField()
{
	Tetris::TetrisField::assign_empty_field(this->collider);
}

unsigned char Tetris::TetrisField::update_collider(const TetrisBody& body, unsigned char& burn_y)
{
	//update the collider
#ifdef USE_ROUND_NEAREST
	unsigned char rounded_x = Common::ZMath::round_nearest(body.current_position.x),
		rounded_y = Common::ZMath::round_nearest(body.current_position.y);
#else
	unsigned char rounded_x = static_cast<unsigned char>(body.current_position.x);
	unsigned char rounded_y = static_cast<unsigned char>(body.current_position.y);
#endif
	for (unsigned char i = 0; i < T_COLLIDER_LEN; i++)
	{
		//MAKE SURE BEFORE THIS IS CALLED, THERE LOSE CHECK MUST BE CALLED
		if (body.collider[i]) //If there is collider
		{
			unsigned char global_x, global_y; //<<Notice this is unsigned.
			body.i2xy(i, global_x, global_y);
			global_x += rounded_x;
			global_y += rounded_y;
			this->collider[xy2i(global_x, global_y)] = body.type;
		}
	}
	//look for burn signals
	unsigned char burned_rows = 0;
	unsigned char _y = (static_cast<char>(body.current_position.y) + 4);

	//TODO: NOT NECESSARY TO CHECK FROM BOT, JUST CHECK FROM WHERE WE PLACED IT TO PIECE's TOP
	for (unsigned char y = (_y > FIELD_BOTTOM ? FIELD_BOTTOM : _y);
		;//y >= (FIELD_TOP);//y--
		) //From bot to top
	{
		//Check for row being able to burn
		bool delete_row = true;
		for (unsigned char x = FIELD_LEFT; x <= FIELD_RIGHT; x++)
		{
			//TODO: can be optimized into index + x_increment (?)
			if (collider[xy2i(x, y)] == BodyType::BLANK)
			{
				delete_row = false;
				break;
			}
		}
		if (delete_row)
		{
			burn_y = y;
			burned_rows++;
			//From current height to top (exclusive)
			for (unsigned char _y = y; _y > FIELD_TOP; _y--)
			{
				for (unsigned char _x = FIELD_LEFT; _x <= FIELD_RIGHT; _x++)
				{
					//TODO: DEBUG: blame this if burn algorithm doesn't yield good data
					//col[xy2i(_x, _y)] = col[xy2i(_x,_y-1)];
					unsigned char this_index = xy2i(_x, _y);
					collider[this_index] = collider[this_index - WIDTH];
				}
			}
			//highest (y = 0)
			for (unsigned char _x = FIELD_LEFT; _x <= FIELD_RIGHT; _x++)
			{
				//unsigned char this_index = xy2i(_x, 0);
				collider[_x] = BodyType::BLANK;
			}

		}
		else
		{
			if (y == FIELD_TOP)
			{
				break;
			}
			y--; //Since row is not deleting, it should go up
		}
	}
	return burned_rows;
}

unsigned char Tetris::TetrisField::update_collider(const TetrisBody body)
{
	////update the collider
	//unsigned char rounded_x = Common::ZMath::round_nearest(body.current_position.x),
	//	rounded_y = Common::ZMath::round_nearest(body.current_position.y);
	//for (unsigned char i = 0; i < T_COLLIDER_LEN; i++)
	//{
	//	//MAKE SURE BEFORE THIS IS CALLED, THERE LOSE CHECK MUST BE CALLED
	//	if (body.collider[i]) //If there is collider
	//	{
	//		unsigned char global_x, global_y; //<<Notice this is unsigned.
	//		body.i2xy(i, global_x, global_y);
	//		global_x += rounded_x;
	//		global_y += rounded_y;
	//		this->collider[xy2i(global_x, global_y)] = body.type;
	//	}
	//}
	////look for burn signals
	//unsigned char burned_rows = 0;
	////TODO: NOT NECESSARY TO CHECK FROM BOT, JUST CHECK FROM WHERE WE PLACED IT TO PIECE's TOP
	//for (char y = FIELD_BOTTOM;
	//	y >= FIELD_TOP;
	//	) //From bot to top
	//{
	//	//Check for row being able to burn
	//	bool delete_row = true;
	//	for (unsigned char x = FIELD_LEFT; x <= FIELD_RIGHT; x++)
	//	{
	//		if (collider[xy2i(x, y)] == BodyType::BLANK)
	//		{
	//			delete_row = false;
	//			break;
	//		}
	//	}
	//	if (delete_row)
	//	{
	//		burned_rows++;
	//		//From current height to top (exclusive)
	//		for (unsigned char _y = y; _y > FIELD_TOP; _y--)
	//		{
	//			for (unsigned char _x = FIELD_LEFT; _x <= FIELD_RIGHT; _x++)
	//			{
	//				//TODO: DEBUG: blame this if burn algorithm doesn't yield good data
	//				//col[xy2i(_x, _y)] = col[xy2i(_x,_y-1)];
	//				unsigned char this_index = xy2i(_x, _y);
	//				collider[this_index] = collider[this_index - WIDTH];
	//			}
	//		}
	//		//highest (y = 0)
	//		for (unsigned char _x = FIELD_LEFT; _x <= FIELD_RIGHT; _x++)
	//		{
	//			//unsigned char this_index = xy2i(_x, 0);
	//			collider[_x] = BodyType::BLANK;
	//		}
	//	}
	//}
	//return burned_rows;
	unsigned char x;
	return update_collider(body, x);
}

void Tetris::TetrisField::update_collider(const TetrisBody& body, unsigned char burn_y[4], unsigned char& n_burned)
{
	//(x,y) in which this represents the top left of the piece in global position
	unsigned char rounded_x = RoundingExt::position_round<unsigned char>(body.current_position.x),
		rounded_y = RoundingExt::position_round<unsigned char>(body.current_position.y);
	//Iterate through the tetris body and assign it into the field's collider
	unsigned char bottom_most_global_y = rounded_y;
	for (unsigned char i = 0; i < T_COLLIDER_LEN; i++)
	{
		if (body.collider[i])
		{
			unsigned char global_x, global_y;
			TetrisBody::i2xy(i, global_x, global_y);
			global_x += rounded_x;
			global_y += rounded_y;
			if (global_y > bottom_most_global_y) bottom_most_global_y = global_y;
			this->collider[xy2i(global_x, global_y)] = body.type;
		}
	}
	//--Handle burning--
	//Iterate from bot of the current piece to top.
	//Purpose: check for burn and drag everything from above of the burning row
	//downwards
	//PLEASE WORK ON REWORDING, HOLY SMOKES
	n_burned = 0;
	int iterating_y = bottom_most_global_y;
	//O(1)
	for (; iterating_y >= rounded_y; iterating_y--)
	{
		unsigned char field_index0 = xy2i(0, iterating_y);
		if (should_delete_row(field_index0))
		{
			//row delete handling
			burn_y[n_burned++] = iterating_y;
		}
	}
	if (n_burned == 0) return;
	//We can merge both clearing the burn_y rows and
	//handling falling physics by replacing the supposed empty
	//row with the nearest non-to-be-empty above row(s)
	//burn_y will have the order from bot to top
	unsigned char next_empty = 1;
#define empties_found next_empty
	//O(n)
	//TODO: make sure whether y should have char type
	//TODO: surely, we can optimize this even more
	//		by looping until next_empty < n_burned
	//		then assign only blank values until top.
	//		This would eliminate condition checking.
	for (unsigned char base_y = burn_y[0]; base_y > FIELD_TOP; base_y--)
	{
		while (next_empty < n_burned && (base_y - next_empty) == burn_y[next_empty])
		{
			next_empty++;
		}
		//note: swap_y = base_y - next_empty
		unsigned char base_index0 = xy2i(0, base_y);
		for (unsigned char x = FIELD_LEFT; x <= FIELD_RIGHT; x++)
		{
			if (
				//base_y > burn_y[n_burned-1] || 
				base_y < empties_found)
			{
				collider[base_index0 + x] = BodyType::BLANK;
			}
			else
			{
				collider[base_index0 + x] = collider[xy2i(x, base_y - empties_found)];
			}
		}
#undef empties_found
	}
}

bool Tetris::TetrisField::should_delete_row(const unsigned char& field_index0)
{
	//check left to right, if there is a hole, just get done checking the row
	//since it shouldn't be burned or deleted
	for (unsigned char x = FIELD_LEFT; x <= FIELD_RIGHT; x++)
	{
		if (collider[field_index0 + x] == BodyType::BLANK)
		{
			return false;
		}
	}
	return true;
}

bool Tetris::TetrisField::check_collider(const TetrisCollider& col, const Position2D& new_position) const
{
#ifdef USE_ROUND_NEAREST
	char rounded_x = Common::ZMath::round_nearest(new_position.x);
	char rounded_y = Common::ZMath::round_nearest(new_position.y);
#else
	char rounded_x = (char)new_position.x;
	char rounded_y = (char)new_position.y;
#endif
	for (unsigned char i = 0; i < T_COLLIDER_LEN; i++)
	{
		if (col[i]) //Ignore if there is not a collider there
		{
			unsigned char local_x, local_y;
			TetrisBody::i2xy(i, local_x, local_y);
			char global_x = local_x + rounded_x
				, global_y = local_y + rounded_y;
			if (
				//There is a collision out of bound
				global_x < FIELD_LEFT || global_x > FIELD_RIGHT || global_y < FIELD_TOP || global_y > FIELD_BOTTOM
				|| collider[xy2i(global_x, global_y)] != BodyType::BLANK //the field already have a collider there
				)
			{
				//COLLIDER DOES NOT FIT
				return false;
			}

		}
	}
	//IT HAS TO FIT (since everything is not overllaped)
	return true;
}



bool Tetris::TetrisField::check_collider(const TetrisBody& body) const
{
	return check_collider(body.collider, body.current_position);
}

//Please check new_rot.
bool Tetris::try_rotate(Tetris::TetrisBody& body, const Tetris::Rotation& new_rot, const TetrisField& field)
{
	TetrisCollider temp;
	TetrisBody::rotate(temp, new_rot, body.type);
	if (field.check_collider(temp, body.current_position)) //If rotate doesn't overlap
	{
		body.current_rot = new_rot;
		//reassign collider
		for (unsigned char i = 0; i < T_COLLIDER_LEN; i++)
		{
			body.collider[i] = temp[i];
		}
		return true;
	}
	return false;
}
void Tetris::TetrisField::assign_empty_field(FieldCollider& col)
{
	for (unsigned i = 0; i < LENGTH; i++)
	{
		col[i] = BodyType::BLANK;
	}
	assign_border(col);
}

void Tetris::TetrisField::assign_border(FieldCollider& col)
{
	//Border instantializing
	for (unsigned char y = FIELD_TOP; y <= FIELD_HEIGHT; y++)
	{
		col[xy2i(FIELD_LEFT - 1, y)] = BodyType::BORDER;
		col[xy2i(FIELD_RIGHT + 1, y)] = BodyType::BORDER;
	}
	for (unsigned char x = FIELD_LEFT; x <= FIELD_RIGHT; x++)
	{
		col[xy2i(x, FIELD_HEIGHT)] = BodyType::BORDER;
	}
}

//constexpr unsigned char Tetris::TetrisField::xy2i(const unsigned char& x, const unsigned char& y)
//
//
//constexpr void Tetris::TetrisField::i2xy(const unsigned char & i, unsigned char& x, unsigned char& y)
