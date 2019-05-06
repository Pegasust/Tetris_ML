#pragma once
#include <limits>
#include "core_timing.h"
#include "vector2d.h"
#include "rounding.h"
namespace GameFeatures
{
	typedef TMath::vector2<double> Position2D;
	typedef TMath::vector2<double> Velocity2D;
	struct GravityUnit
	{
		typedef unsigned long Level;
		const Level max_level = std::numeric_limits<Level>::max();
		const Level min_level = 1;
		const double v_gravity0 = (1.0/3.0); // goes 1/3 block per one second
		const double v_gravity_max = 60; //limit for 60 fps
		Level current_level = min_level;
		constexpr double v_gravity()
		{
			//Linear scaling
			auto m = (v_gravity_max - v_gravity0) / (max_level - min_level); //slope
			return m * (current_level - min_level) + v_gravity0; //y = m(x-1)+y1
		}
	};
}