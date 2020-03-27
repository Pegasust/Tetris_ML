#include "game_module.h"

double Tetris::GameModule::down_cast(TetrisBody const& body, TetrisField const& field) const {
    // return down_cast(body.collider, body.current_position.x, field);

#define DNE Tetris::T_COLLIDER_HEIGHT + 1
    unsigned char lowest_y[4] = {DNE, DNE, DNE, DNE}; // The lowest y-value of each x
    /*
    Example:
    -x--  0
    -xx-  1
    --x-  2
    ----  3
    would have DNE, 1, 2, DNE
    */
    // Find lowest_y
#define TETRIMINO_WIDTH 4
    for (int x = 0; x < TETRIMINO_WIDTH; x++) {
        for (int y = TETRIMINO_WIDTH - 1; y >= 0; y--) {
            unsigned char index = Tetris::TetrisBody::xy2i(x, y);
            if (body.collider[index]) {
                lowest_y[x] = y;
                break;
            }
        }
    }
    int min_dist =
        std::numeric_limits<int>::max(); // doesn't rly matter unless the field is messed up
    // down_cast using math
    for (int i = 0; i < 4; i++) // For each column in current body
    {
        if (lowest_y[i] == DNE) {
            continue; // Pass through DNE values
        }
        //#ifdef USE_ROUND_NEAREST
        //        int field_x = (Tetris::TetrisField::FIELD_LEFT - 1) +
        //                      Common::ZMath::round_nearest(body.current_position.x) + i;
        //        int field_y = Common::ZMath::round_nearest(body.current_position.y) + lowest_y[i]
        //        + 1;
        //#else
        //        int field_x = (Tetris::TetrisField::FIELD_LEFT - 1) + (int)body.current_position.x
        //        + i; int field_y = (int)body.current_position.y + lowest_y[i] + 1;
        //#endif
        int field_x = (Tetris::TetrisField::FIELD_LEFT - 1) +
                      RoundingExt::position_round<int>(body.current_position.x) + i;
        // 1 block under lowest y
        int field_y = RoundingExt::position_round<int>(body.current_position.y) + lowest_y[i] + 1;
        int dist = 0;
        for (; field_y <= Tetris::TetrisField::FIELD_BOTTOM // Till bot
               && dist < min_dist                           // if dist is more than min_dist, just
             // discard the cast down for this x-value.
             ;
             field_y++) {
            if (field.collider[Tetris::TetrisField::xy2i(field_x, field_y)] !=
                Tetris::BodyType::BLANK) {
                // Collision happened
                break;
            }
            dist += 1;
        }
        if (dist < min_dist) {
            min_dist = dist;
        }
    }
    // min_dist is the minimum blocks of space between the checking body and
    // the field (all of min_dist blocks between those are blanks)
    // Ex:
    /* min_dist = 2
    - x - - -
    - x x - -
    - - x - -
    - - - - -
    - - - - -
    z - z - z
    z z z z z
    */
    return static_cast<double>(min_dist) - (body.current_position.y - floor(body.current_position.y));
    //return static_cast<double>(min_dist) +
    //       (ceil(body.current_position.y) - body.current_position.y);
#undef TETRIMINO_WIDTH
#undef DNE
}

double Tetris::GameModule::down_cast(TetrisCollider const& col, double const& x_pos,
                                     TetrisField const& field) const {
    double return_val = -1.0;
    while (field.check_collider(col, {x_pos, return_val + 1.0})) {
        return_val += 1.0;
    }
    return return_val;
}

double Tetris::GameModule::down_cast_from_top(TetrisBody const& body,
                                              TetrisField const& field) const {
    return down_cast(body.collider, body.current_position.x, field);
}

