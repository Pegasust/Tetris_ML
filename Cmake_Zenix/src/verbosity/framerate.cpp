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

Verbosity::FrameTiming& Verbosity::FrameLogger::physics_tm()
{
	static FrameTiming p_tm;
	return p_tm;
}

Verbosity::FrameTiming& Verbosity::FrameLogger::visual_tm()
{
	static FrameTiming p_tm;
	return p_tm;
}

std::string Verbosity::FrameLogger::dying_msg()
{
	double phys = physics_tm().get_average_frametime();
	double vis = visual_tm().get_average_frametime();
	double phys_c = FrameTiming::framerate_from(phys);
	double vis_c = FrameTiming::framerate_from(vis);
	std::string phys_str = std::to_string(phys_c) + " FPS (" + std::to_string(phys) + " ms)";
	std::string vis_str = std::to_string(vis_c) + " FPS (" + std::to_string(vis) + " ms)";
	std::string s = "avg_physics: " + phys_str;
	s.append("\navg_visuals: " + vis_str);
	return s;
}

Verbosity::VerbosityWriter<false>& Verbosity::FrameLogger::get_perf_log()
{
	static VerbosityWriter<false> perf_log = VerbosityWriter<false>(
		LOG_ROOT("performance"), &dying_msg);
	return perf_log;
}
