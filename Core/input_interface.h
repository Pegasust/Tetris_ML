#pragma once
#include "core_timing.h"
namespace InputInterface
{
	struct Input
	{
		double x, y;
#ifdef FRAMEBASED
		int n_frames_update;
#endif
	};
	struct InputInfo
	{

	};
}