#pragma once
#include "..//Zenix_ML/Zenxis.h"
//Rewritten for efficient multithreading
namespace ZenxisMath
{
	typedef std::chrono::steady_clock RNGClock;
	union RNG
	{
		uint64_t longexpr;
		double doubleexpr;
	};
	class RNGHelper
	{
	public:
		//Changes the value
		static RNG xorshift(RNG state[1]);
		static RNG get_rand();
		//[0.0, 1.0)
		static double double_co_cast(RNG state[1]);
		//(0.0, 1.0]
		static double double_oc_cast(RNG state[1]);
		static double double_oo_cast(RNG state[1]);
		static double oo_range(RNG state[1], const double& min, const double& max);
	};

}

namespace TetrisML
{
	DNAArray array_from_seed(ZenxisMath::RNG seed[1]);
	class Zenx_MT //Zenx with efficient multithreading
	{
	public:
		typedef double Fitness;
		DNAArray dna;
		Fitness universal_fitness; //0.0

		RecordInfo record;
		double sum_hscore;
		unsigned long long times_played;
		Fitness get_fitness(const ZenixAgent::RawObservation& obsv);

		ZenixAgent::RawObservation experiment(TMath::GameRNG::RNGUnion);
	};
}