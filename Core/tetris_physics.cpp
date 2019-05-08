#include "tetris_physics.h"

bool GameFeatures::Rigidbody::update(const GravityUnit& gravity, const Velocity2D& input, Position2D* (*col_detect)(const Position2D& p0, const Position2D& p1)
	, int n_frames_update)
{
#ifdef FRAMEBASED
	const double& time = CoreTiming::TimeModule::millis_per_frame * (double)n_frames_update;
#endif
	Velocity2D this_v;
	if (input.y == -1)
	{
		this_v.y = -99999999.9;
	}
	else
	{
		this_v.y = -gravity.v_gravity_now;
	}
	this_v.x = input.x;
	//Collision detection
	Position2D p0 = current_position;
	Position2D p1 = current_position + this_v * time;
	Position2D* new_p = col_detect(p0, p1);
	if (new_p == &p0 || TMath::round_nearest(new_p->magnitude() * 10.0)/10.0 < 1.0) //It doesn't change that much
	{
		return false; //It's game over
	}
	else
	{
		this->current_position = *new_p;
		return true;
	}
}

GameFeatures::Rigidbody::Rigidbody(Position2D initial_position) :current_position(initial_position)
{

}
