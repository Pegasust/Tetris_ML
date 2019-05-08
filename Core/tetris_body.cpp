#include "tetris_body.h"
template<unsigned char w, unsigned char h>
constexpr unsigned char LiteGameEngine::TetrisBody<w, h>::xy2i(const unsigned char& x, const unsigned char& y)
{
	return y * 4 + x;
}

template<unsigned char max_x, unsigned char max_y>
constexpr void LiteGameEngine::TetrisBody<max_x, max_y>::i2xy(const unsigned char& i, unsigned char& x, unsigned char& y)
{
	y = i / 4;
	x = i - (y * 4);
}

template<unsigned char max_x, unsigned char max_y>
void LiteGameEngine::TetrisBody<max_x, max_y>::rotate()
{
	current_rot = (current_rot + 1) % 4;
	for (unsigned char i = 0; i < TETRIS_COLLIDER_LENGTH; i++)
	{
		unsigned char x, y;
		this->i2xy(i, x, y);
		unsigned char j;
#define WIDTH 4
		switch (current_rot)
		{
			case UP:
				j = i;
				break;
			case RIGHT:
				j = (WIDTH * (WIDTH - 1)) + y - (WIDTH * x);
				break;
			case DOWN:
				j = (WIDTH * WIDTH) - 1 - (WIDTH * y) - x;
				break;
			case LEFT:
				j = WIDTH - 1 + y + (WIDTH * x);
				break;
		}
#undef WIDTH
		this->collider[i] = (colliders[this->type])[j];
	}
}

template<unsigned char max_x, unsigned char max_y>
void LiteGameEngine::TetrisBody<max_x, max_y>::reassign(const BodyType& type)
{
	this->type = type;
	//assign collider
	for (unsigned char i = 0; i < TETRIS_COLLIDER_LENGTH; i++)
	{
		collier[i] = (colliders[type])[i];
	}
	current_position = { initial_x, initial_y };
	current_rot = UP;
}

template<unsigned char max_x, unsigned char max_y>
void LiteGameEngine::TetrisBody<max_x, max_y>::rotate(TetrisCollider& collider, const Rotation& rot, const BodyType& type)
{
	for (unsigned char i = 0; i < TETRIS_COLLIDER_LENGTH; i++)
	{
		unsigned char x, y;
		i2xy(i, x, y);
		unsigned char j;
#define WIDTH 4
		switch (rot)
		{
		case UP:
			j = i;
			break;
		case RIGHT:
			j = (WIDTH * (WIDTH - 1)) + y - (WIDTH * x);
			break;
		case DOWN:
			j = (WIDTH * WIDTH) - 1 - (WIDTH * y) - x;
			break;
		case LEFT:
			j = WIDTH - 1 + y + (WIDTH * x);
			break;
		}
#undef WIDTH
		collider[i] = (colliders[type])[j];
	}
}



template<unsigned char max_x, unsigned char max_y>
LiteGameEngine::TetrisBody<max_x, max_y>::TetrisBody(const BodyType& type, const Position2D& initial_pos, const Rotation& initial_rot) :
	type(type), current_position(initial_position), current_rot(initial_rot)
{
	collider = new TetrisCollider(colliders[type]);
}

template<unsigned char max_x, unsigned char max_y>
LiteGameEngine::TetrisBody<max_x, max_y>::TetrisBody(const BodyType& type) :
	type(type), current_position({ initial_x, initial_y }), current_rot(UP)
{
	collider = new TetrisCollider(colliders[type]);
}

template<unsigned char width, unsigned char height>
LiteGameEngine::TetrisField<width, height>::TetrisField()//:
	//all_colliders()
{
	assign_empty_field(collider);
}

//template<unsigned char width, unsigned char height>
//void LiteGameEngine::TetrisField<width, height>::update_collider(const TetrisCollider& tetris_col, 
//	const BodyType& t, const Position2D& pos)
//{
//#pragma region deprecated
//	//for (unsigned i = 0; i < LENGTH; i++)
//	//{
//	//	this->collider[i] = false;
//	//}
//	////collider is now all false
//
//	//for (unsigned int i = 0; i < all_colliders.size(); i++)
//	//{
//	//	TetrisBody* & col = all_colliders[i];
//	//	char rounded_y = TMath::round_nearest(col->current_position.y);
//	//	char rounded_x = TMath::round_nearest(col->current_position.x);
//	//	for (unsigned char j = 0; j < TETRIS_COLLIDER_LENGTH; j++)
//	//	{
//	//		char global_y;
//	//		char global_x;
//	//		col->i2xy(j, global_x, global_y);
//	//		global_y += rounded_y;
//	//		global_x += rounded_x;
//	//		this->collider[this->xy2i(global_x, global_y)] |= (*col)[j]; //Assign true only
//	//	}
//	//}
//	////Collider consists of only tetris bricks
//	//assign_border(this->collider);
//
//#pragma endregion
//
//	//tetris_col is being added to the grid, no need to check.
//	unsigned char global_x = TMath::round_nearest(pos.x);
//	unsigned char global_y = TMath::round_nearest(pos.y);
//	for (unsigned char i = 0; i < TETRIS_COLLIDER_LENGTH; i++)
//	{
//		TetrisBody
//	}
//}

