#pragma once
#include "../common/zmath.h"

//Round nearest yields game physics error like clipping through the borders
//#define USE_ROUND_NEAREST //if not defined, the physics will use round down instead

#define V_FALL_0 3.0
#define V_FALL_MAX 100.8

////If not defined, tetriminos will try to rotate from the center
////And could care
//#define TETRIMINO_TRY_ROTATE_IN_PLACE
//
#ifdef TETRIMINO_TRY_ROTATE_IN_PLACE
#error NOT IMPLEMENTED!
#endif


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