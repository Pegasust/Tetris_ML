#include "game_rng.h"
TMath::GameRNG::RNGSeed TMath::GameRNG::generate_random_seed()
{
	//Get time now
	auto invoke_time = RNGClock::now();
	//Hardcoded because 0xDEADBEEF is a valid number : ^)
	RNGSeed x[1] = { 0xDEADBEEF };
	//Have it undergo some task to eliminate the upcoming RNGClock::now() the exact same
	x[0] = xorshift64(x);
	//Get the time span (this is pseudo-random due to the various clock speed)
	auto time_span = RNGClock::now() - invoke_time;
	//In nanosecond => It is actually pretty hard to recreate this randomness
	RNGSeed y = time_span.count();
	//Divide by two on each because the two values might be too big and makes it out of 
	//64 bit boundary.
	//Moreover, since there is a very small chance, but is likely to happen, that y = 0.
	//We don't want that to happen since the xorshift will produce only 0s if
	//we throw a 0 as the seed.
	return (x[0] / 2) + (y / 2);
}

bool TMath::GameRNG::get_bool(RNGSeed state[1], double percentage)
{
	bool val;
	if (percentage >= 1.0) val = true;
	else
	{
		val = (static_cast<double>(state[0]) / static_cast<double>(RNGSEED_MAX) > percentage);
	}
	state[0] = xorshift64(state);
	return val;
}