#pragma once
#include "Zenoy_agent.h"
#include "zmath.h"
namespace Zenoy
{
	
	typedef double Fitness; //Determines the fitness of this Zeny
	typedef double DecisionFitness; //Determines the fitness of its decision
	typedef CoefficientVector DNAConfig;

	struct ZenyInput
	{
		LiteGameModule::InputInfo game_input;
		DecisionFitness decision_fitness;
	};
	struct ReplayInfo
	{
		std::vector<ZenyInput> inputs;
		TMath::GameRNG::RNGSeed seed;
	};
	struct LifetimeRecord
	{
		double hscore;
		uint64_t hlevel;
		ReplayInfo best_replay;
	};
	class Zeny
	{
		const DNAConfig dna;
		LifetimeRecord record;
		double sum_hscore;
		uint64_t times_played;
		Fitness current_fitness;

		//Returns unscaled observation contains AVERAGE aggregate height, bulk, holes; TOTAL #burns; HIGHEST score
		ZenoyAgent::UObservation experiment(TMath::GameRNG::RNGUnion seed[1], 
			void(*render_func)(LiteGameModule::LiteModule const&), 
			void(*render_simulation_func)(LiteGameModule::LiteModule const&));
		DecisionFitness get_decision_fitness(const ZenoyAgent::UObservation& obsv) const;
	};

}