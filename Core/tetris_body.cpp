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
LiteGameEngine::TetrisBody<max_x, max_y>::TetrisBody(const BodyType& type):
	type(type), current_position({ initial_x, initial_y }), current_rot(UP)
{
	collider = new TetrisCollider(colliders[type]);
}

template<unsigned char width, unsigned char height>
LiteGameEngine::TetrisField<width, height>::TetrisField():
	all_colliders()
{
	assign_empty_field(collider);
}

template<unsigned char width, unsigned char height>
void LiteGameEngine::TetrisField<width, height>::update_collider()
{
	for (unsigned i = 0; i < LENGTH; i++)
	{
		this->collider[i] = false;
	}
	//collider is now all false

	for (unsigned int i = 0; i < all_colliders.size(); i++)
	{
		TetrisBody* & col = all_colliders[i];
		char rounded_y = TMath::round_nearest(col->current_position.y);
		char rounded_x = TMath::round_nearest(col->current_position.x);
		for (unsigned char j = 0; j < TETRIS_COLLIDER_LENGTH; j++)
		{
			char global_y;
			char global_x;
			col->i2xy(j, global_x, global_y);
			global_y += rounded_y;
			global_x += rounded_x;
			this->collider[this->xy2i(global_x, global_y)] |= (*col)[j]; //Assign true only
		}
	}
	//Collider consists of only tetris bricks
	assign_border(this->collider);
}

template<unsigned char width, unsigned char height>
void LiteGameEngine::TetrisField<width, height>::assign_empty_field(FieldCollider& col)
{
	for (unsigned i = 0; i < LENGTH; i++)
	{
		col[i] = false;
	}
	assign_border(col);
}
template<unsigned char width, unsigned char height>
void LiteGameEngine::TetrisField<width, height>::assign_border(FieldCollider& col)
{
	//Border instantializing
	for (unsigned char y = 0; y < HEIGHT; y++)
	{
		col[xy2i(0, y)] = true;
		col[xy2i(WIDTH - 1)] = true;
	}
	for (unsigned char x = 1; x < width; x++)
	{
		col[xy2i(x, height)] = true;
	}
}

template<unsigned char width, unsigned char height>
bool LiteGameEngine::TetrisField<width, height>::try_add_body(const TetrisBody<width, height>& body)
{
	if (collider_fit(body.collider, body.current_position, this))
	{
		this->all_colliders.push_back(body);
		return true;
	}
	return false;
}

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
				|| field[field.xy2i(global_x, global_y)] //the field already have a collider there
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
