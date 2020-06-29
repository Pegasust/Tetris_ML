#include "tetris_extended_engine.h"

TetrisAPI::TetrisExtendedEngine::TetrisExtendedEngine()
    : TetrisAPI::TetrisExtendedEngine::TetrisExtendedEngine(rand()) {}

TetrisAPI::TetrisExtendedEngine::TetrisExtendedEngine(unsigned long long seed,
    unsigned int input_reserve)
    : initial_seed(seed), engine(seed), game_clock(false), time_initialized(false),
      input_collection() {}

TetrisAPI::TetrisExtendedEngine TetrisAPI::TetrisExtendedEngine::from_replay(const std::string& replay) {
    TetrisReplay::RandomSeed seed;
    std::string serialized_inputs;
    TetrisReplay::deserialize_replay(seed, serialized_inputs, replay);
    TetrisAPI::TetrisExtendedEngine retval(seed);
    retval.input_collection.add_entries(serialized_inputs);
    return retval;
}

//TetrisAPI::TetrisExtendedEngine::TetrisExtendedEngine(const std::string& replay) {
//    std::string serialized_inputs;
//    TetrisReplay::deserialize_replay(initial_seed, serialized_inputs, std::istringstream(replay));
//    engine = std::move(TetrisEngine(initial_seed));
//
//}

Tetris::TetrisField& TetrisAPI::TetrisExtendedEngine::update(
    const Tetris::Input& input, bool& update_staticized_last_piece, TetrisBody& controlling_piece,
    unsigned char burn_y[4], unsigned char& n_burned, double& delta_time) {
    // Handle time initilization
    if (!time_initialized) {
        delta_time = 0.0;
        time_initialized = true; // time will be initilized after the
        // if statement
    } else {
        delta_time = std::min(game_clock.fp_diff_seconds_micro(), MAX_UPDATE_INTERVAL);
    }
    last_update = std::move(game_clock.reset());
    TetrisAPI::update(engine, input, delta_time, update_staticized_last_piece, controlling_piece,
                      burn_y, n_burned);
    input_collection.add_entry(input, delta_time);
    return engine.game_field;
}


void TetrisAPI::TetrisExtendedEngine::reset(unsigned long long seed) {
    Common::ZMath::UInt64RNG::RNGSeed copied_seed = seed;
    initial_seed = std::move(seed);
    new_game(std::move(copied_seed), engine);
    time_initialized = false;
}

Tetris::TetrisField& TetrisAPI::TetrisExtendedEngine::recreate_once(
    bool& out_last_piece_staticized, TetrisBody& out_controlling_piece, unsigned char out_burn_y[4],
    unsigned char& out_n_burned, double& out_seconds_elapsed) {
    Tetris::InputEntry least_recent_entry = input_collection.least_recent();
    input_collection.remove_least_recent();
    TetrisAPI::update(engine, least_recent_entry.input, least_recent_entry.delay,
                      out_last_piece_staticized, out_controlling_piece, out_burn_y, out_n_burned);
    out_seconds_elapsed = least_recent_entry.delay;
    return engine.game_field;
}

size_t TetrisAPI::TetrisExtendedEngine::reset(const std::string& replay) {
    TetrisReplay::RandomSeed seed;
    std::string serialized_inputs;
    TetrisReplay::deserialize_replay(seed, serialized_inputs, replay);
    reset(seed);
    return input_collection.overwrite_entries(serialized_inputs);
}

Common::ZMath::UInt64RNG::RNGSeed TetrisAPI::TetrisExtendedEngine::reset_for_recreation() {
    Common::ZMath::UInt64RNG::RNGSeed copied_seed = initial_seed;
    reset(copied_seed);
    return initial_seed;
}