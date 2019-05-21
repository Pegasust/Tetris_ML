#include "Zeny.h"
ZenoyAgent::UObservation Zenoy::Zeny::experiment(TMath::GameRNG::RNGUnion seed[1], void(*render_func)(LiteGameModule::LiteModule const&), void(*render_simulation_func)(LiteGameModule::LiteModule const&))
{

	auto _seed = (TMath::GameRNG::RNGSeed*)(seed);
	ZenoyAgent::TModule::LiteModule exp_mod(_seed);
	ZenoyAgent::UObservation uobsv;
	for (unsigned int i = 0; i < CoefficientVector::LENGTH; i++)
	{
		uobsv.values[i].double_expr = 0.0;
	}
	uint64_t moves_made = 0;
	ReplayInfo replay;
	replay.seed = seed[0].long_expr;
	bool game_over;
	do
	{
		DecisionFitness fittest_decision = std::numeric_limits<DecisionFitness>::lowest();
		ZenoyAgent::UObservation move_observation;
		char fittest_x = std::numeric_limits<char>::lowest();
		unsigned char fittest_y;
		ZenoyAgent::TEngine::Rotation fittest_rot = ZenoyAgent::TEngine::UP;

		for (ZenoyAgent::TEngine::Rotation rot = ZenoyAgent::TEngine::UP; rot <= ZenoyAgent::TEngine::LEFT; rot++)
		{
			ZenoyAgent::TEngine::TetrisCollider col;
			ZenoyAgent::TEngine::TetrisBody::rotate(col, rot, exp_mod.controlling_piece.type);
			for (char x = ZenoyAgent::TEngine::TetrisBody::MIN_POS_X; x <= ZenoyAgent::TEngine::TetrisBody::MAX_POS_X; x++)
			{
				//Check whether x, y, and rot is even ILLEGAL
				double _y = LiteAPI::down_cast(col, x, exp_mod.field);
				if (_y == -1.0)
				{
					//The piece is at an illegal x position
					continue;
				}
				unsigned char y = static_cast<unsigned char>(_y);

				//Check whether x, y, and rot can be achieved
				if(ZenoyAgent::)
			}
		}
	} while (!game_over);
	


	return uobsv;
}
Zenoy::DecisionFitness Zenoy::Zeny::get_decision_fitness(const ZenoyAgent::UObservation& obsv) const
{
	DecisionFitness f = obsv.values[0].double_expr * dna.values[0].double_expr;
	for (unsigned char i = 1; i < CoefficientVector::LENGTH; i++)
	{
		f += (obsv.values[i].double_expr * dna.values[i].double_expr);
	}
	return f;
}
