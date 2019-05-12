#pragma once
#include "Zenx.h"
#include <unordered_set>
#include <fstream>
#include <sstream>
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
		//H: After the big bang, many Zenxis' lives are now extended to a longer period.
		//One life can play more Tetris games now, and they store their knowledge by writing.
		void experiment(unsigned int const& n_exp, std::ofstream & store_stream);
		struct ZenxMetaInfo
		{
			
			ZenixAgent::RawObservation obsv;
			double level;
			double high_score;
		};
		//std::string serialize_string(const DNAArray& dna, ZenxMetaInfo const& best_zenx_info, ZenxMetaInfo const& gen_avg_info);
		void push_data(std::ostream & fstream,
			const DNAArray& dna, ZenxMetaInfo const& best_zenx_info, 
			ZenxMetaInfo const& gen_avg_info);
		void push_data(std::ostream& fstream,
			ZenxMetaInfo const& best_zenx_info,
			ZenxMetaInfo const& gen_avg_info);
		static void render(const LiteGameModule::LiteModule& mod);
		//static void render();
	};
}