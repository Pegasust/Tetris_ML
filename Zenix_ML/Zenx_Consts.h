#pragma once
namespace TetrisML
{
	const unsigned long long N_INDIVIDUALS_PER_GEN = 
#ifdef _DEBUG
		10
#else
		1000
#endif
		;
	constexpr double SURVIVING_RATIO = 0.2;
	constexpr double MUTATION_RATIO = 0.05;
	constexpr double MUTATE_RANGE = 0.4; //interval is +- (MUTATE_RANGE / 2), inclusive



	constexpr double MUTATE_MAX = MUTATE_RANGE / 2.0;
	constexpr double MUTATE_MIN = -(MUTATE_RANGE / 2.0);
	const unsigned long long SURVIVORS_PER_GEN = N_INDIVIDUALS_PER_GEN * SURVIVING_RATIO;
	const unsigned long long N_COMPLETE_RANDOM = N_INDIVIDUALS_PER_GEN - (SURVIVORS_PER_GEN * 1.5); //*1.5 because each survivor could only reproduce once (?) per generation
}