// bool Tetris::GameModule::try_update(const Input& info, double const& seconds_since_last_update)
//{
//	Position2D new_position = controlling_piece.current_position;
//	if (info == Input::CAST_DOWN)
//	{
//		new_position.y = down_cast(controlling_piece, game_field);
//		//goto lose_check
//	}
//	bool return_val = false;
//	switch (info)
//	{
//	//case CAST_DOWN:
//	//	new_position.y = down_cast(controlling_piece, game_field);
//	//	if (new_position.y == -1.0) //If the game is supposed to be lost
//	//	{
//	//		lost = true;
//	//		return false; //it doesn't matter, really
//	//	}
//	//	break;
//
//	case LEFT:
//		if (/*LGEngine::collider_fit(controlling_piece.collider,
//			{new_position.x-1.0, new_position.y}, field)*/
//			game_field.check_collider(controlling_piece.collider, { new_position.x - 1.0,
// new_position.y
//})
//			)
//		{
//			new_position.x = new_position.x - 1.0;
//			return_val = true;
//		}
//		break;
//	case RIGHT:
//		if (/*LGEngine::collider_fit(controlling_piece.collider,
//			{ new_position.x + 1.0, new_position.y }, field)*/
//			game_field.check_collider(controlling_piece.collider, { new_position.x + 1.0,
// new_position.y
//})
//			)
//		{
//			new_position.x = new_position.x + 1.0;
//			return_val = true;
//		}
//		break;
//	case ROTATE:
//		return_val = try_rotate(controlling_piece, (controlling_piece.current_rot + 1) % 4,
// game_field); 		break;
//	}
//
//	double fall_displacement = v_fall_at(current_level) * seconds_since_last_update;
//	if (info == Input::DOWN)
//	{
//		fall_displacement *= 1.5;
//		return_val = true;
//	}
//	else if(info == Input::UP)
//	{
//		fall_displacement *= 0.8;
//		return_val = true;
//	}
//
//#ifdef USE_DEPRECATED_FALL_COLLISION_CHECK
//	//This uses less computational power at the cost of risk of pieces clipping through.
//	//This is only safe when the fall_displacement is within 1.0
//	new_position.y += fall_displacement;
//	if (!game_field.check_collider(controlling_piece.collider, new_position))
//#else
//	double down_cast_y = down_cast(controlling_piece, game_field);
//	if(static_cast<int> (new_position.y += fall_displacement) <= static_cast<int> (down_cast_y))
//#endif
//		//if the controlling piece collides with something in this update
//	{
//		controlling_piece.current_position.y = down_cast_y;
//		unsigned char rows_burned = game_field.update_collider(controlling_piece);
//		unsigned char scaled_burned = 0;
//		if (rows_burned > 0)
//		{
//			//calculate scaled burned for score increment and required rows reduction
//			scaled_burned = calculate_scaled_burn_score(rows_burned);
//			if (scaled_burned < n_rows) //if this update doesn't yield promotion
//			{
//				n_rows -= scaled_burned;
//			}
//			else
//			{
//				//promote
//				n_rows += n_rows_at(current_level) - scaled_burned; //+= to make sure it's positive
//				score += level_up_score_bonus();
//			}
//			//increment to score
//			score += score_displacement(scaled_burned);
//		}
//
//
//	}
//}

