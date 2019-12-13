#include "rng.h"

std::mutex Common::ZMath::UInt64RNG::mtx;

Common::ZMath::UInt64RNG::UInt64RNG(const RNGSeed& seed)
{
	this->seed = seed;
}

Common::ZMath::UInt64RNG::RNGSeed Common::ZMath::UInt64RNG::get_value()
{
	RNGSeed r = this->seed;
	xorshift64(&this->seed);
	return r;
}

double Common::ZMath::UInt64RNG::get_double()
{
	double d = to_double(this->seed);
	xorshift64(&this->seed);
	return d;
}

bool Common::ZMath::UInt64RNG::get_bool(double& percentage)
{
	bool b = get_bool(this->seed, percentage);
	xorshift64(&this->seed);
	return b;
}

bool Common::ZMath::UInt64RNG::get_bool(RNGSeed& seed, double& percentage)
{
	//bool value;
	if (percentage >= 1.0) return true;//value = true;
	else
	{
		double d = Common::ZMath::UInt64RNG::to_double(seed);
		return //value =
			(1.0 - d) <= percentage;
	}
}

double Common::ZMath::get_random()
{
	static small_prng rng = small_prng(rand());
	return rng.get_double();
}
