#pragma once

#include "tetris_pieces.h"
#include <queue>

namespace Tetris
{
	typedef unsigned long long Level;
	constexpr Level MAX_LEVEL = std::numeric_limits<Level>::max();
	constexpr Level MIN_LEVEL = std::numeric_limits<Level>::min();;

	constexpr double v_fall0 = 10.0; //blocks per second
	constexpr double v_fall_max = 98.0; //blocks per second
	constexpr double v_slope = (v_fall_max - v_fall0) / (MAX_LEVEL - MIN_LEVEL);

	constexpr double v_fall_at(const Level& level)
	{
		return v_slope * level + v_fall0;
	}

	const unsigned long long n_rows0 = 7;
	constexpr unsigned long long n_rows_at(const Level& level)
	{
		return n_rows0 + (level / 3);
	}

	enum Input
	{
		NONE,
		DOWN,
		UP,
		LEFT,
		RIGHT,
		CAST_DOWN,
		ROTATE,
	};
	using namespace Tetris;
	class GameModule
	{
	public:
		Level current_level;
		unsigned long long n_rows;//The amount of rows (scaled) needs to be burned until level up
		static const unsigned char WIDTH = 10, HEIGHT = 20; //Field specifications
		static const unsigned char N_PIECE_AHEAD = 3;
		static constexpr double dragdown_gravity_mult = 4.0;
		static constexpr double dragup_gravity_mult = 0.5;
		TetrisField game_field;
		Common::ZMath::UInt64RNG current_seed;
		TetrisBody controlling_piece;
		std::queue<BodyType> coming_pieces;
		double score;
		bool lost;

		//metadata
		double highest_score;
		unsigned long long n_rows_burned;
		unsigned int tetris_scored;
	public:
		//Returns the y value where the piece would fall to
		double down_cast(TetrisBody const& body, TetrisField const& field) const;

		//Returns -1.0 if game should be over
		double down_cast(TetrisCollider const& col, double const& x_pos, TetrisField const& field) const;
		double down_cast_from_top(TetrisBody const& body, TetrisField const& field) const;
		//returns true if input yields result different from inputting NONE
		//bool try_update(const Input& info, double const& seconds_since_last_update);

		//Return if it's a useful input
		bool try_update(Input const& input,
			unsigned char& n_burned,
			unsigned char& burn_y,
			bool const& reassign_controlling_piece,
			double const& seconds_since_last_update,
			double const pre_static_threshold = 0.8
		);

		GameModule(const unsigned long long& initial_seed);
	private:
		unsigned char calculate_scaled_burn_score(unsigned char const& burned) const;
		constexpr double score_displacement(unsigned char const& scaled_burn) 
		{
			return (static_cast<double>(current_level) + 1.0) / 700.0 + (static_cast<double>(scaled_burn * (current_level + 1)) / 125.0);
		}
		constexpr double level_up_score_bonus()
		{
			return static_cast<double>(current_level + 1.0) / 100.0;
		}
	};
}