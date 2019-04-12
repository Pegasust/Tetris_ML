#include "core_timing.h"
#ifdef TIMEBASED
int CoreTiming::TimeModule::get_frames_updates()
{
	auto invoke_time = frame_clock::now();
	std::chrono::milliseconds span = std::chrono::duration_cast<std::chrono::milliseconds>(invoke_time - last_update);
	int value = Rounding::round_nearest((double)span.count() / millis_per_frame);
	if (value != 0)
	{
		last_update = frame_clock::now();
	}
}

void CoreTiming::TimeModule::reset_clock()
{
}
#endif
bool CoreTiming::TimeModule::try_update(int nFrames, void(*update_func))
{
	return false;
}
