#include "game_clock.h"

// std::common_type_t<std::chrono::nanoseconds, std::chrono::nanoseconds>
// Common::GameClock::Instance::nano_time_diff(std::chrono::time_point<Clock> const& now) const

// std::common_type_t<std::chrono::nanoseconds, std::chrono::nanoseconds>
// Common::GameClock::Instance::nano_time_diff() const
//

Common::GameClock::Instance::Instance(bool initialize_time) {
    if (initialize_time) {
        then = Clock::now();
    } else {
        then = std::chrono::time_point<Clock>();
    }
}

void Common::GameClock::Instance::reset_then() {
    then = Clock::now();
}

std::chrono::time_point<Common::GameClock::Clock> Common::GameClock::Instance::reset() {
    auto retval = std::move(then);
    then = Clock::now();
    return retval;
}
