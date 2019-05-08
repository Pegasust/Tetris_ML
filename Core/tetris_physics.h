#pragma once
#include <limits>
#include "core_timing.h"
#include "vector2d.h"
#include "rounding.h"
#include <vector>
namespace GameFeatures
{
	typedef TMath::vector2<double> Position2D;
	typedef TMath::vector2<double> Velocity2D;
	//Provides gravity for tetris game
	struct GravityUnit
	{
		typedef unsigned long Level;
		const Level max_level = std::numeric_limits<Level>::max();
		const Level min_level = 1;
		const double v_gravity0 = (1.0/3.0); // goes 1/3 block per one second
		const double v_gravity_max = 60; //limit for 60 fps
		Level current_level = min_level;
		double v_gravity_now = v_gravity0;
		//constexpr double v_gravity()
		//{
		//	//Linear scaling
		//	auto m = (v_gravity_max - v_gravity0) / (max_level - min_level); //slope
		//	return m * (current_level - min_level) + v_gravity0; //y = m(x-1)+y1
		//}
		//TODO: Assign this once every update and before Rigidbody's update
		void assign_v_gravity()
		{
			//Linear scaling
			auto m = (v_gravity_max - v_gravity0) / (max_level - min_level); //slope
			v_gravity_now = m * (current_level - min_level) + v_gravity0; //y = m(x-1)+y1
		}
	};
	struct Rigidbody
	{
		Position2D current_position;
		//Returns continue or lose (false)
		bool update(const GravityUnit& gravity, const Velocity2D& input, Position2D*(*col_detect)(const Position2D& p0, const Position2D& p1)
#ifdef TIMEBASED
			//TODO: Time-based module needs time input
#else
			, int n_frames_update = 1
#endif

		);
		Rigidbody(Position2D initial_position);
	};
}