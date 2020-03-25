#include "framerate.h"

Verbosity::FrameTiming::FrameTiming()
    : initial_point(std::chrono::steady_clock::now()), total_frames(0), last_evoked(initial_point),
      frozen(false), frozen_time() {}

void Verbosity::FrameTiming::reset_counter() {
    initial_point = std::chrono::steady_clock::now();
    last_evoked = initial_point;
    total_frames = 0;
}

double Verbosity::FrameTiming::get_current_frametime() {
    auto evoked = get_end_point();
    double delta_time = static_cast<double>((evoked - last_evoked).count());
    nano2millis(delta_time);
    total_frames++;
    last_evoked = evoked;
    return delta_time;
}

double Verbosity::FrameTiming::get_average_frametime() {
    auto evoked = get_end_point();
    double delta_time = static_cast<double>((evoked - initial_point).count()) / (++total_frames);
    nano2millis(delta_time);
    last_evoked = evoked;
    return delta_time;
}

void Verbosity::FrameTiming::get_frametime(double& current, double& average) {
    auto evoked = get_end_point();
    current = static_cast<double>((evoked - last_evoked).count());
    average = static_cast<double>((evoked - initial_point).count()) / (++total_frames);
    nano2millis(current);
    nano2millis(average);
    last_evoked = evoked;
}

void Verbosity::FrameTiming::freeze_counter() {
    frozen_time = std::chrono::steady_clock::now();
    frozen = true;
}

Verbosity::FrameTiming& Verbosity::FrameLogger::physics_tm() {
    static FrameTiming p_tm;
    return p_tm;
}

Verbosity::FrameTiming& Verbosity::FrameLogger::visual_tm() {
    static FrameTiming p_tm;
    return p_tm;
}

Verbosity::FrameTiming& Verbosity::FrameLogger::draw_tm() {
    static FrameTiming p_tm;
    return p_tm;
}

std::string Verbosity::FrameTiming::to_string(FrameTiming& f) {
    double avg_ms = f.get_average_frametime();
    double fps = FrameTiming::framerate_from(avg_ms);
    return std::to_string(fps) + " FPS (" + std::to_string(avg_ms) + "ms)";
}

std::string Verbosity::FrameTiming::to_string() {
    return Verbosity::FrameTiming::to_string(*this);
}

std::string Verbosity::FrameLogger::dying_msg() {
    physics_tm().freeze_counter();
    visual_tm().freeze_counter();
    draw_tm().freeze_counter();
    std::string s = "avg physics: " + physics_tm().to_string();
    s += "\navg visual: " + visual_tm().to_string();
    s += "\navg draw: " + draw_tm().to_string();
    return s;
}

Verbosity::VerbosityWriter<false>& Verbosity::FrameLogger::get_perf_log() {
    static VerbosityWriter<false> perf_log =
        VerbosityWriter<false>(LOG_ROOT("performance"), &dying_msg);
    return perf_log;
}
