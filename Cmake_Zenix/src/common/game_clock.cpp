#include "game_clock.h"

Common::GameClock::Instance::Instance() :
	then(Clock::now())
{
}

//std::common_type_t<std::chrono::nanoseconds, std::chrono::nanoseconds> Common::GameClock::Instance::nano_time_diff(std::chrono::time_point<Clock> const& now) const


//std::common_type_t<std::chrono::nanoseconds, std::chrono::nanoseconds> Common::GameClock::Instance::nano_time_diff() const
//

void Common::GameClock::Instance::reset_then()
{
	then = Clock::now();
}
