#include "rounding.h"

int Rounding::round_nearest(double x)
{
	return floor(x + 0.5);
}

int Rounding::round_nearest(float f)
{
	float aftersum = f + 0.5f;
	return floor(aftersum);
}
