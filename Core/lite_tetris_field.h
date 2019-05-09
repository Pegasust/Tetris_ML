#pragma once
#include "lite_tetris_body.h"
namespace LGEngine
{
	class TetrisField
	{
	public:
		static constexpr unsigned char WIDTH = LGEngine::FIELD_WIDTH + 2;
		static constexpr unsigned char HEIGHT = LGEngine::FIELD_HEIGHT + 1;
		static constexpr unsigned char LENGTH = WIDTH * HEIGHT;

		static constexpr unsigned char FIELD_BOTTOM = LGEngine::FIELD_HEIGHT - 1;
		static constexpr unsigned char FIELD_TOP = 0;
		static constexpr unsigned char FIELD_LEFT = 1;
		static constexpr unsigned char FIELD_RIGHT = LGEngine::FIELD_WIDTH;
		
		typedef BodyType FieldCollider[LENGTH];
		FieldCollider collider;

		TetrisField();
		//Returns the number of rows burned
		unsigned char update_collider(const TetrisBody body); //Trust me and just update without check.
		bool check_collider(const TetrisCollider& col, const Position2D& new_position) const;
		bool check_collider(const TetrisBody& body) const;
		static void assign_empty_field(FieldCollider& col);
		static void assign_border(FieldCollider& col);
		static constexpr unsigned char xy2i(const unsigned char& x, const unsigned char& y);
		static constexpr void i2xy(const unsigned char& i, unsigned char& x, unsigned char & y);
	};
	bool try_rotate(TetrisBody& collider, const LGEngine::Rotation& new_rot, const TetrisField& field);
}