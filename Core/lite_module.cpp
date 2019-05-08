#include "lite_module.h"

bool LiteGameModule::LiteModule::try_update(InputInfo& info)
{
	LiteGameEngine::Position2D new_position = controlling_piece.current_position;
	for (; info.n_frames_update > 0; info.n_frames_update--)
	{
		//Handle input
		//LTetrisBody* potential_new_body = new LTetrisBody(*controlling_piece);
		switch(info.input)
		{
			case DOWN:
				//potential_new_body->current_position.y = potential_new_body->current_position.y + 1;
				while (LiteGameEngine::collider_fit(controlling_piece.collider, 
					{ new_position.x, new_position.y + 1.0 }, this->field))
					//While it can still go down
				{
					new_position.y = new_position.y + 1.0;
	#ifdef _DEBUG
					if (new_position.y == HEIGHT * 3) //(It's okay dad, I know what I'm doing)
					{
						throw "Infinite loop from DOWN input.";
						break;
					}
	#endif
				}
				break;
			case LEFT:
				if (LiteGameEngine::collider_fit(controlling_piece.collider, 
					{new_position.x-1.0, new_position.y}, field))
				{
					new_position.x = new_position.x - 1.0;
				}
				break;
			case RIGHT:	
				if (LiteGameEngine::collider_fit(controlling_piece.collider, 
					{ new_position.x + 1.0, new_position.y }, field))
				{
					new_position.x = new_position.x + 1.0;
				}
				break;
			case ROTATE:
				LiteGameEngine::try_rotate(controlling_piece, field);
				break;
		}
		
		//do gravity
		//This is always less than 1.0
		double gravity_displacement = LiteGameModule::v_gravity_at(current_level) * seconds_per_update;
		new_position.y += gravity_displacement;
		if (!LiteGameEngine::collider_fit(controlling_piece.collider, new_position, field))
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
					score += (double)current_level / 100.0;
				}
			}
			score += (double)current_level / 700.0 + (((double)scaled_burned * current_level) / 125.0) ;

			//reassign controlling piece
			controlling_piece.reassign(coming_pieces.front());
			if (burned < 4 && !LiteGameEngine::collider_fit(controlling_piece.collider, controlling_piece.current_position, field))
			{
				//game over
				score = std::numeric_limits<double>::lowest();
				return false;
			}
			coming_pieces.pop();
			//generate the next one
			current_seed = TMath::GameRNG::xorshift64(&current_seed);
			coming_pieces.push(LiteGameEngine::rng_seed2bodytype(current_seed));
		}
		else
		{
			//no reassignment needed
			controlling_piece.current_position = new_position;
			//This will encourage ML to press DOWN
			score -= (double)current_level / 8000.0;
		}
		if (highest_score < score) highest_score = score;
	}
	return true;
}

LiteGameModule::LiteModule::LiteModule(const TMath::GameRNG::RNGSeed& initial_seed) :
	field(), current_seed(initial_seed),
	controlling_piece(LiteGameEngine::rng_seed2bodytype(initial_seed)),
	coming_pieces()
{
	for (unsigned char i = 0; i < 3; i++)
	{
		current_seed = TMath::GameRNG::xorshift64(&current_seed);
		coming_pieces.push(LiteGameEngine::rng_seed2bodytype(current_seed));
	}
}

LiteGameEngine::TetrisBody<10, 20> * LiteGameModule::LiteModule::create_new_body(const TMath::GameRNG::RNGSeed & seed)
{
	return new LiteGameEngine::TetrisBody<10, 20>(LiteGameEngine::rng_seed2bodytype(seed));
}


//LiteGameModule::LiteModule::LiteModule():
//	field(), current_seed(TMath::GameRNG::generate_random_seed())
//{
//
//}
