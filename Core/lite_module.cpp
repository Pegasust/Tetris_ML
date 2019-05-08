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
			new_position.y = floor(new_position.y - gravity_displacement);
		}
		controlling_piece.current_position = new_position;
	}
	
}

LiteGameModule::LiteModule::LiteModule(const TMath::GameRNG::RNGSeed& initial_seed) :
	field(), current_seed(initial_seed),
	controlling_piece(*create_new_body(current_seed)),
	coming_pieces()
{
	for (unsigned char i = 0; i < 3; i++)
	{
		current_seed = TMath::GameRNG::xorshift64(&current_seed);
		coming_pieces.push_back(create_new_body(current_seed));
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
