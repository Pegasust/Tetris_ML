#include "Zenx.h"
#ifdef RENDER
#include <conio.h>
#endif

void TetrisML::DNAConfig::do_normalization()
{
	double length = calculate_length();
	score_scale /= length;
	bulkiness_scale /= length;
	burn_scale /= length;
	aggregate_height_scale /= length;
	holes_scale /= length;
}
double TetrisML::DNAConfig::calculate_length()
{
#define SQR(x) (x*x)
	double length = sqrt((SQR(score_scale) + SQR(bulkiness_scale) + SQR(burn_scale) + SQR(aggregate_height_scale) + SQR(holes_scale)));
#undef SQR
	return length;
}

TetrisML::DNAConfig& TetrisML::DNAConfig::operator+=(const DNAConfig& rhs)
{
	this->score_scale += rhs.score_scale;
	this->bulkiness_scale += rhs.bulkiness_scale;
	this->burn_scale += rhs.burn_scale;
	this->aggregate_height_scale += rhs.aggregate_height_scale;
	this->holes_scale += rhs.holes_scale;
	return *this;
}

TetrisML::DNAConfig& TetrisML::DNAConfig::operator/=(const double& rhs)
{
	this->score_scale /= rhs;
	this->bulkiness_scale /= rhs;
	this->burn_scale /= rhs;
	this->aggregate_height_scale /= rhs;
	this->holes_scale /= rhs;
	return *this;
}

TetrisML::DNAConfig& TetrisML::DNAConfig::operator*=(const double& rhs)
{
	this->score_scale *= rhs;
	this->bulkiness_scale *= rhs;
	this->burn_scale *= rhs;
	this->aggregate_height_scale *= rhs;
	this->holes_scale *= rhs;
	return *this;
}


TetrisML::Zenx::Fitness TetrisML::Zenx::get_fitness(const ZenixAgent::RawObservation& obsv)
{
//#define SCALE(configuration) (dna.##configuration##_scale * obsv.##configuration)
//	return SCALE(score) + SCALE(bulkiness) + SCALE(burn) + SCALE(aggregate_height) + SCALE(holes);
//#undef SCALE
	Fitness f = dna.score_scale * obsv.score
		+ dna.aggregate_height_scale * obsv.aggregate_height
		+ dna.bulkiness_scale * obsv.bulkiness
		+ dna.burn_scale * obsv.burn
		+ dna.holes_scale * obsv.holes;
	return f;
}

ZenixAgent::RawObservation& TetrisML::Zenx::play_once()
{
	TMath::GameRNG::RNGSeed seed[] = {
#ifdef SEED
		SEED
#else
		TMath::GameRNG::generate_random_seed()
#endif
	};
	ZenixAgent::TModule::LiteModule main_mod = ZenixAgent::TModule::LiteModule(seed);
	//ZenixAgent::TModule::InputInfo input;
	ZenixAgent::RawObservation best_lifetime_obsv;
	
	char fittest_x;
	unsigned char fittest_y;
	ZenixAgent::TEngine::Rotation fittest_rot;
	do
	{
		//Within scope: In every situation
		Fitness local_best_fitness = std::numeric_limits<Fitness>::lowest();
		//For every choice
		for (LGEngine::Rotation rot = LGEngine::UP; rot <= LGEngine::LEFT; rot++) //Try every rotation
		{
			for (char x = LGEngine::TetrisBody::MIN_POS_X; x <= LGEngine::TetrisBody::MAX_POS_X; x++) //Try every x
			{
				//TODO: Optimize this: remove downcast from game_helper::can_move_piece
				//Do downcast here instead?
				for (unsigned char y = LGEngine::TetrisBody::MAX_POS_Y; y <= LGEngine::TetrisBody::MIN_POS_Y; y--) //Try y
					//From bot to top.
				{
					// TODO: Optimize an output for can_move_piece can improve performance
					//Try to make the move to find out if it's the best choice
					if (ZenixAgent::can_move_piece(main_mod, x, y, rot))
					{
						auto cloned_mod = main_mod;
						unsigned char burned;
						//Make the actual move on the cloned mod
						if (ZenixAgent::attempt_move_piece(cloned_mod, x, y, rot, burned))
						{
							//gather info
							auto obsv = ZenixAgent::get_raw_observation(cloned_mod.field);
							//Since this is not game over, data are surely not altered.
							obsv.score = cloned_mod.score;
							obsv.burn = burned;
							//Calculate this fitness
							Fitness situation_fitness = get_fitness(obsv);
							//If it is not the best fitness in the situation
							if (situation_fitness > local_best_fitness)
							{
								local_best_fitness = situation_fitness;
								fittest_x = x;
								fittest_y = y;
								fittest_rot = rot;
								if (situation_fitness > best_fitness)
								{
									best_fitness = situation_fitness;
									best_lifetime_obsv = obsv;
								}
							}
						}
						else
						{
#ifdef _DEBUG
							throw "Zenx::get_fitness::play_once() throwing error at attempting to move piece at location. This should not be called unless attempt_move_piece() or can_move_piece() is defected.";
#endif
						}
						//H: Don't try to go higher. Save your time Zenxis.
						break;
					}
				}
			}
		}


		//No possible move
		//Well, it's equal to loss anyways
		break;
	} while (ZenixAgent::apply_moveset(main_mod, fittest_x, fittest_x, fittest_rot));
	lifetime_record.assign(main_mod);

	return best_lifetime_obsv;
}