// bool Tetris::GameModule::fast_deprecated_try_update(Input const& input, unsigned char& n_burned,
// unsigned char& burn_y, bool const& reassign_controlling_piece, double const&
// seconds_since_last_update, double const pre_static_threshold)
//{
//	double gravity_mult = 1.0;
//	bool useful_input = false;
//	handle_input(input, useful_input, gravity_mult);
//	//Handle the fact that the tetrimino is falling down
//	if (input != Input::CAST_DOWN)
//	{
//		double gravity_displacement = v_fall_at(current_level) * seconds_since_last_update *
// gravity_mult; 		double dist = down_cast(controlling_piece, game_field); 		if
// (gravity_displacement < (dist + pre_static_threshold)) //This one is not going to place it down
//		{
//			//score -= useless_move_decrement(current_level);
//			n_burned = 0;
//			controlling_piece.current_position.y += gravity_displacement;
//			return useful_input; //DO NOT STATICIZE THE PIECE
//		}
//		else
//		{
//			controlling_piece.current_position.y += gravity_displacement;
//		}
//	}
//	//Reachable only when the current piece should be staticized
//	//modify score & level up if necessary
//	n_burned = game_field.update_collider(controlling_piece, burn_y);
//	unsigned char scaled_burned = 0;
//	if (n_burned > 0)
//	{
//		n_rows_burned += n_burned;
//		if (n_burned == 4)
//		{
//			tetris_scored++;
//		}
//		//scoring and levelling up
//		scaled_burned = calculate_scaled_burn_score(n_burned);
//		if (scaled_burned < n_rows)
//		{
//			n_rows -= scaled_burned;
//		}
//		else
//		{
//			//level up
//			n_rows += calculate_scaled_burn_score(++current_level) - scaled_burned;
//			score += static_cast<double>(current_level + 1.0) / 100.0;
//		}
//	}
//	score += score_displacement(scaled_burned);
//	if (highest_score < score) highest_score = score;
//	if (n_burned < 4 &&
//		!game_field.check_collider(
//			Tetris::TetrisBody::colliders[
//				Tetris::TetrisBody::get_min_index(
//		coming_pieces.front())], { Tetris::TetrisBody::initial_x, Tetris::TetrisBody::initial_y }))
////game over
//	{
//		lost = true;
//		return useful_input;
//	}
//	if (reassign_controlling_piece)
//	{
//		controlling_piece.reassign(coming_pieces.front());
//		coming_pieces.pop();
//		coming_pieces.push(Tetris::body_type_val(current_seed.get_value()));
//	}
//	return useful_input;
//}

void Tetris::GameModule::update(Input const& input, unsigned char burn_y[4],
                                unsigned char& n_burned, bool const& reassign_controlling_piece,
                                double const& delta_seconds, bool& staticize_piece,
                                double const pre_static_threshold) {
    using namespace Common;
    ASSERT(!lost, "Attempted to call update when module is lost!");
    double gravity_mult = 1.0;
    handle_input(input, gravity_mult);

    if (input != Input::CAST_DOWN) {
        // The distance that is pulled down by gravity
        double gravity_vel = v_fall_at(current_level) * gravity_mult;
        //#ifdef DEBUG_DEFINED
        //        if (input != Input::NONE) {
        //
        //            VERBOSITY_LOG("Input: " + std::to_string(input));
        //            VERBOSITY_LOG("Gravity Velocity: " + std::to_string(gravity_vel));
        //        }
        //#endif
        double gravity_displacement = delta_seconds * gravity_vel;
        //#ifdef DEBUG_DEFINED
        //        if (input != Input::NONE) {
        //            VERBOSITY_LOG("gravity_disp: " + std::to_string(gravity_displacement) + " = "
        //            +
        //                          std::to_string(delta_seconds*1000.0) + " * " +
        //                          std::to_string(gravity_vel/1000.0));
        //        }
        //#endif
        // The distance from controlling piece to the bottom of game_field
        // (from the bottom-most non-zero tile of controlling piece
        // to the top of non-zero tile of game_field in the same column
        // as the bottom-most non-zero tile)
        double dist = down_cast(controlling_piece, game_field);
        if (gravity_displacement < (dist + pre_static_threshold)) {
            // if displacement from gravity is not enough to put the piece
            // in such position to be staticized
            n_burned = 0;
            controlling_piece.current_position.y += gravity_displacement;
            staticize_piece = false;
            return;
        }
        // displacement from gravity should be sufficient for this piece
        // to be staticized
        staticize_piece = true;
        // controlling_piece.current_position.y += gravity_displacement;
        controlling_piece.current_position.y += dist;
    } else {
        staticize_piece = true;
    }
    game_field.update_collider(controlling_piece, burn_y, n_burned);
    unsigned char scaled_burned = 0;
    if (n_burned > 0) {
        n_rows_burned += n_burned;
        if (n_burned == 4) {
            tetris_scored++;
        }
        // scoring and levelling up
        scaled_burned = calculate_scaled_burn_score(n_burned);
        if (scaled_burned < n_rows) {
            n_rows -= scaled_burned;
        } else {
            // level up
            if (current_level == MAX_LEVEL) {
                n_maxstage++;
            }
            n_rows += calculate_scaled_burn_score(++current_level) - scaled_burned;
            score += static_cast<double>(current_level + 1.0) / 100.0;
        }
    }
    score += score_displacement(scaled_burned);
    if (highest_score < score)
        highest_score = score;
    lost = lose_check(n_burned, game_field, coming_pieces.front());
    if (reassign_controlling_piece) {
        reassign();
    }
}

