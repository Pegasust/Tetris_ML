#pragma once
#include <string>
#include <stdint.h>

#include "../common/game_clock.h"
#include "verbosity_core.h"

namespace Verbosity {
class FrameTiming {
    std::chrono::steady_clock::time_point initial_point;
    std::uint32_t total_frames;
    bool frozen;
    std::chrono::steady_clock::time_point frozen_time;
    std::chrono::steady_clock::time_point last_evoked;
    constexpr void nano2millis(double& nano) const{
        nano /= (1000.0 * 1000.0);
    }
	inline std::chrono::steady_clock::time_point get_end_point() const {
        return frozen ? frozen_time : std::chrono::steady_clock::now();
	}

public:
    FrameTiming();
    void reset_counter();
    /// Freezes the counter. Currently, there is no way to resume the counter.
    void freeze_counter();
    void update_frame_count() {
        total_frames++;
    }
    // updates total_frames, ms (not working properly at all).
    double get_current_frametime();
    // updates total_frames, ms
    double get_average_frametime();
    // updates total_frames, ms
    void get_frametime(double& current, double& average);
    constexpr static double framerate_from(double& frametime_ms) {
        return 1000.0 / frametime_ms;
    }

public:
    FrameTiming(const FrameTiming&) = delete;
    void operator=(const FrameTiming&) = delete;
    static std::string to_string(FrameTiming& f);
    std::string to_string();
};

class FrameLogger {
public:
    // Physics update
    static FrameTiming& physics_tm();
    // Draw to visual buffer
    static FrameTiming& visual_tm();
    // Display to screen
    static FrameTiming& draw_tm();

private:
    static std::string dying_msg();

public:
    static VerbosityWriter<false>& get_perf_log();
};

} // namespace Verbosity