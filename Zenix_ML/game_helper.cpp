#include "game_helper.h"

bool ZenixAgent::attempt_move_piece(TModule::LiteModule& mod, const char& x, const unsigned char& y, TEngine::Rotation const& rotation, unsigned char & burned)
{
	//rot upd
	UpdateInfo rotate_inf;
	rotate_inf.input = TModule::ROTATE;
	if (rotation < mod.controlling_piece.current_rot) //0 < 3
	{
		rotate_inf.n_frames_update = rotation * 4 - mod.controlling_piece.current_rot;
	}
	else
	{
		rotate_inf.n_frames_update = rotation - mod.controlling_piece.current_rot;
	}

	if (!mod.try_update(rotate_inf)) return false;

	//x upd
	UpdateInfo temp_inf;
	temp_inf.n_frames_update = abs(mod.controlling_piece.current_position.x - x);
	if (x > mod.controlling_piece.current_position.x) //Desired position is to the right
	{
		temp_inf.input = TModule::Input::LEFT;
	}
	else
	{
		temp_inf.input = TModule::Input::RIGHT;
	}
	if (!mod.try_update(temp_inf)) return false;
	
	//y upd
	temp_inf.n_frames_update = 1;
	temp_inf.input = TModule::Input::DOWN;
	//if (!mod.try_update(temp_inf)) return false;
	bool game_not_over;
	burned = LiteAPI::simulate(mod, game_not_over, temp_inf);
	if (!game_not_over) return false;
	return true;
}


bool ZenixAgent::apply_moveset(TModule::LiteModule& mod, const char& x, const unsigned char& y, TEngine::Rotation const& rotation)
{
	//rot upd
	UpdateInfo rotate_inf;
	rotate_inf.input = TModule::ROTATE;
	if (rotation < mod.controlling_piece.current_rot) //0 < 3
	{
		rotate_inf.n_frames_update = rotation * 4 - mod.controlling_piece.current_rot;
	}
	else
	{
		rotate_inf.n_frames_update = rotation - mod.controlling_piece.current_rot;
	}

	if (!mod.try_update(rotate_inf)) return false;

	//x upd
	UpdateInfo temp_inf;
	temp_inf.n_frames_update = abs(mod.controlling_piece.current_position.x - x);
	if (x > mod.controlling_piece.current_position.x) //Desired position is to the right
	{
		temp_inf.input = TModule::Input::LEFT;
	}
	else
	{
		temp_inf.input = TModule::Input::RIGHT;
	}
	if (!mod.try_update(temp_inf)) return false;

	//y upd
	temp_inf.n_frames_update = 1;
	temp_inf.input = TModule::Input::DOWN;
	//if (!mod.try_update(temp_inf)) return false;
	//bool game_not_over;
	//burned = LiteAPI::simulate(mod, game_not_over, temp_inf);
	//if (!game_not_over) return false;
	return mod.try_update(temp_inf);
}

bool ZenixAgent::can_move_piece(const TModule::LiteModule& mod, const char& x, const unsigned char& y, TEngine::Rotation const& rotation)
{
	//Unoptimized way. Probably can use math.
	auto temp_mod = mod;

	//check for rotation
	if (temp_mod.controlling_piece.current_rot != rotation) //only check when rotation is different
	{
		//TEngine::Rotation rot;
		UpdateInfo temp_inf;
		temp_inf.input = TModule::ROTATE;
		if (rotation < temp_mod.controlling_piece.current_rot) //0 < 3
		{
			temp_inf.n_frames_update = rotation * 4 - temp_mod.controlling_piece.current_rot;
		}
		else
		{
			temp_inf.n_frames_update = rotation - temp_mod.controlling_piece.current_rot;
		}


		bool game_not_over = true;
		if (LiteAPI::simulate(temp_mod, game_not_over, temp_inf) == -1 //input does not yield anything helpful
			|| !game_not_over //game over
			// Maybe for some reason controlling_piece.current_rot != rotation?
			)
		{
			return false;
		}
	}

	//check for X
	if ((char)temp_mod.controlling_piece.current_position.x != x) //Only see if the x value is different
	{
		UpdateInfo temp_inf;
		temp_inf.n_frames_update = abs(temp_mod.controlling_piece.current_position.x - x);
		if (x > temp_mod.controlling_piece.current_position.x) //Desired position is to the right
		{
			temp_inf.input = TModule::Input::LEFT;
		}
		else
		{
			temp_inf.input = TModule::Input::RIGHT;
		}
		//TODO: Can have a side-cast here to prevent multiple instances of a same state.
		bool game_not_over = true;
		if (LiteAPI::simulate(temp_mod, game_not_over, temp_inf) == -1 //input does not yield anything helpful
			|| !game_not_over //game over
			// Maybe for some reason controlling_piece.position.x != x?
			)
		{
			return false;
		}
	}
	//THIS POINT: can achive same x

	//check for same y
	if ((unsigned char)temp_mod.controlling_piece.current_position.y != y) //only check when y value is different
	{
		if ((unsigned char)LiteAPI::down_cast(temp_mod.controlling_piece, temp_mod.field) != y)
		{
			return false;
		}
	}
	return true;
}

ZenixAgent::RawObservation ZenixAgent::get_raw_observation(const TEngine::TetrisField& field)
{
	RawObservation return_value = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	unsigned char last_top_y = field.FIELD_BOTTOM;
	for (unsigned char x = field.FIELD_LEFT; x <= field.FIELD_RIGHT; x++)
	{
		unsigned char current_top_y = field.FIELD_BOTTOM;
		bool previously_occupied = false;
		for (unsigned char y = field.FIELD_TOP; y <= field.FIELD_BOTTOM; y++)
		{
			if (field.collider[field.xy2i(x, y)] != TEngine::BLANK) //When that collider is not blank
			{				
				if (current_top_y > y) //If the current y is less than current top record					
				{
					current_top_y = y;
				}
				previously_occupied = true;
			}
			else
			{
				//hole check
				if (previously_occupied) //If the upper tile is occupied
				{
					return_value.holes++; //HOLES COMPLETED
					previously_occupied = false;
				}
			}
		}
		return_value.aggregate_height += (field.FIELD_BOTTOM - current_top_y); //AGGREGATE HEIGHT COMPLETED
		if (x != field.FIELD_LEFT) //Ignore the first column because there is no data from its left
		{
			return_value.bulkiness += abs(current_top_y - last_top_y); //BULKINESS COMPLETED
		}
		last_top_y = current_top_y;
	}
	return return_value;
}
