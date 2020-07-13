/**
 * OLD Purpose: To easily see what information is intended for normal players
 * to see, and the output potentials
 * NEW: To easilly communicate between the AI's output and the game's inputs
 *
 */
#pragma once
#include <limits>
#include "../common/game_clock.h"
#include "../replay/serialize_format.h"
#include "tetris_core.h"
namespace TetrisAPI {
/*
Time will keep on running as long as the game is not lost.
Delta time is calculated from the difference in time from last
game update to this call of game update (which includes time of
game physics update)
* This engine will also keeps track of the inputs flooded into
update() function.
*/
class TetrisExtendedEngine {
protected:
    Common::GameClock::Instance game_clock;
    bool time_initialized;

public:
    using InputCollection = Tetris::InputCollection;

    Common::ZMath::UInt64RNG::RNGSeed initial_seed;
    TetrisEngine engine;
    InputCollection input_collection;
    Common::GameClock::Clock::time_point last_update;
    /*initialize with a random seed*/
    TetrisExtendedEngine();
    /*initialize with a given seed.
    input_reserve is only reserve for future usage and serves no purpose
    with MSVC build (and probably many other builds).
    */
    TetrisExtendedEngine(unsigned long long seed, unsigned int input_reserve = (1 << 16));
    inline bool& lost() { return engine.lost; }
    inline Common::ZMath::UInt64RNG& current_seed() { return engine.current_seed; }
    inline void post_update(const bool& piece_staticized)
    {
        if (piece_staticized) {
            TetrisAPI::reassign_piece(engine);
        }
    }

    /* Initialize with deserialized replay string */
    static TetrisExtendedEngine from_replay(const std::string& replay);

    /*
    Updates the game without reassigning the piece.
    Remember to do engine.reassign after peeking at controlling piece.
    Outputs:
        burn_y[4]: the meaningful information is from 0 to n_burned -1.
            elmeents from n_burned to 4 is preserved and might be inaccurate.
    */
    Tetris::TetrisField& update(
        // inputs
        const Tetris::Input& input,
        // outputs (including the returning field
        bool& update_staticized_last_piece, // whether this update
                                            // staticized the last controlling piece
        TetrisBody& controlling_piece,
        unsigned char burn_y[4], // The y-position that were burned
                                 // in the last update
        unsigned char& n_burned, // The total amount of number burned
                                 // from the last update
        double& delta_time       // (Output) Difference in time since last update in seconds
    );

    /* Perform the least recent key recorded to this game. */
    Tetris::TetrisField& recreate_once(
        bool& out_last_piece_staticized, TetrisBody& out_controlling_piece,
        unsigned char out_burn_y[4], unsigned char& out_n_burned, double& out_seconds_elapsed);
    void reset(unsigned long long seed);
    inline int reset()
    {
        int random = rand();
        reset(random);
        return random;
    }

    /* Reset everything and put in replay. This allows users to save game.
     * Returns the amount of inputs in entry (which is also the number of
     * updates).
     */
    size_t reset(const std::string& replay);

    /* Resets the game to recreate accordingly to input collection.
     * initial_seed is preserved.
     */
    Common::ZMath::UInt64RNG::RNGSeed reset_for_recreation();
    // bool serialize_current_game(const std::string& username = "Zenix");
};
} // namespace TetrisAPI