#pragma once
#include <stdint.h>
#include <string>

#include "../common/game_clock.h"
#include "verbosity_core.h"


namespace Verbosity
{
	class FrameTiming
	{
		std::chrono::steady_clock::time_point initial_point;
		std::uint32_t total_frames;

		std::chrono::steady_clock::time_point last_evoked;
		constexpr void nano2millis(double& nano)
		{
			nano /= (1000.0 * 1000.0);
		}
	public:
		FrameTiming();
		void reset_counter();
		void update_frame_count()
		{
			total_frames++;
		}
		//updates total_frames, ms
		double get_current_frametime();
		//updates total_frames, ms
		double get_average_frametime();
		//updates total_frames, ms
		void get_frametime(double& current, double& average);
		constexpr static double framerate_from(double& frametime_ms)
		{
			return 1000.0 / frametime_ms;
		}
	public:
		FrameTiming(const FrameTiming&) = delete;
		void operator=(const FrameTiming&) = delete;
	};

	class FrameLogger
	{
	public:
		static FrameTiming& physics_tm()
		{
			static FrameTiming p_tm;
			return p_tm;
		}
		static FrameTiming& visual_tm()
		{
			static FrameTiming v_tm;
			return v_tm;
		}
	private:
		static std::string dying_msg()
		{
			double phys = physics_tm().get_average_frametime();
			double vis = visual_tm().get_average_frametime();
			double phys_c = FrameTiming::framerate_from(phys);
			double vis_c = FrameTiming::framerate_from(vis);
			std::string phys_str = std::to_string(phys_c) + "/" + std::to_string(phys) + "ms";
			std::string vis_str = std::to_string(vis_c) + "/" + std::to_string(vis) + "ms";
			std::string s = "avg_physics: "+phys_str;
			s.append("avg_visuals: " + vis_str);
			return s;
		}
	public:
		static VerbosityWriter<false>& get_perf_log()
		{
			static VerbosityWriter<false> perf_log = VerbosityWriter<false>(
				LOG_ROOT("performance"), &dying_msg);
			return perf_log;
		}
	};

}