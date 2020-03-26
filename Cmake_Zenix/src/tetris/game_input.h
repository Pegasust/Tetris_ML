/*
 * Stores information on game input. I initially wanted to name this file
 * input.h, but since "input" is considered a keyword in CMake, I decided
 * not to.

 * GameInput and InputCollection are declared in this file.
*/
#pragma once

// Force human readable serialization
#define HUMAN_READABLE_SERIALIZATION

// Deserialize larger strings without out of mem by flushing strings
// as the collection parses from strings.
// #define ASYNC_PARSE

// Optimize collection by attempting to detect inputs
// and add to the delay (not recommended).
/*
 * Known problems: getting inputs like Input::RIGHT, Input::LEFT,
 * Input::CAST_DOWN requires separate input processing.
 */
// #define OPTIMIZE_INPUT_DELAY

#ifndef HUMAN_READABLE_SERIALIZATION
#ifdef DEBUG_DEFINED
#define HUMAN_READABLE_SERIALIZATION
#endif
#endif

#include <string>
#include <queue>
#include "../common/string_manip.h"

namespace Tetris {
/*
 * List of inputs supported by Tetris::GameModule.
 */
enum GameInput : unsigned char {
    NONE,
    DOWN,
    UP,
    LEFT,
    RIGHT,
    CAST_DOWN,
    ROTATE_FORTH,
    ROTATE_BACK,
    PROGRAM_EXIT
};
/*
 * Since GameModule uses Input instead of GameInput
 */
typedef GameInput Input;

struct InputEntry {
    GameInput input;
    double delay;
};

/*
 * Returns a string that represents entry. This string does not end
 * with a newline.
 */
std::string entry_string(const InputEntry& entry);
InputEntry deserialize_entry(const std::string& string);

///*
// If capacity is -1, there is no capacity.
// If capacity is 0, the collection produces NOP
//*/
// template<int capacity>

class InputCollection {
private:
    // A queue that needs to know its recently added element
    using Collection = std::deque<InputEntry>;
    // bool empty;
    // int count;

public:
    /* Resorts to collection functions should they are not implemented.
    */
    Collection collection;
    InputCollection();
    /*
     * Deserializes from serialized_collection
     */
    InputCollection(const std::string& serialized_collection);

    inline bool empty() {
        return collection.empty();
    }
    /*
     * The function takes in 2 args that are supposedly passed in to
     * the GameModule's update function. This method adds an entry to the
     * collection.
     */
    void add_entry(const GameInput& input, const double& delay);
    /*
     * Adds an entry to the collection.
     */
    void add_entry(const InputEntry& entry);
    /* Add all entries from serialized input_collection.*/
    void add_entries(const std::string& serialized_collection);
    /*
     * Make string that can be deserialized.
     * string should be "empty" string as it will be appended through
     * the process.
     *
     * The resulting string will be appended with line-separated
     * entries with trailing newline but no leading newline.
     *
     * Serialized entries are flushed out of memory.
     */
    void serialize_self(std::string& string);
    void clear();
    InputEntry& most_recent();
    InputEntry& least_recent();
    void remove_most_recent();
    void remove_least_recent();
};
} // namespace Tetris
#include "game_input.ipp"