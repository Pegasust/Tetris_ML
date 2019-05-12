#pragma once
#include "game_helper.h"
#include "Zenx_Consts.h"
//H: Zenx are to address the race of people who play Tetris their whole life.
//H: Zenxis pass their knowledge by reproducing
//H: Zenxis do not define gender; their reproduction require another Zenx.

//Available defs:
/*
#define SEED 0xDEADBEEF
*/
namespace TetrisML
{
	struct DNAConfig //contains all cooefficients
	{
		double
			score_scale, //How important the score is
			bulkiness_scale, //How important it is to keep the field bulky (sum of absolute difference of adjacent pieces) (-)
			burn_scale, //How important it is to burn (clear rows)
			aggregate_height_scale, //How important it is to keep the sum of height in each column (should be -)
			//Calculated: SUM of all column, each column is: (max_y - all_pieces[get_least_height()]);
			holes_scale//How important it is to make holes (should be -)
			;
		void do_normalization();
		double calculate_length();
		DNAConfig& operator+=(const DNAConfig& rhs);
		DNAConfig& operator /=(const double& rhs);
		DNAConfig& operator *=(const double& rhs);
	};
	union DNAArray
	{
		DNAConfig dna;
#define NOT_0 3
		double arr[NOT_0];
#undef NOT_0
	};
	struct MetaInfo
	{
		unsigned long long generation;
		DNAArray dna_config;
	};
	struct RecordInfo
	{
		unsigned long long level;
		double highest_score;
		void assign(ZenixAgent::TModule::LiteModule const& mod);
	};
	class Zenx
	{
	public:
		typedef double Fitness;
		typedef double Bias;
		DNAConfig dna;
		Fitness best_fitness = std::numeric_limits<Fitness>::lowest();
		//H: Zenxis remember their past life.
		RecordInfo lifetime_record;

		double sum_hscore;
		unsigned long long times_played;
		Fitness get_fitness(const ZenixAgent::RawObservation& obsv);

		//std::vector<TMath::GameRNG::RNGSeed> played_seeds;

		//Also assign best_fitness, lifetime_record
		ZenixAgent::RawObservation& play_once();
		//Also assign best_fitness, lifetime_record
		ZenixAgent::RawObservation& play_once(TMath::GameRNG::RNGUnion seed[1]);
		//Returns a more scientific and sensible observation
		ZenixAgent::RawObservation& experiment(TMath::GameRNG::RNGUnion seed[1], void (*render_func)(), void (*render_simulation_func)());
		//RETURNS UNNORMALIZED DNA
		static DNAConfig& reproduce(const Zenx& parent1, const Zenx& parent2);
		//RETURNS UNNORMALIZED DNA
		//If bi is negative, value is more biased towards one and vice versa
		static DNAConfig& reproduce(const Zenx& p1, const Zenx& p2, const Bias& bi);
		static bool try_mutate(DNAConfig& child_dna, TMath::GameRNG::RNGUnion x[1]);
		Zenx(const DNAArray& dna);

		bool operator >(Zenx const& rhs);
		Zenx();
	};
}