TetrisML::DNAConfig& TetrisML::Zenx::reproduce(const Zenx& parent1, const Zenx& parent2)
{
	//Scale each parent's DNA by their best fitness
	DNAConfig scaled_1 = parent1.dna;
	scaled_1 *= parent1.best_fitness;
	DNAConfig scaled_2 = parent2.dna;
	scaled_2 *= parent2.best_fitness;
	scaled_1 += scaled_2;
	//This is not normalized. Be sure to normalize dna before testing
	return scaled_1;
}

TetrisML::DNAConfig& TetrisML::Zenx::reproduce(const Zenx& parent1, const Zenx& parent2, const Bias& bi)
{
#if _DEBUG
	if (bi > 1.0 || bi < -1.0) throw "Bias should be in the range [-1.0, 1.0] (TetrisML::Zenx::reproduce)";
#endif
	//Scale each parent's DNA by their best fitness
	DNAConfig scaled_1 = parent1.dna;
	scaled_1 *= parent1.best_fitness;
	scaled_1 *= (1 - bi) / 2; //bi = 1, mult = 0, bi = 0, mult = 1/2, bi = -1, mult = 1
	DNAConfig scaled_2 = parent2.dna;
	scaled_2 *= parent2.best_fitness;
	scaled_2 *= (1 + bi) / 2; //bi = 1, mult = 1, bi = 0, mult = 1/2, bi = -1, mult = 0
	scaled_1 += scaled_2;
	//This is not normalized. Be sure to normalize dna before testing
	return scaled_1;
}

bool TetrisML::Zenx::try_mutate(DNAConfig& child_dna, TMath::GameRNG::RNGUnion x[1])
{
	x[0].long_expr = TMath::GameRNG::generate_random_seed();
	if (TMath::GameRNG::get_bool(x, MUTATION_RATIO))
	{
		//when this child is about to be mutated
		DNAArray* cast = (DNAArray*)& child_dna;
		for (unsigned char i = 0; i < (sizeof(DNAArray) / sizeof(double)); i++)
			//Iterate through all coefficients
		{
			//inline rng let's go
			//In interval [1, 5]
			char n_coefficients_change = (x[0].long_expr % (sizeof(DNAArray) / sizeof(double)));
			for (; n_coefficients_change >= 0; n_coefficients_change--)
			{
				TMath::GameRNG::xorshift64(x);
				cast->arr[n_coefficients_change] += TMath::GameRNG::get_value(x, MUTATE_MIN, MUTATE_MAX);
			}			
		}
		return true;
	}
	return false;
}

TetrisML::Zenx::Zenx(const DNAArray& dna):
	dna(dna.dna), lifetime_record({ 0, 0.0 })
{
}

bool TetrisML::Zenx::operator>(Zenx const& rhs)
{
	return this->lifetime_record.highest_score > rhs.lifetime_record.highest_score;
}

TetrisML::Zenx::Zenx():
	dna({ 0.0, 0.0, 0.0, 0.0, 0.0 }), lifetime_record({ 0, 0.0 })
{
}

void TetrisML::RecordInfo::assign(ZenixAgent::TModule::LiteModule const& mod)
{
	level = mod.current_level;
	highest_score = mod.highest_score;
}


