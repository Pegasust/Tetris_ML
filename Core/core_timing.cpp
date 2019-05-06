#include "core_timing.h"
#ifdef TIMEBASED
int CoreTiming::TimeModule::get_frames_updates()
{
	auto invoke_time = frame_clock::now();
	std::chrono::milliseconds span = std::chrono::duration_cast<std::chrono::milliseconds>(invoke_time - last_update);
	int value = TMath::round_nearest((double)span.count() / millis_per_frame);
	if (value != 0)
	{
		//reset timer to correct time
		last_update = (frame_clock::now() + 
			(span - 
				//cast from double to millis
			std::chrono::milliseconds(TMath::round_nearest(static_cast<double>(value) * millis_per_frame))));
	}
	return value;
}

void CoreTiming::TimeModule::reset_clock()
{
	last_update = frame_clock::now();
}
#endif
bool CoreTiming::TimeModule::try_update(void(*update_func) ()
#ifdef FRAMEBASED
	, int nFrames
#endif
)
{
#if TIMEBASED
	int nFrames = get_frames_update();
#endif
	if (nFrames > 0)
	{
		for (int i = 0; i < nFrames; i++)
		{
			(*update_func)();
		}
		return true;
	}
	return false;
}

bool CoreTiming::TimeModule::try_update(
#ifdef FRAMEBASED
	int nFrames
#endif
)
{
#if TIMEBASED
	nFrames = get_frames_update();
#endif
	if (nFrames > 0)
	{
		for (int i = 0; i < nFrames; i++)
		{
			this->game_update_func();
		}
		return true;
	}
	return false;
}

CoreTiming::TimeModule::TimeModule(const void(*update_func)()):game_update_func(update_func)
{

}
CoreTiming::TimeModule::TimeModule() : game_update_func(0)
{

}
