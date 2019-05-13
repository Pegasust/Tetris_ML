#include "game_rng.h"
TMath::GameRNG::RNGSeed TMath::GameRNG::generate_random_seed()
{
	//Get time now
	auto invoke_time = RNGClock::now();
	//Hardcoded because 0xDEADBEEF is a valid number : ^)
	RNGSeed x[1] = { 0xDEADBEEF };
	//Have it undergo some task to eliminate the upcoming RNGClock::now() the exact same
	for (int i = 0; i < 2; i++)
	{
		xorshift64(x);
	}
	//Get the time span (this is pseudo-random due to the various clock speed)
	auto time_span = RNGClock::now() - invoke_time;
	//In nanosecond => It is actually pretty hard to recreate this randomness
	RNGSeed y = time_span.count();
#ifdef _DEBUG
	std::cout << "y = " << y << std::endl;
#endif
	//Divide by two on each because the two values might be too big and makes it out of 
	//64 bit boundary.
	//Moreover, since there is a very small chance, but is likely to happen, that y = 0.
	//We don't want that to happen since the xorshift will produce only 0s if
	//we throw a 0 as the seed.
	x[0] = (x[0] / 2) + (y / 2);
	//Just randomize :)
	xorshift64(x);
	return x[0];
}

bool TMath::GameRNG::get_bool(RNGSeed state[1], double percentage)
{
	bool val;
	if (percentage >= 1.0) val = true;
	else
	{
		//val = (static_cast<double>(state[0]) / static_cast<double>(RNGSEED_MAX) > percentage);
		val = (state[0]-1) < ((RNGSEED_MAX-1) * percentage);
	}
	xorshift64(state);
	return val;
}

bool TMath::GameRNG::get_bool(RNGUnion state[1], double percentage)
{
	bool val;
	if (percentage >= 1.0) val = true;
	else
	{
		//val = (static_cast<double>(state[0]) / static_cast<double>(RNGSEED_MAX) > percentage);
		val = (state[0].long_expr - 1) < ((RNGSEED_MAX - 1) * percentage);
	}
	xorshift64(state);
	return val;
}

double TMath::GameRNG::get_value(RNGUnion state[1], double const& min, double const& max)
{
//#define IS_POSITIVE(signbit) (signbit == 0)
//	if (isnan(state[0].double_expr))
//	{
//		if (IS_POSITIVE(std::signbit(state[0].double_expr)))
//		{
//			return max;
//		}
//		else
//		{
//			return min;
//		}
//	}
//	double range = max - min;
//	return min + ((state[0].double_expr / std::numeric_limits<double>::max()) * range);
//#undef IS_POSITIVE
	double range = max - min;
	const unsigned long long stepper = 100000000000;
	double stepper_inc = range / stepper;
	uint64_t step_mult = state[0].long_expr % stepper;
	double increment = step_mult * stepper_inc; //unsigned
	xorshift64(state);
	return min + increment;
}

//std::mutex TMath::mtx = std::mutex();