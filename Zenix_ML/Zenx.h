#pragma once
#include "game_helper.h"
//H: Zenx are to address the race of people who play Tetris their whole life.
//H: Zenxis pass their knowledge by reproducing
//H: Zenxis do not define gender; their reproduction require another Zenx.

//Available defs:
/*
#define SEED 0xDEADBEEF
*/
namespace TetrisML
{
	const unsigned long long N_INDIVIDUALS_PER_GEN = 100;
	constexpr double SURVIVING_RATIO = 0.1;
	constexpr double MUTATION_RATIO = 0.05;
	constexpr double MUTATE_RANGE = 0.2; //range is +- (MUTATE_RANGE / 2), inclusive




	const unsigned long long SURVIVORS_PER_GEN = N_INDIVIDUALS_PER_GEN * SURVIVING_RATIO;
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
	struct MetaInfo
	{
		unsigned long long generation;
		double dna_config[N_INDIVIDUALS_PER_GEN];
	};
	struct RecordInfo
	{
		unsigned long long level;
		double highest_score;
		TMath::GameRNG::RNGSeed last_seed;
		RecordInfo(ZenixAgent::TModule::LiteModule const& mod);
	};
	class Zenx
	{
	public:
		typedef double Fitness;
		typedef double Bias;
		DNAConfig dna;
		Fitness best_fitness = std::numeric_limits<Fitness>::lowest();
		//ZenixAgent::TModule::LiteModule latest_mod;

		//H: Zenxis remember their past life.
		RecordInfo lifetime_record;
		Fitness get_fitness(const ZenixAgent::RawObservation& obsv);

		//std::vector<TMath::GameRNG::RNGSeed> played_seeds;

		//Also assign best_fitness, lifetime_record
		ZenixAgent::RawObservation play_once();
		
		//RETURNS UNNORMALIZED DNA
		static DNAConfig reproduce(const Zenx& parent1, const Zenx& parent2);
		//RETURNS UNNORMALIZED DNA
		//If bi is negative, value is more biased towards one and vice versa
		static DNAConfig reproduce(const Zenx& p1, const Zenx& p2, const Bias& bi);
		static constexpr bool try_mutate(DNAConfig& child_dna);

	};
}