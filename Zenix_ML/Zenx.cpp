#include "Zenx.h"

TetrisML::WeightedObservation::WeightedObservation(const DNAConfig& config, const RawObservation& raw)
{
	score = raw.score * config.score_scale;
	bulkiness = raw.bulkiness * config.bulkiness_scale;
	burn = raw.burn * config.burn_scale;
	aggregate_height = raw.aggregate_height * config.aggregate_height_scale;
	holes = raw.holes * config.holes_scale;
}
