#include "zmath.h"

int Common::ZMath::round_nearest(double x)
{
	return floor(x + 0.5);
}

int Common :: ZMath::round_nearest(float x)
{
	float aftersum = x + 0.5f;
	return floor(aftersum);
}

