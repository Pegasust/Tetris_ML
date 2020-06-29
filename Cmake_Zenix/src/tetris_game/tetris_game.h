#pragma once

#include <chrono>
#include <thread>
#include <unordered_map>
#include <stdint.h>
#include "../common/file_managing.hpp"
#include "../common/game_clock.h"
#include "../common/keyboard_input.h"
#include "../common/rng.h"
#include "../common/zmath.h"
#include "../common/common.hpp"
#include "../tetris/game_module.h"
#include "../verbosity/framerate.h"
#include "../video_core/video_core.h"
#include "../tetris_api/tetris_extended_engine.h"
// #define KEYSTROKE_UNORDERED_MAP  // Disabled because of low performance.
namespace TetrisGame {
template <bool threaded, uint64_t target_framerate, Common::ZMath::UInt64RNG::RNGSeed rng_seed>
class Tetris {
    static constexpr ::Tetris::Input get_input(const int& keypress) {
#ifdef KEYSTROKE_UNORDERED_MAP
        static std::unordered_map<int, ::Tetris::Input> key2inp = {
            {-1, ::Tetris::Input::NONE},
            {80, ::Tetris::Input::DOWN},          // Down arrow
            {72, ::Tetris::Input::UP},            // Up arrow
            {75, ::Tetris::Input::LEFT},          // Left arrow
            {77, ::Tetris::Input::RIGHT},         // Right arrow
            {32, ::Tetris::Input::CAST_DOWN},     // space
            {122, ::Tetris::Input::ROTATE_BACK},  // z button
            {120, ::Tetris::Input::ROTATE_FORTH}, // x button
            {62, ::Tetris::Input::PROGRAM_EXIT}   // F4 button
        };
        if (key2inp.find(keypress) == key2inp.end()) {
            // Cache this value.
            // key2inp[keypress]= ::Tetris::Input::NONE;
            return ::Tetris::Input::NONE;
        }
        return key2inp.at(keypress);
#else
        switch (keypress) {
        case -1:
            return ::Tetris::Input::NONE;
        case 80:
            return ::Tetris::Input::DOWN;
        case 72:
            return ::Tetris::Input::UP;
        case 75:
            return ::Tetris::Input::LEFT;
        case 77:
            return ::Tetris::Input::RIGHT;
        case 32:
            return ::Tetris::Input::CAST_DOWN;
        case 122:
            return ::Tetris::Input::ROTATE_BACK;
        case 120:
            return ::Tetris::Input::ROTATE_FORTH;
        case 62:
            return ::Tetris::Input::PROGRAM_EXIT;
        }
#endif
    }
    static constexpr uint64_t frametime_micros = (1000 * 1000 / target_framerate);
    using KeyboardModule =
        Common::KeyboardListener<true, frametime_micros / 3 * 2, std::chrono::microseconds>;
    void start_game();

public:
    Common::ZMath::UInt64RNG::RNGSeed rng;
    bool exit;
    VideoCore::VideoHandler videocore;
    // Common::GameClock::Instance game_clock;
    Tetris();
    void start();

private:
    using GameEngine = TetrisAPI::TetrisExtendedEngine;
    static void threaded_update(bool& str_upd_done, const ::Tetris::GameModule& game,
                                Renderer::MainRenderer::RenderData& rd);
};
} // namespace TetrisGame

#include "tetris_game.ipp"