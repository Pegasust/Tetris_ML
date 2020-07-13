/**
 * Updating this game in very small values will result in
 * no apparent difference in the change in velocity.
 */

#pragma once

#include "tetris_pieces.h"
#include "game_specs.h"
#include "../common/assertion.hpp"
#include "verbosity/framerate.h"
#include "game_input.h"
#include <list>
#include <queue>
#include <ostream>
namespace Tetris
{
	using Level = unsigned long long;
	constexpr Level MAX_LEVEL = std::numeric_limits<Level>::max();
	constexpr Level MIN_LEVEL = std::numeric_limits<Level>::min();;
    constexpr bool v = std::numeric_limits<Level>::is_integer;
	constexpr double v_fall0 = 
#ifdef V_FALL_0
		V_FALL_0
#else
		10.0 //blocks per second
#endif
	;
	constexpr double v_fall_max =
#ifdef V_FALL_MAX
		V_FALL_MAX
#else
		98.0 //blocks per second
#endif
		;
	constexpr double v_slope = (v_fall_max - v_fall0) / (MAX_LEVEL - MIN_LEVEL);

	constexpr double v_fall_at(const Level& level)
	{
		return v_slope * level + v_fall0;
	}
    constexpr double min_dragdown_gravity_mult = (1.0 / 30.0);

	const unsigned long long n_rows0 = 7;
	constexpr unsigned long long n_rows_at(const Level& level)
	{
		return n_rows0 + (level / 3);
	}

	using namespace Tetris;
	class GameModule
	{
	public:
        typedef std::queue<BodyType> ComingPieces;
		Level current_level;
		unsigned long long n_rows;//The amount of rows (scaled) needs to be burned until level up
		static const unsigned char WIDTH = 10, HEIGHT = 20; //Field specifications
		static const unsigned char N_PIECE_AHEAD = 3;
		static constexpr double dragdown_gravity_mult = 4.0;
		static constexpr double dragup_gravity_mult = 0.5;
		TetrisField game_field;
		Common::ZMath::UInt64RNG current_seed;
		TetrisBody controlling_piece;
		ComingPieces coming_pieces;
		double score;
		bool lost;

		//metadata
		double highest_score;
		unsigned long long n_rows_burned;
		unsigned int tetris_scored;
		unsigned char n_maxstage;
	public:
		//Returns the y value where the piece would fall to

		//Returns the distance in y-axis from the current position to the contacting position
		double down_cast(TetrisBody const& body, TetrisField const& field) const;

		//Returns -1.0 if game should be over
		double down_cast(TetrisCollider const& col, double const& x_pos, TetrisField const& field) const;
		double down_cast_from_top(TetrisBody const& body, TetrisField const& field) const;
		//returns true if input yields result different from inputting NONE
		//bool try_update(const Input& info, double const& seconds_since_last_update);

		////Return if it's a useful input
		//bool fast_deprecated_try_update(Input const& input,
		//	unsigned char& n_burned,
		//	unsigned char& burn_y,
		//	bool const& reassign_controlling_piece,
		//	double const& seconds_since_last_update,
		//	double const pre_static_threshold = 0.8
		//);

        /*
		 * Updates the state of the game.
		 *
		 * Args:
			 * In:
			 * reassign_controlling_piece: whether to reassign controlling_piece
			 if the controlling_piece staticizes.
			 * pre_static_threshold: the threshold that controlling_piece should
			 pass before it is staticized. Example: colliding piece from field
			 is at y = 5; controlling_piece should be at y=5.8 or more (if
			 threshold = 0.8) so that controlling_piece is staticized.
			 * delta_seconds: the number of seconds has passed since the last
			 update.
			 * Out:
             * staticize_piece: whether the update staticized controlling_piece.
			 * burn_y: the y-coordinates that were burned in this update
			 (filled from index 0 to 3).
			 * n_burned: the number of rows that were burned in this update
			 (useful to check for burn_y)


		*/
		void update(Input const& input,
			unsigned char burn_y[4],
			unsigned char& n_burned,
			bool const& reassign_controlling_piece,
			double const& delta_seconds,
			bool& staticize_piece,
			double const pre_static_threshold = 0.8
		);		
		inline void update(Input const& input,
			unsigned char burn_y[4],
			unsigned char& n_burned,
			bool const& reassign_controlling_piece,
			double const& delta_seconds,
			double const pre_static_threshold = 0.8
		)
		{
			static bool ignored_staticize;
			return update(input, burn_y, n_burned, reassign_controlling_piece,
				delta_seconds, ignored_staticize, pre_static_threshold);
		}

		inline void reassign()
		{
			controlling_piece.reassign(coming_pieces.front());
			coming_pieces.pop();
			coming_pieces.push(Tetris::body_type_val(current_seed.get_value()));
		}
		GameModule(const unsigned long long& initial_seed);
	private:
		unsigned char calculate_scaled_burn_score(unsigned char const& burned) const;
		void handle_input(const Tetris::Input& input, bool& useful_input, double& gravity_mult);
		void handle_input(const Tetris::Input& input, double& gravity_mult);
		constexpr double score_displacement(unsigned char const& scaled_burn)
		{
			return (static_cast<double>(current_level) + 1.0) / 700.0 + (static_cast<double>(scaled_burn * (current_level + 1)) / 125.0);
		}
		constexpr double level_up_score_bonus()
		{
			return static_cast<double>(current_level + 1.0) / 100.0;
		}
		constexpr bool lose_check(const unsigned char& n_burned, const TetrisField& game_field,
			const BodyType& upcoming_type)
		{
			return n_burned < 4 && !game_field.check_collider(Tetris::TetrisBody::colliders
				[Tetris::TetrisBody::get_min_index(upcoming_type)],
				{ TetrisBody::initial_x, TetrisBody::initial_y }
			);
		}
};

}