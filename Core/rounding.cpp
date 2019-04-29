#include "rounding.h"

int TMath::round_nearest(double x)
{
	return floor(x + 0.5);
}

int TMath::round_nearest(float f)
{
	float aftersum = f + 0.5f;
	return floor(aftersum);
}
