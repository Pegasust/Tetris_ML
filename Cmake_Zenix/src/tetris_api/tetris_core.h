#pragma once

#include "../tetris/game_module.h"

namespace TetrisAPI {
constexpr double DEFAULT_PRE_STATIC_THRESHOLD = 0.8;
constexpr double DEFAULT_DELTA_TIME = 1.0 / 60.0;
typedef Tetris::GameModule TetrisEngine;
using namespace Tetris;

inline TetrisEngine* new_game(const unsigned long long& seed) {
    return new Tetris::GameModule(seed);
}

inline void new_game(const unsigned long long& seed, TetrisEngine& mod) {
    mod = Tetris::GameModule(seed);
}

/*
Only get a copy of the actual field excluding the controlling piece.
*/
TetrisField get_copy_game_field(const TetrisEngine& src);
/**
* controlling is the controlling piece falling down. The value
is assigned after the update.
* game is the game state. Value is reassigned after the call
* input is the input that we want the game to update
* burn_y is the array of y positions that is burned in this update
* n_burned is the number of rows burned in this update [0,4]
* delta_seconds is the amount of time in seconds passed since the
last update.
* pre_static_threshold is the amount of tiles to wait until the piece
is staticized to the game_field. (only tested for [0, 1))
[0, inf) is possible but is untested.
* controlling_staticized is the boolean that represents whether
controlling is staticized in this update.
*/
inline void update(
    // inputs
    TetrisEngine& game, const Input& input, double const& delta_seconds,
    double const& pre_static_threshold,
    // outputs
    bool& controlling_staticized, TetrisBody& controlling, unsigned char burn_y[4],
    unsigned char& n_burned) {
    game.update(input, burn_y, n_burned, false, delta_seconds, controlling_staticized,
                pre_static_threshold);
    controlling = TetrisBody(game.controlling_piece);
}

inline void update(
    // inputs
    TetrisEngine& game, const Input& input, double const& delta_seconds,
    // outputs
    bool& controlling_staticized, TetrisBody& controlling, unsigned char burn_y[4],
    unsigned char& n_burned) {
    game.update(input, burn_y, n_burned, false, delta_seconds, controlling_staticized,
                DEFAULT_PRE_STATIC_THRESHOLD);
    controlling = TetrisBody(game.controlling_piece);
}

inline void update(TetrisEngine& game, const Input& input,
                   // outputs
                   bool& controlling_staticized, TetrisBody& controlling, unsigned char burn_y[4],
                   unsigned char& n_burned) {
    TetrisAPI::update(game, input, DEFAULT_DELTA_TIME, DEFAULT_PRE_STATIC_THRESHOLD,
                      controlling_staticized, controlling, burn_y, n_burned);
}
/*
 * "Resume" the game by reassigning.
*/
inline void reassign_piece(TetrisEngine& game) {
    game.reassign();
}

constexpr void lost(const TetrisEngine& game, bool& output) {
    output = game.lost;
}

constexpr void score(const TetrisEngine& game, double& output) {
    output = game.score;
}

constexpr void highest_score(const TetrisEngine& game, double& output) {
    output = game.highest_score;
}

constexpr void total_rows_burned(const TetrisEngine& game, unsigned long long& output) {
    output = game.n_rows_burned;
}

constexpr void total_tetris_scored(const TetrisEngine& game, unsigned int& output) {
    output = game.tetris_scored;
}

constexpr void times_reached_max_stage(const TetrisEngine& game, unsigned char& output) {
    output = game.n_maxstage;
}

constexpr void current_level(const TetrisEngine& game, Level& output) {
    output = game.current_level;
}

/*
Returns the number of rows (accepts scaled rows) that needs
to be burned to proceed to the next stage.
Scaled rows number simply means that the more rows being burned,
the higher scaled rows is.
Ex:
format: actual rows being burned -> scaled rows
    1 -> 1
    2 -> 2
    3 -> 4
    4 -> 5
*/
constexpr void scaled_rows_required(const TetrisEngine& game, unsigned long long& output) {
    output = game.n_rows;
}

constexpr void game_field(const TetrisEngine& game, TetrisField& output) {
    output = game.game_field;
}

} // namespace TetrisAPI