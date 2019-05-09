#include "lite_tetris_field.h"

LGEngine::TetrisField::TetrisField()
{
	LGEngine::TetrisField::assign_empty_field(this->collider);
}

unsigned char LGEngine::TetrisField::update_collider(const TetrisBody body)
{
	//update the collider
	unsigned char rounded_x = TMath::round_nearest(body.current_position.x),
		rounded_y = TMath::round_nearest(body.current_position.y);
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
	//TODO: NOT NECESSARY TO CHECK FROM BOT, JUST CHECK FROM WHERE WE PLACED IT TO PIECE's TOP
	for (unsigned char y = FIELD_BOTTOM; 
		y >= FIELD_TOP;//y--
#ifdef _DEBUG
		y--
#endif
		) //From bot to top
	{
		//Check for row being able to burn
		bool delete_row = true;
		for (unsigned char x = FIELD_LEFT; x <= FIELD_RIGHT; x++)
		{
			if (collider[xy2i(x, y)] == BodyType::BLANK)
			{
				delete_row = false;
				break;
			}
		}
		if (delete_row)
		{
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
#if _DEBUG
			y++;
#endif

		}
		else
		{
#ifndef _DEBUG
			y--; //Since row is not deleting, it should go up
#endif
		}
	}
	return burned_rows;
}

bool LGEngine::TetrisField::check_collider(const TetrisCollider& col, const Position2D& new_position) const
{
	char rounded_x = (char)new_position.x;
	char rounded_y = (char)new_position.y;
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

bool LGEngine::TetrisField::check_collider(const TetrisBody& body) const
{
	return check_collider(body.collider, body.current_position);
}

//Please check new_rot.
bool LGEngine::try_rotate(LGEngine::TetrisBody& body, const LGEngine::Rotation& new_rot, const TetrisField& field)
{
	
	TetrisCollider temp;
	TetrisBody::rotate(temp, new_rot, body.type);
	if(field.check_collider(temp, body.current_position)) //If rotate doesn't overlap
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
void LGEngine::TetrisField::assign_empty_field(FieldCollider& col)
{
	for (unsigned i = 0; i < LENGTH; i++)
	{
		col[i] = BodyType::BLANK;
	}
	assign_border(col);
}

void LGEngine::TetrisField::assign_border(FieldCollider& col)
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

//constexpr unsigned char LGEngine::TetrisField::xy2i(const unsigned char& x, const unsigned char& y)
//
//
//constexpr void LGEngine::TetrisField::i2xy(const unsigned char & i, unsigned char& x, unsigned char& y)
