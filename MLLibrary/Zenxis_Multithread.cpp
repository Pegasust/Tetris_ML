#include "Zenxis_Multithread.h"

ZenxisMath::RNG ZenxisMath::RNGHelper::xorshift(RNG state[1])
{
	RNG x = state[0];
	x.longexpr ^= x.longexpr << 13;
	x.longexpr ^= x.longexpr >> 7;
	x.longexpr ^= x.longexpr << 17;
	return state[0] = x;
}

ZenxisMath::RNG ZenxisMath::RNGHelper::get_rand()
{
	RNG x[1];
	x[0].longexpr = { 0xDEADBEEF };
	long long tm_since_epoch = (RNGClock::now().time_since_epoch()).count();
	x[0].longexpr = (x[0].longexpr / 2) + (tm_since_epoch / 2);
	return x[0];
}

double ZenxisMath::RNGHelper::double_co_cast(RNG state[1])
{
	RNG x = { state[0].longexpr >> 11 };
	return x.longexpr != 0ULL? x.doubleexpr: 0.0;
}

double ZenxisMath::RNGHelper::double_oc_cast(RNG state[1])
{
	RNG x = { state[0].longexpr >> 11 };
	return x.longexpr != 0ULL ? x.doubleexpr : 1.0;
}

double ZenxisMath::RNGHelper::double_oo_cast(RNG state[1])
{
	RNG n[1] = { state[0] };
	do
	{
		xorshift(n);
	} while ((n[0].longexpr <= 0x00000000000007FFULL));
	return n[0].doubleexpr;
}

double ZenxisMath::RNGHelper::oo_range(RNG state[1],const double& min, const double& max)
{
	double range = max - min;
	return (min + (range * double_oo_cast(state)));
}

TetrisML::DNAArray TetrisML::array_from_seed(ZenxisMath::RNG seed[1])
{
	DNAArray arr;
	for (int i = 0; i < DNAArray::ARR_LENGTH; i++)
	{
		arr.arr[i] = ZenxisMath::RNGHelper::oo_range(seed, -1.0, 1.0);
		ZenxisMath::RNGHelper::xorshift(seed);
	}
	arr.dna.do_normalization();
	return arr;
}

TetrisML::Zenx_MT::Fitness TetrisML::Zenx_MT::get_fitness(const ZenixAgent::RawObservation& obsv)
{
	Fitness f = dna.dna.score_scale * obsv.score
		+ dna.dna.aggregate_height_scale * obsv.aggregate_height
		+ dna.dna.bulkiness_scale * obsv.bulkiness
		+ dna.dna.burn_scale * obsv.burn
		+ dna.dna.holes_scale * obsv.holes;
	return f;
}

ZenixAgent::RawObservation TetrisML::Zenx_MT::experiment(TMath::GameRNG::RNGUnion)
{
	return ZenixAgent::RawObservation();
}
