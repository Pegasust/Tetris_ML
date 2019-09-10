#pragma once
#include "../common/zmath.h"

//#define USE_ROUND_NEAREST //if not defined, the physics will use round down instead







namespace RoundingExt
{
	template<typename T>
	constexpr T position_round(double const& d)
	{
#ifdef USE_ROUND_NEAREST
		return (T)Common::ZMath::round_nearest(d);
#else
		return static_cast<T>(d);
#endif
	}
	template<typename T>
	constexpr T position_round(float const& f)
	{
#ifdef USE_ROUND_NEAREST
		return (T)Common::ZMath::round_nearest(f);
#else
		return static_cast<T>(f);
#endif
	}
}