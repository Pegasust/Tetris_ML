#include "lite_module.h"

bool LiteGameModule::LiteModule::try_update(InputInfo& info)
{
	LGEngine::Position2D new_position;
	for (; info.n_frames_update > 0; info.n_frames_update--)
	{
		new_position = controlling_piece.current_position;
		//Handle input
		//LTetrisBody* potential_new_body = new LTetrisBody(*controlling_piece);
		switch(info.input)
		{
			case DOWN:
				//potential_new_body->current_position.y = potential_new_body->current_position.y + 1;
				new_position.y = floor(new_position.y);
				while (/*LGEngine::collider_fit(controlling_piece.collider, 
					{ new_position.x, new_position.y + 1.0 }, this->field))*/
					field.check_collider(controlling_piece.collider, { new_position.x, new_position.y })
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
			case LEFT:
				if (/*LGEngine::collider_fit(controlling_piece.collider, 
					{new_position.x-1.0, new_position.y}, field)*/
					field.check_collider(controlling_piece.collider, { new_position.x - 1.0, new_position.y })
					)
				{
					new_position.x = new_position.x - 1.0;
				}
				break;
			case RIGHT:	
				if (/*LGEngine::collider_fit(controlling_piece.collider, 
					{ new_position.x + 1.0, new_position.y }, field)*/
					field.check_collider(controlling_piece.collider, { new_position.x + 1.0, new_position.y })
					)
				{
					new_position.x = new_position.x + 1.0;
				}
				break;
			case ROTATE:
				LGEngine::try_rotate(controlling_piece, (controlling_piece.current_rot+1) % 4, field);
				break;
		}
		
		//do gravity
		//This is always less than 1.0
		double gravity_displacement = LiteGameModule::v_gravity_at(current_level) * seconds_per_update;
		if (info.input != Input::DOWN)
		{
			new_position.y += gravity_displacement;
		}
		if /*(!LGEngine::collider_fit(controlling_piece.collider, new_position, field))*/
			(!field.check_collider(controlling_piece.collider, new_position)) //If the thing collides in this update
		{
			//attempt to return to the nearest position
			controlling_piece.current_position.y = floor(new_position.y - gravity_displacement);
			unsigned char burned = field.update_collider(controlling_piece);
			unsigned char scaled_burned = 0;
			if (burned > 0)
			{
				//scoring and levelling up
				scaled_burned = TMath::round_nearest((double)burned * 1.499+((double)current_level/15.0));
				if (scaled_burned < n_level_up_rows)
				{
					n_level_up_rows -= scaled_burned;
				}
				else
				{
					//level up
					n_level_up_rows += required_rows(++current_level) - scaled_burned;
					score += (double)(current_level +1.0)/ 100.0;
				}
			}
			score += ((double)current_level + 1.0) / 700.0 + ((double)(scaled_burned * (current_level+1)) / 125.0) ;

			//reassign controlling piece
			controlling_piece.reassign(coming_pieces.front());
			if (burned < 4 &&/* !LGEngine::collider_fit(controlling_piece.collider, controlling_piece.current_position, field)*/
				!field.check_collider(controlling_piece)
				)
			{
				//game over
				score = std::numeric_limits<double>::lowest();
				return false;
			}
			coming_pieces.pop();
			//generate the next one
			current_seed = TMath::GameRNG::xorshift64(&current_seed);
			coming_pieces.push(LGEngine::body_type_val(current_seed));
		}
		else
		{
//#ifdef _DEBUG
//			std::cout << "No reassignment.\0" << std::endl;
//#endif
			//no reassignment needed
			controlling_piece.current_position = new_position;
			//This will encourage ML to press DOWN
			score -= ((double)current_level + 1.0) / 8000.0;
		}
		if (highest_score < score) highest_score = score;
	}
//#ifdef _DEBUG
//	std::cout << "current score: " << std::setprecision(15) <<score << std::endl << "highest score: " << highest_score << '\0' << std::endl;
//#endif
	return true;
}

LiteGameModule::LiteModule::LiteModule(TMath::GameRNG::RNGSeed initial_seed[1]) :
	current_level(MIN_LEVEL),
	n_level_up_rows(required_rows(current_level)),
	field(),
	current_seed(initial_seed[0]),
	controlling_piece(LGEngine::body_type_val(initial_seed[0])),
	coming_pieces(),
	score(0.0),
	highest_score(0.0)
{
	//controlling_piece = LTetrisBody(LGEngine::rng_seed2bodytype(initial_seed[0]));
	for (unsigned char i = 0; i < 3; i++)
	{
		current_seed = TMath::GameRNG::xorshift64(&current_seed);
		coming_pieces.push(LGEngine::body_type_val(current_seed));
	}
}

//LGEngine::TetrisBody<10, 20> * LiteGameModule::LiteModule::create_new_body(const TMath::GameRNG::RNGSeed & seed)
//{
//	return new LGEngine::TetrisBody<10, 20>(LGEngine::rng_seed2bodytype(seed));
//}


//LiteGameModule::LiteModule::LiteModule():
//	field(), current_seed(TMath::GameRNG::generate_random_seed())
//{
//
//}
//constexpr double LiteGameModule::v_gravity_at(Level level);
//constexpr double LiteGameModule::v_gravity_at(Level level)
//{
//	//Lienar scaling: m * x + b
//	return (v_gravity_max - v_gravity0) / static_cast<double>(MAX_LEVEL - MIN_LEVEL) // slope
//		* level + v_gravity0;
//}
//constexpr unsigned long long LiteGameModule::required_rows(const Level& level);
//constexpr unsigned long long LiteGameModule::required_rows(const Level& level)
//{
//		return required_rows0 + (level / 3);
//}
