#pragma once

template <bool threaded, uint64_t target_framerate, Common::ZMath::UInt64RNG::RNGSeed rng_seed>
void TetrisGame::Tetris<threaded, target_framerate, rng_seed>::start_game() {

    INITIALIZE_VERBOSITY_LOGGER;
    const uint64_t ms_frame_dur = static_cast<uint64_t>(1000) / (target_framerate);
    const auto next_frame_dur = std::chrono::duration<uint64_t, std::milli>(ms_frame_dur);
    VERBOSITY_LOG("ms_frame_dur = " + std::to_string(ms_frame_dur));
    // VERBOSITY_LOG("Successfully create a new log");
    bool renderer_initialized = false;
    do {
        ::Tetris::GameModule game(this->rng);
        game_clock = Common::GameClock::Instance();
        // This thread draw renderer buffer to end-user client.
        videocore.start_async_display(&videocore, game);
        std::thread r_upd_th; // Render update thread
        bool physics_updated = true;
        if (!renderer_initialized) {
            Renderer::MainRenderer::try_initialize(game, videocore.display_data);
            renderer_initialized = true;
        }
        // Get data from game data to draw into renderer buffer (visual update)
        if (threaded) {
            r_upd_th = std::thread([&, this]() {
                static const auto sleep_duration =
                    std::chrono::duration<int, std::milli>(std::max(1,FRAMETIME_MILLIS));
                while (!game.lost) {
                    if (physics_updated) {
                        Renderer::MainRenderer::try_update(game, videocore.display_data);
                        physics_updated = false;
                        videocore.keep_displaying_data = true;
                    }
                    Verbosity::FrameLogger::visual_tm().update_frame_count();
                    std::this_thread::sleep_for(sleep_duration);
                }
            });
        } else {
            Verbosity::FrameLogger::visual_tm().update_frame_count();
        }
        // Initialize performance log
        Verbosity::FrameLogger::get_perf_log();
        while (!game.lost) {
            int key = KeyboardModule::get_key();
            ::Tetris::Input input =
                TetrisGame::Tetris<threaded, target_framerate, rng_seed>::get_input(key);
#ifndef ______________________________PROGRAM_EXIT______________________________________
            if (input == ::Tetris::Input::PROGRAM_EXIT) {
                videocore.stop_displaying();
                game.lost = true;
                r_upd_th.join();
                KeyboardModule::exit_synchronously();
                return;
            }
#endif
            // auto timepoint = Common::GameClock::Clock::now();
            unsigned char n_burned;
            unsigned char y_burned[4];
            auto diff = game_clock.nano_time_diff();
            // Transform time_diff to seconds
            double time_diff = std::min(MAX_UPDATE_INTERVAL,
                static_cast<double>(diff.count()) / static_cast<double>(std::chrono::nanoseconds::period::den));
            
            // double time_diff = game_clock.fp_diff_seconds(timepoint);
            auto last_update = game_clock.reset();
            game.update(input, y_burned, n_burned, true, time_diff);
            Verbosity::FrameLogger::physics_tm().update_frame_count();
            if (threaded) {
                physics_updated = true;
            } else {
                Renderer::MainRenderer::try_update(game, videocore.display_data);
                Verbosity::FrameLogger::visual_tm().update_frame_count();
            }
            if (time_diff * 1000.0 > (ms_frame_dur * 2)) {
                VERBOSITY_LOG("Physics update took " + std::to_string(time_diff) + " s");
            }
            if (ms_frame_dur > 0) {
                std::this_thread::sleep_until(last_update + next_frame_dur);
            }
        }
        // On game lost
        this->rng = game.current_seed.get_value();
        videocore.stop_displaying();

        if (threaded) {
            r_upd_th.join();
        }
    } while (!exit);
}
template <bool threaded, uint64_t target_framerate, Common::ZMath::UInt64RNG::RNGSeed rng_seed>
void TetrisGame::Tetris<threaded, target_framerate, rng_seed>::threaded_update(
    bool& str_upd_done, const ::Tetris::GameModule& game, Renderer::MainRenderer::RenderData& rd) {
    str_upd_done = false;
    Renderer::MainRenderer::try_update(game, rd);
    str_upd_done = true;
}
template <bool threaded, uint64_t target_framerate, Common::ZMath::UInt64RNG::RNGSeed rng_seed>
TetrisGame::Tetris<threaded, target_framerate, rng_seed>::Tetris() : rng(rng_seed), exit(false) {
    videocore = VideoCore::VideoHandler();
}

template <bool threaded, uint64_t target_framerate, Common::ZMath::UInt64RNG::RNGSeed rng_seed>
void TetrisGame::Tetris<threaded, target_framerate, rng_seed>::start() {
    if (threaded) {
        std::thread new_thread = std::thread([this]() { start_game(); });
        new_thread.join();
    } else {
        start_game();
    }
}