template<unsigned char width, unsigned char height>
unsigned char LiteGameEngine::TetrisField<width, height>::update_collider(const TetrisBody<width, height>& body)
{
	//update the collider
	unsigned char rounded_x = TMath::round_nearest(body.current_position.x),
		rounded_y = TMath::round_nearest(body.current_position.y);
	for (unsigned char i = 0; i < TETRIS_COLLIDER_LENGTH; i++)
	{
		if (body.collider[i]) //If there is collider
		{
			unsigned char global_x, global_y;
			body.i2xy(i, global_x, global_y);
			global_x += rounded_x;
			global_y += rounded_y;
			this->collider[xy2i(global_x, global_y)] = body.type;
		}
	}
	//look for burn signals
	unsigned char burned_rows = 0;
	for (unsigned char y = height - 1; y >= 0;//y--
		) //From bot to top
	{
		bool delete_row = true;
		for (unsigned char x = 1; x < width; x++)
		{
			if (col[xy2i(x, y)] == BodyType::BLANK)
			{
				delete_row = false;
				break;
			}
		}
		if (delete_row)
		{
			burned_rows++;
			//Everything except highest
			for (unsigned char _y = y; _y > 0; _y--)
			{
				for (unsigned char _x = 1; _x < width; _x++)
				{
					//TODO: DEBUG: blame this if burn algorithm doesn't yield good data
					//col[xy2i(_x, _y)] = col[xy2i(_x,_y-1)];
					unsigned char this_index = xy2i(_x, _y);
					col[this_index] = col[this_index - WIDTH];
				}
			}
			//highest (y = 0)
			for (unsigned char _x = 1; _x < width; _x++)
			{
				//unsigned char this_index = xy2i(_x, 0);
				col[_x] = BodyType::BLANK;
			}

		}
		else
		{
			y--; //Since row is not deleting, it should go up
		}
	}
	return burned_rows;
}

template<unsigned char width, unsigned char height>
void LiteGameEngine::TetrisField<width, height>::assign_empty_field(FieldCollider& col)
{
	for (unsigned i = 0; i < LENGTH; i++)
	{
		col[i] = BodyType::BLANK;
	}
	assign_border(col);
}
template<unsigned char width, unsigned char height>
void LiteGameEngine::TetrisField<width, height>::assign_border(FieldCollider& col)
{
	//Border instantializing
	for (unsigned char y = 0; y < HEIGHT; y++)
	{
		col[xy2i(0, y)] = BodyType::BORDER;
		col[xy2i(WIDTH - 1)] = BodyType::BORDER;
	}
	for (unsigned char x = 1; x < width; x++)
	{
		col[xy2i(x, height)] = BodyType::BORDER;
	}
}
#pragma region deprecated
//template<unsigned char width, unsigned char height>
//bool LiteGameEngine::TetrisField<width, height>::try_add_body(const TetrisBody<width, height>& body)
//{
//	if (collider_fit(body.collider, body.current_position, this))
//	{
//		this->all_colliders.push_back(body);
//		return true;
//	}
//	return false;
//}
#pragma endregion



template<unsigned char w, unsigned char h>
bool LiteGameEngine::collider_fit(const TetrisCollider& col, const Position2D& new_position, TetrisField<w, h>& field)
{
	char rounded_x = (char)(new_position.x);
	char rounded_y = (char)(new_position.y);
	for (unsigned char i = 0; i < TETRIS_COLLIDER_LENGTH; i++)
	{
		if (col[i]) //Ignore if there is not a collider there
		{
			char global_y;
			char global_x;
			TetrisBody<w, h>::i2xy(i, global_x, global_y);
			global_y += rounded_y;
			global_x += rounded_x;
			if (
				global_x <= 0 || global_x >= field.WIDTH - 1 || global_y <= 0 || global_y >= h-1 //There is a collision out of bound
				|| field.collider[field.xy2i(global_x, global_y)] != BodyType::BLANK //the field already have a collider there
				)
			{
				return false;
			}
		}
	}
	return true;
}

template<unsigned char w, unsigned char h>
bool LiteGameEngine::try_rotate(TetrisBody<w, h>& body, TetrisField<w, h>& field)
{
	TetrisBody<w, h>::Rotation new_rot = (body.current_rot + 1) % 4;
	TetrisCollider temp;
	LiteGameEngine::TetrisBody<w, h>::rotate(temp, new_rot, body.type);
	if (collider_fit(temp, body.current_position, field))
	{
		body.current_rot = new_rot;
		body.collider = temp;
		return true;
	}
	return false;
}
