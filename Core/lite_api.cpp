#include "lite_api.h"

double LiteAPI::down_cast(LGEngine::TetrisBody const& body, LGEngine::TetrisField const& field)
{
	double return_val = floor(body.current_position.y);
	while (field.check_collider(body.collider, { body.current_position.x, return_val + 1.0 }))
		//While it can still go downwards
	{
		return_val += 1.0;
	}
	return return_val;
}

char LiteAPI::simulate (LiteGameModule::LiteModule& mod, bool& game_not_over, LiteGameModule::InputInfo& info)
{
#ifdef RENDER
	std::cout << "[Simulation]: Input: " << info.input << std::endl;
#ifdef _TRACE
	_getch();	

#endif
	Renderer::RenderUnit rdr(mod);
#endif
	LGEngine::Position2D new_position;
	char total_burns = 0;
	for (; info.n_frames_update > 0; info.n_frames_update--)
	{
		new_position = mod.controlling_piece.current_position;
		//Handle input
		//LTetrisBody* potential_new_body = new LTetrisBody(*controlling_piece);
		switch (info.input)
		{		
		case LiteGameModule::LEFT:
			if (/*LGEngine::collider_fit(controlling_piece.collider,
				{new_position.x-1.0, new_position.y}, field)*/
				mod.field.check_collider(mod.controlling_piece.collider, { new_position.x - 1.0, new_position.y })
				)
			{
				new_position.x = new_position.x - 1.0;
			}
			else
			{
#ifdef RENDER
				std::cout << "Input invalid" << std::endl;
#ifdef _TRACE
				_getch();
#endif
#endif
				return -1;
			}
			break;
		case LiteGameModule::RIGHT:
			if (/*LGEngine::collider_fit(controlling_piece.collider,
				{ new_position.x + 1.0, new_position.y }, field)*/
				mod.field.check_collider(mod.controlling_piece.collider, { new_position.x + 1.0, new_position.y })
				)
			{
				new_position.x = new_position.x + 1.0;
			}
			else
			{
#ifdef RENDER
				std::cout << "Input invalid" << std::endl;
#ifdef _TRACE
				_getch();
#endif
#endif
				return -1;
			}
			break;
		case LiteGameModule::ROTATE:
			if (!LGEngine::try_rotate(mod.controlling_piece, (mod.controlling_piece.current_rot + 1) % 4, mod.field))
			{
				return -1;
#ifdef RENDER
				std::cout << "Input invalid" << std::endl;
#ifdef _TRACE
				_getch();
#endif
#endif
			}
			break;
		}

		//do gravity
		//This is always less than 1.0
		double gravity_displacement = LiteGameModule::v_gravity_at(mod.current_level) * LiteGameModule::seconds_per_update;
		if (info.input != LiteGameModule::DOWN)
		{
			new_position.y += gravity_displacement;
		}
		if /*(!LGEngine::collider_fit(controlling_piece.collider, new_position, field))*/
			(!mod.field.check_collider(mod.controlling_piece.collider, new_position)) //If the thing collides in this update
		{
			//attempt to return to the nearest position
			mod.controlling_piece.current_position.y = floor(new_position.y - gravity_displacement);
			unsigned char burned = mod.field.update_collider(mod.controlling_piece);
			total_burns += burned;
			unsigned char scaled_burned = 0;
			if (burned > 0)
			{
				//scoring and levelling up
				scaled_burned = TMath::round_nearest((double)burned * 1.499 + ((double)mod.current_level / 15.0));
				if (scaled_burned < mod.n_level_up_rows)
				{
					mod.n_level_up_rows -= scaled_burned;
				}
				else
				{
					//level up
					mod.n_level_up_rows += LiteGameModule::required_rows(++mod.current_level) - scaled_burned;
					mod.score += (double)(mod.current_level + 1.0) / 100.0;
				}
			}
			mod.score += ((double)mod.current_level + 1.0) / 700.0 + ((double)(scaled_burned * (mod.current_level + 1)) / 125.0);

			//reassign controlling piece
			//mod.controlling_piece.reassign(mod.coming_pieces.front());
			if (burned < 4 &&/* !LGEngine::collider_fit(controlling_piece.collider, controlling_piece.current_position, field)*/
				!mod.field.check_collider(mod.controlling_piece)
				)
			{
				//game over
				mod.score = std::numeric_limits<double>::lowest();
				game_not_over = false;
				//return total_burns;
			}
			//mod.coming_pieces.pop();
			////generate the next one
			//mod.current_seed = TMath::GameRNG::xorshift64(&mod.current_seed);
			//mod.coming_pieces.push(LGEngine::body_type_val(mod.current_seed));
#ifdef RENDER
			Renderer::clear_console();
			rdr.update_string(mod);
			rdr.render();
			std::cout << "[Simulation]: Input: " << info.input << std::endl;
#ifdef _TRACE
			_getch();
#endif
#endif
			return total_burns;
		}
		else
		{
			//#ifdef _DEBUG
			//			std::cout << "No reassignment.\0" << std::endl;
			//#endif
						//no reassignment needed
			mod.controlling_piece.current_position = new_position;
			//This will encourage ML to press DOWN
			mod.score -= ((double)mod.current_level + 1.0) / 8000.0;
		}
		if (mod.highest_score < mod.score) mod.highest_score = mod.score;
#ifdef RENDER
		Renderer::clear_console();
		rdr.update_string(mod);
		rdr.render();
		std::cout << "[Simulation]: Input: " << info.input << std::endl;
#ifdef _TRACE
		_getch();
#endif
#endif
	}
	//#ifdef _DEBUG
	//	std::cout << "current score: " << std::setprecision(15) <<score << std::endl << "highest score: " << highest_score << '\0' << std::endl;
	//#endif

	return total_burns;
}

