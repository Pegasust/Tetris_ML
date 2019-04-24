#include "core_timing.h"
#ifdef TIMEBASED
int CoreTiming::TimeModule::get_frames_updates()
{
	auto invoke_time = frame_clock::now();
	std::chrono::milliseconds span = std::chrono::duration_cast<std::chrono::milliseconds>(invoke_time - last_update);
	int value = Rounding::round_nearest((double)span.count() / millis_per_frame);
	if (value != 0)
	{
		//reset timer to correct time
		last_update = frame_clock::now() + 
			(span - 
				//cast from double to millis
			std::chrono::duration<double, std::chrono::milliseconds>
				//calculate difference in time
				(static_cast<double>(value) * millis_per_frame));
	}
}

void CoreTiming::TimeModule::reset_clock()
{
	last_update = frame_clock::now();
}
#endif
bool CoreTiming::TimeModule::try_update(int nFrames, void(*update_func) ())
{
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
