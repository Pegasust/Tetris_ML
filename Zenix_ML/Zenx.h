#pragma once
#include "game_helper.h"
//Zenx is a genetic-based
namespace TetrisML
{
	const unsigned long long N_INDIVIDUALS_PER_GEN = 5;
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
	};
	class WeightedObservation: ZenixAgent::RawObservation
	{
	public:
		WeightedObservation(const DNAConfig& config , const RawObservation & raw);
	};
	struct MetaInfo
	{
		unsigned long long generation;
		double dna_config[N_INDIVIDUALS_PER_GEN];
	};
	class Zenx
	{
	public:

	};
}