char LiteAPI::update_yield_burn(LiteGameModule::InputInfo& info, LiteGameModule::LiteModule& mod)
{
	LGEngine::Position2D new_position;
	char total_burns = 0;
	for (; info.n_frames_update > 0; info.n_frames_update--)
	{
		new_position = mod.controlling_piece.current_position;
		//Handle input
		//LTetrisBody* potential_new_body = new LTetrisBody(*controlling_piece);
		switch (info.input)
		{
		case LiteGameModule::DOWN:
			//potential_new_body->current_position.y = potential_new_body->current_position.y + 1;
			new_position.y = floor(new_position.y);
			while (/*LGEngine::collider_fit(controlling_piece.collider,
				{ new_position.x, new_position.y + 1.0 }, this->field))*/
				mod.field.check_collider(mod.controlling_piece.collider, { new_position.x, new_position.y })
				//While it can still go down
				)
			{
				new_position.y = new_position.y + 1.0;
				//#ifdef _DEBUG
				//				if ((unsigned int)new_position.y ==((unsigned int) HEIGHT * (unsigned int) 3)) //(It's okay dad, I know what I'm doing)
				//				{
				//					throw "Infinite loop from DOWN input.";
				//					break;
				//				}
				//				//std::cout << "DOWN \0" << std::endl;
				//#endif
			}
			//new_position.y += 0.003;
			break;
		case LiteGameModule::LEFT:
			if (/*LGEngine::collider_fit(controlling_piece.collider,
				{new_position.x-1.0, new_position.y}, field)*/
				mod.field.check_collider(mod.controlling_piece.collider, { new_position.x - 1.0, new_position.y })
				)
			{
				new_position.x = new_position.x - 1.0;
			}
			break;
		case LiteGameModule::RIGHT:
			if (/*LGEngine::collider_fit(controlling_piece.collider,
				{ new_position.x + 1.0, new_position.y }, field)*/
				mod.field.check_collider(mod.controlling_piece.collider, { new_position.x + 1.0, new_position.y })
				)
			{
				new_position.x = new_position.x + 1.0;
			}
			break;
		case LiteGameModule::ROTATE:
			LGEngine::try_rotate(mod.controlling_piece, (mod.controlling_piece.current_rot + 1) % 4, mod.field);
			break;
		}

		//do gravity
		//This is always less than 1.0
		double gravity_displacement = LiteGameModule::v_gravity_at(mod.current_level) * LiteGameModule::seconds_per_update;
		if (info.input != LiteGameModule::Input::DOWN)
		{
			new_position.y += gravity_displacement;
		}
		if /*(!LGEngine::collider_fit(controlling_piece.collider, new_position, field))*/
			(!mod.field.check_collider(mod.controlling_piece.collider, new_position)) //If the thing collides in this update
		{
			//attempt to return to the nearest position
			mod.controlling_piece.current_position.y = floor(new_position.y - gravity_displacement);
			unsigned char burned = mod.field.update_collider(mod.controlling_piece);
			total_burns += burned;
			unsigned char scaled_burned = 0;
			if (burned > 0)
			{
				//scoring and levelling up
				scaled_burned = TMath::round_nearest((double)burned * 1.499 + ((double)mod.current_level / 15.0));
				if (scaled_burned < mod.n_level_up_rows)
				{
					mod.n_level_up_rows -= scaled_burned;
				}
				else
				{
					//level up
					mod.n_level_up_rows += LiteGameModule::required_rows(++mod.current_level) - scaled_burned;
					mod.score += (double)(mod.current_level + 1.0) / 100.0;
				}
			}
			mod.score += ((double)mod.current_level + 1.0) / 700.0 + ((double)(scaled_burned * (mod.current_level + 1)) / 125.0);

			//reassign controlling piece
			mod.controlling_piece.reassign(mod.coming_pieces.front());
			//if (burned < 4 &&/* !LGEngine::collider_fit(controlling_piece.collider, controlling_piece.current_position, field)*/
			//	!mod.field.check_collider(mod.controlling_piece)
			//	)
			//{
			//	//game over
			//	mod.score = std::numeric_limits<double>::lowest();
			//	return false;
			//}
			mod.coming_pieces.pop();
			//generate the next one
			mod.current_seed = TMath::GameRNG::xorshift64(&mod.current_seed);
			mod.coming_pieces.push(LGEngine::body_type_val(mod.current_seed));
		}
		else
		{
			//#ifdef _DEBUG
			//			std::cout << "No reassignment.\0" << std::endl;
			//#endif
						//no reassignment needed
			mod.controlling_piece.current_position = new_position;
			//This will encourage ML to press DOWN
			mod.score -= ((double)mod.current_level + 1.0) / 8000.0;
		}
		if (mod.highest_score < mod.score) mod.highest_score = mod.score;
	}
	//#ifdef _DEBUG
	//	std::cout << "current score: " << std::setprecision(15) <<score << std::endl << "highest score: " << highest_score << '\0' << std::endl;
	//#endif
	return total_burns;
}

//char LiteAPI::side_cast(LGEngine::TetrisBody const& body, char const& displacement, LGEngine::TetrisField const& field)
//{
//	for(unsig)
//}
