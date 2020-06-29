#pragma once

template <bool threaded, uint64_t target_framerate, Common::ZMath::UInt64RNG::RNGSeed rng_seed>
void TetrisGame::Tetris<threaded, target_framerate, rng_seed>::start_game() {

    INITIALIZE_VERBOSITY_LOGGER;
    const uint64_t ms_frame_dur = static_cast<uint64_t>(1000) / (target_framerate);
    const auto next_frame_dur = std::chrono::duration<uint64_t, std::milli>(ms_frame_dur);
    VERBOSITY_LOG("ms_frame_dur = " + std::to_string(ms_frame_dur));
    VERBOSITY_LOG("Asking user for name.");
    std::cout << "Please enter your name. [default: Zenix]" << std::endl;
    std::string username = Common::KeyboardListener<false, 5>::get_cin_line("Zenix");
    std::cout << "Username: " << username << std::endl;
    VERBOSITY_LOG("Username: " + username);
    // VERBOSITY_LOG("Successfully create a new log");
    volatile bool renderer_initialized = false;
    TetrisAPI::TetrisExtendedEngine game(this->rng);
    do {
        if (game.lost()) {
            VERBOSITY_LOG("\nResetting the game instance.");
            game.reset(this->rng);
        }
        videocore.pause_displaying();
        std::cout << "RNG: " << Common::decimal2hex_str(this->rng) << std::endl;
        VERBOSITY_LOG("RNG: " + Common::decimal2hex_str(this->rng));
        std::cout << "Press any button to start playing" << std::endl;
        Common::KeyID key = Common::SynchronousKeyboard::get_key();
        VERBOSITY_LOG("User pressed " << key);
        videocore.resume_displaying();
        // game_clock = Common::GameClock::Instance();
        // This thread draw renderer buffer to end-user client.
        videocore.start_async_display(&videocore, game.engine);
        std::thread r_upd_th; // Render update thread
        volatile bool physics_updated = true;
        if (!renderer_initialized) {
            Renderer::MainRenderer::try_initialize(game.engine, videocore.display_data);
            renderer_initialized = true;
        }
        // Get data from game data to draw into renderer buffer (visual update)
        if (threaded) {
            r_upd_th = std::thread([&, this]() {
                static const auto sleep_duration =
                    std::chrono::duration<int, std::milli>(std::max(1, FRAMETIME_MILLIS));
                while (!game.lost()) {
                    if (physics_updated) {
                        Renderer::MainRenderer::try_update(game.engine, videocore.display_data);
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
        while (!game.lost()) {
            int key = KeyboardModule::get_key();
            ::Tetris::Input input =
                TetrisGame::Tetris<threaded, target_framerate, rng_seed>::get_input(key);
#ifndef ______________________________PROGRAM_EXIT______________________________________
            if (input == ::Tetris::Input::PROGRAM_EXIT) {
                videocore.stop_displaying();
                game.lost() = true;
                r_upd_th.join();
                KeyboardModule::exit_synchronously();
                return;
            }
#endif
            unsigned char n_burned;
            unsigned char y_burned[4];
            double time_diff;
            bool piece_staticized;
            ::Tetris::TetrisBody controlling_body;
            game.update(input, piece_staticized, controlling_body, y_burned, n_burned, time_diff);
            game.post_update(piece_staticized);
            Verbosity::FrameLogger::physics_tm().update_frame_count();
            if (threaded) {
                physics_updated = true;
            } else {
                Renderer::MainRenderer::try_update(game.engine, videocore.display_data);
                Verbosity::FrameLogger::visual_tm().update_frame_count();
            }
            if (time_diff * 1000.0 > (ms_frame_dur * 2)) {
                VERBOSITY_LOG("Physics update took " + std::to_string(time_diff) + " s");
            }
            if (ms_frame_dur > 0) {
                std::this_thread::sleep_until(game.last_update + next_frame_dur);
            }
        }
        // On game lost
        VERBOSITY_LOG("Game lost.");
        VERBOSITY_LOG("Total moves recorded: " << game.input_collection.collection.size());
        this->rng = game.current_seed().get_value();
        videocore.stop_displaying();
        // Find the next path to save this replay
        std::string dummy_path = Common::PathConsts::RELATIVE_MOVE_ROOT;
        VERBOSITY_LOG(
            "Attempting to create move directory if not existed. Dir: " _PP_RELATIVE_MOVE_ROOT_);
        Common::Paths::create_directory_if_not_existed(dummy_path);
        dummy_path += (Common::Paths::format_move_filename(username.c_str(), game.initial_seed,
                                                           "tetris_game_" __DATE__));
        std::string path;
        Common::Paths::next_available_filename(dummy_path, path);
        VERBOSITY_LOG("Saving replay path at " + path);
        // Create this new file
        std::fstream replay_file = Common::Paths::create_file(path);
        CAN_BE_OPTIMIZED_MSG("Getting string here might take up quite some memory if not flushed");
        // Serialize this replay into string
        std::string serialize_str;
        // Do not retain the moves because user is lost.
        TetrisReplay::serialize_replay<false>(serialize_str, game.initial_seed,
                                              game.input_collection);
        replay_file << serialize_str;
        VERBOSITY_LOG("Finished writing to file.");
        VERBOSITY_LOG("Closing file.");
        std::cout << "Saved moves in " << path << std::endl;
        replay_file.close();
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
