#include "framerate.h"

Verbosity::FrameTiming::FrameTiming() :
	initial_point(std::chrono::steady_clock::now()),
	total_frames(0),
	last_evoked(initial_point)
{

}

void Verbosity::FrameTiming::reset_counter()
{
	initial_point = std::chrono::steady_clock::now();
	last_evoked = initial_point;
	total_frames = 0;
}

double Verbosity::FrameTiming::get_current_frametime()
{
	auto evoked = std::chrono::steady_clock::now();
	double delta_time = static_cast<double>((evoked - last_evoked).count());
	nano2millis(delta_time);
	total_frames++;
	last_evoked = evoked;
	return delta_time;

}

double Verbosity::FrameTiming::get_average_frametime()
{
	auto evoked = std::chrono::steady_clock::now();
	double delta_time = static_cast<double>((evoked - initial_point).count())/(++total_frames);
	nano2millis(delta_time);
	last_evoked = evoked;
	return delta_time;
}

void Verbosity::FrameTiming::get_frametime(double& current, double& average)
{
	auto evoked = std::chrono::steady_clock::now();
	current = static_cast<double>((evoked - last_evoked).count());
	average = static_cast<double>((evoked - initial_point).count())/(++total_frames);
	nano2millis(current);
	nano2millis(average);
	last_evoked = evoked;
}