ZenixAgent::RawObservation& TetrisML::Zenx::play_once(TMath::GameRNG::RNGUnion seed[1])
{
	ZenixAgent::TModule::LiteModule main_mod = ZenixAgent::TModule::LiteModule((TMath::GameRNG::RNGSeed*)&seed);
	//ZenixAgent::TModule::InputInfo input;
	ZenixAgent::RawObservation best_lifetime_obsv;

#ifdef RENDER
	Renderer::RenderUnit rdr(main_mod);
#endif
	char fittest_x;
	unsigned char fittest_y;
	ZenixAgent::TEngine::Rotation fittest_rot;
	do
	{
		fittest_x = std::numeric_limits<char>::lowest();
#ifdef RENDER
		Renderer::clear_console();
		rdr.update_string(main_mod);
		rdr.render();
		std::cout << "MOVE_SET_APPLIED." << std::endl;
#ifdef _TRACE
		_getch();
		std::cout << "Press smt..." << std::endl;

#endif
#endif
		//Within scope: In every situation
		Fitness local_best_fitness = std::numeric_limits<Fitness>::lowest();
		//For every choice
		for (LGEngine::Rotation rot = LGEngine::UP; rot <= LGEngine::LEFT; rot++) //Try every rotation
		{
			for (char x = LGEngine::TetrisBody::MIN_POS_X; x <= LGEngine::TetrisBody::MAX_POS_X; x++) //Try every x
			{
				//TODO: Optimize this: remove downcast from game_helper::can_move_piece
				//Do downcast here instead?
				for (unsigned char y = LGEngine::TetrisBody::MAX_POS_Y; y < 255 && y >= LGEngine::TetrisBody::MIN_POS_Y; y--) //Try y
					//From bot to top.
				{
					//Check if collider can even be there to prevent going through a hassle.
					ZenixAgent::TEngine::TetrisCollider temp;
					ZenixAgent::TEngine::TetrisBody::rotate(temp, rot, main_mod.controlling_piece.type);
					ZenixAgent::TEngine::Position2D position_check = { (double)x, (double)y };

					if (!main_mod.field.check_collider(temp, position_check))
					{
						continue;
					}
					// TODO: Optimize an output for can_move_piece can improve performance
					//Try to make the move to find out if it's the best choice
					if (ZenixAgent::can_move_piece(main_mod, x, y, rot))
					{
						auto cloned_mod = main_mod;
						unsigned char burned;
						//Make the actual move on the cloned mod
						if (ZenixAgent::attempt_move_piece(cloned_mod, x, y, rot, burned))
						{
#ifdef RENDER
							Renderer::RenderUnit rdr(cloned_mod);
							Renderer::clear_console();
							rdr.render();
							std::cout << "Attempt: <" << (int)x << ", " << (int)y << ", " << (int) rot << ">.";
#endif
							//gather info
							auto obsv = ZenixAgent::get_raw_observation(cloned_mod.field);
							//Since this is not game over, data are surely not altered.
							obsv.score = cloned_mod.score;
							obsv.burn = burned;
							//Calculate this fitness
							Fitness situation_fitness = get_fitness(obsv);
							//If it is not the best fitness in the situation
							if (situation_fitness > local_best_fitness)
							{
								local_best_fitness = situation_fitness;
								fittest_x = x;
								fittest_y = y;
								fittest_rot = rot;
								if (situation_fitness > best_fitness)
								{
									best_fitness = situation_fitness;
									best_lifetime_obsv = obsv;
								}
							}
						}
						else
						{
#ifdef _DEBUG
							throw "Zenx::get_fitness::play_once() throwing error at attempting to move piece at location. This should not be called unless attempt_move_piece() or can_move_piece() is defected.";
#endif
						}
						//H: Don't try to go higher. Save your time Zenxis.
						break;
					}
				}
				
			}
		}


		//No possible move
		if (fittest_x == std::numeric_limits<char>::lowest())
		{
#ifdef RENDER
			std::cout << "No possible move." << std::endl; 
#ifdef _TRACE
				_getch();
				std::cout << "Press smt..." << std::endl;
#endif
			//throw "WTF?";
			break;
#endif
			//Well, it's equal to loss anyways
		}
	} while (ZenixAgent::apply_moveset(main_mod, fittest_x, fittest_x, fittest_rot));
#ifdef RENDER
	Renderer::clear_console();
	std::cout << "Game over." << std::endl;
#ifdef _TRACE
	_getch();
	std::cout << "Press smt..." << std::endl;

#endif
#endif
	lifetime_record.assign(main_mod);

	return best_lifetime_obsv;
}