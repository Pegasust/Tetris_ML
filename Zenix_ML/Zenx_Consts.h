#pragma once
namespace TetrisML
{
#ifdef MULTI_THREAD
	const unsigned char THREADS =
#if defined (_RASPI) || defined (_RASPI64)
		1
#else
		2

#endif
	;
#endif

	const unsigned long long N_INDIVIDUALS_PER_GEN = 
#if defined (_DEBUG) || defined (RELEASE_N_OP)
		100
#else		
#if defined (_RASPI) || defined (_RASPI64)
		20
#else
		1000
#endif
#endif
		;
	const unsigned int EXPERIMENTS_PER_LIFE =
#if defined (_DEBUG) || defined (RELEASE_N_OP)
		3
#else
#if defined (_RASPI) || defined (_RASPI64)
		3
#else
		10
#endif
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