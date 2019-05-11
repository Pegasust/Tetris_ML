#pragma once
#include "Zenx.h"
#include <unordered_set>
namespace TetrisML
{
	typedef TMath::GameRNG::RNGUnion Seed;
	typedef TMath::GameRNG SeedHelper;
	//H: Zenxis is a plural term of Zenx.
	class Zenxis
	{
	public:
		Seed seed[1];
		std::vector<Zenx> population;
		MetaInfo current_best;
		double current_best_score;
		unsigned long long generation;

		Zenxis(const Seed& initial_seed);
		//H: The big bang kickstarts the entire population to life for one generation.
		void big_bang();
	};
}