void Tetris::GameModule::handle_input(const Tetris::Input& input, bool& useful_input,
                                      double& gravity_mult) {
    switch (input) {
    case CAST_DOWN: {
        double dist = down_cast(controlling_piece, game_field);
        controlling_piece.current_position.y += dist;
        useful_input = true;
    } break;
    case LEFT: {
        if (game_field.check_collider(controlling_piece.collider,
                                      {controlling_piece.current_position.x - 1.0,
                                       controlling_piece.current_position.y})) {
            controlling_piece.current_position.x -= 1.0;
            useful_input = true;
        } else {
            useful_input = false;
        }
    } break;
    case RIGHT: {
        if (game_field.check_collider(controlling_piece.collider,
                                      {controlling_piece.current_position.x + 1.0,
                                       controlling_piece.current_position.y})) {
            controlling_piece.current_position.x += 1.0;
        } else {
            useful_input = false;
        }
    } break;
    case ROTATE_FORTH: {
        useful_input = Tetris::try_rotate(controlling_piece,
                                          (controlling_piece.current_rot + 1) % 4, game_field);
    } break;
    case ROTATE_BACK: {
        useful_input = Tetris::try_rotate(controlling_piece,
                                          (controlling_piece.current_rot - 1) % 4, game_field);
    }
    case DOWN: {
        gravity_mult = dragdown_gravity_mult;
        useful_input = true;
    } break;
    case UP: {
        gravity_mult = dragup_gravity_mult;
        useful_input = true;
    } break;
    }
}

void Tetris::GameModule::handle_input(const Tetris::Input& input, double& gravity_mult) {
    switch (input) {
    case CAST_DOWN: {
        double dist = down_cast(controlling_piece, game_field);
        controlling_piece.current_position.y += dist;
    } break;
    case LEFT: {
        if (game_field.check_collider(controlling_piece.collider,
                                      {controlling_piece.current_position.x - 1.0,
                                       controlling_piece.current_position.y})) {
            controlling_piece.current_position.x -= 1.0;
        }
    } break;
    case RIGHT: {
        if (game_field.check_collider(controlling_piece.collider,
                                      {controlling_piece.current_position.x + 1.0,
                                       controlling_piece.current_position.y})) {
            controlling_piece.current_position.x += 1.0;
        }
    } break;
    case ROTATE_FORTH: {
        Tetris::try_rotate(controlling_piece, (controlling_piece.current_rot + 1) % 4, game_field);
    } break;
    case ROTATE_BACK: {
        // For some weird reasons, if the following expression is a one-liner, the compiler
        // will optimize the modulo 4 away
        Rotation new_rot = controlling_piece.current_rot - 1;
        new_rot = new_rot % 4;
        Tetris::try_rotate(controlling_piece, new_rot, game_field);
    } break;
    case DOWN: {
        gravity_mult =
            std::max(dragdown_gravity_mult, v_fall_at(current_level) / min_dragdown_gravity_mult);
    } break;
    case UP: {
        gravity_mult = dragup_gravity_mult;
    } break;
    }
}

Tetris::GameModule::GameModule(const unsigned long long& initial_seed)
    : current_level(MIN_LEVEL), n_rows(n_rows_at(current_level)), game_field(),
      current_seed(initial_seed), controlling_piece(body_type_val(initial_seed)), coming_pieces(),
      score(0.0), lost(false), highest_score(0.0), n_rows_burned(0), tetris_scored(0),
      n_maxstage(0) {
    for (unsigned char i = 0; i < N_PIECE_AHEAD; i++) {
        coming_pieces.push(body_type_val(current_seed.get_value()));
    }
}

unsigned char Tetris::GameModule::calculate_scaled_burn_score(unsigned char const& burned) const {
    return static_cast<double>(burned) * 1.499 + static_cast<double>(current_level) / 15.0;
}
