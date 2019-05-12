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

	if (!mod.try_update(rotate_inf)) 
		return false;

	//x upd
	UpdateInfo temp_inf;
	temp_inf.n_frames_update = abs(mod.controlling_piece.current_position.x - x);
	if (x > mod.controlling_piece.current_position.x) //Desired position is to the right
	{
		temp_inf.input = TModule::Input::RIGHT;
	}
	else
	{
		temp_inf.input = TModule::Input::LEFT;
	}
	if (!mod.try_update(temp_inf)) 
		return false;
	
	//y upd
	temp_inf.n_frames_update = 1;
	temp_inf.input = TModule::Input::DOWN;
	//if (!mod.try_update(temp_inf)) return false;
	//bool game_not_over;
	//burned = LiteAPI::simulate(mod, game_not_over, temp_inf);
	//if (!game_not_over) 
	//	return false;
	burned = LiteAPI::update_yield_burn(temp_inf, mod);
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
#ifdef RENDER
#define MOD_NAME mod
#define RENDERER_INIT	Renderer::RenderUnit rdr(MOD_NAME);
#define DISPLAY_INPUT_INFO(input_inf) (std::cout<<"Input info: { "<< input_inf.input<<", "<<input_inf.n_frames_update<<" }"<<std::endl)

#ifdef _TRACE
#define PROMPT_USER   std::cout << "Press smt..." << std::endl; _getch();
#else
#define PROMPT_USER
#endif
#define PRINT_LOSE std::cout<<"RENDERER: Lose."<<std::endl; PROMPT_USER

#define SHOW_SCREEN rdr.update_string(MOD_NAME); rdr.render();PROMPT_USER


	RENDERER_INIT;
	Renderer::clear_console();
	DISPLAY_INPUT_INFO(rotate_inf);
#endif
	if (!mod.try_update(rotate_inf))
	{
		return false;
#ifdef RENDER
		PRINT_LOSE;
#endif
	}
#ifdef RENDER
	SHOW_SCREEN;
#endif
	//x upd
	UpdateInfo temp_inf;
	temp_inf.n_frames_update = abs(mod.controlling_piece.current_position.x - x);
	if (x > mod.controlling_piece.current_position.x) //Desired position is to the right
	{
		temp_inf.input = TModule::Input::RIGHT;
	}
	else
	{
		temp_inf.input = TModule::Input::LEFT;
	}
#ifdef RENDER
	Renderer::clear_console();
	DISPLAY_INPUT_INFO(rotate_inf);
#endif
	if (!mod.try_update(temp_inf)) 
	{
#ifdef RENDER
		PRINT_LOSE; 
#endif
		return false;
	}
#ifdef RENDER
	SHOW_SCREEN;
#endif

	//y upd
	temp_inf.n_frames_update = 1;
	temp_inf.input = TModule::Input::DOWN;
	//if (!mod.try_update(temp_inf)) return false;
	//bool game_not_over;
	//burned = LiteAPI::simulate(mod, game_not_over, temp_inf);
	//if (!game_not_over) return false;
	return mod.try_update(temp_inf);
#ifdef RENDER
#undef MOD_NAME
#endif
}

//Reasigns x and y 
bool ZenixAgent::can_move_piece(const TModule::LiteModule& mod, char& x, unsigned char& y, TEngine::Rotation const& rotation)
{
#define END_X_CHECK_OF_CURRENT_ROT x += 10;
#define CORRECT_Y_NEXT_CHECK(desired_y) y = desired_y-1;
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
			temp_inf.input = TModule::Input::RIGHT;
		}
		else
		{
			temp_inf.input = TModule::Input::LEFT;
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
		if ((unsigned char)LiteAPI::down_cast(temp_mod.controlling_piece, temp_mod.field) != y) //There might be a piece above it
		{
			return false;
		}
	}
	return true;
#undef END_X_CHECK_OF_CURRENT_ROT
#undef CORRECT_Y_NEXT_CHECK
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

ZenixAgent::RawObservation& ZenixAgent::RawObservation::operator+=(RawObservation const& rhs)
{
	score += rhs.score;
	bulkiness += rhs.bulkiness;
	burn += rhs.burn;
	aggregate_height += rhs.aggregate_height;
	holes += rhs.holes;
	return *this;
}

ZenixAgent::RawObservation& ZenixAgent::RawObservation::operator/=(RawObservation const& rhs)
{
	score /= rhs.score;
	bulkiness /= rhs.bulkiness;
	burn /= rhs.burn;
	aggregate_height /= rhs.aggregate_height;
	holes /= rhs.holes;
	return *this;
}

ZenixAgent::RawObservation& ZenixAgent::RawObservation::operator/=(double const& rhs)
{
	bulkiness /= rhs;
	burn /= rhs;
	aggregate_height /= rhs;
	holes /= rhs;
	return *this;
}
