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
		static FrameTiming& physics_tm();
		static FrameTiming& visual_tm();
	private:
		static std::string dying_msg();
	public:
		static VerbosityWriter<false>& get_perf_log();
	};

}