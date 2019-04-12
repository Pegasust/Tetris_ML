#pragma once
//TIMEBASED definition forces the AI to make decision more quickly. If we have more time, I might train the AI Program to acknowledge this.
#define TIMEBASED

#if defined(TIMEBASED) && defined(FRAMEBASED)
//More difficult variation is chosen
#undef TIMEBASED
#endif

#if !defined(TIMEBASED) && !defined(FRAMEBASED)
//Default it to be framebased
#define FRAMEBASED
#endif

#ifdef TIMEBASED
#include <chrono>
#endif
#include <math.h>
#include "rounding.h"
namespace CoreTiming
{
	//Initialize only at the start of the engine
	class TimeModule
	{
	public:
#ifdef TIMEBASED
		typedef std::chrono::steady_clock frame_clock;
		//FPS: 60
		const double millis_per_frame = 1 / 60;
		frame_clock::time_point last_update;
		TimeModule()
		{
			last_update = frame_clock::now();
		}
		int get_frames_updates();
		void reset_clock();
#else

#endif


		bool try_update(int nFrames, void(*update_func));
#ifdef TIMEBASED

#endif
	}
}
