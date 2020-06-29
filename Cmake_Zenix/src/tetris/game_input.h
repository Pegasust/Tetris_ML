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
void deserialize_entry(const std::string& str, GameInput& out_entry, double& out_delay);
;

///*
// If capacity is -1, there is no capacity.
// If capacity is 0, the collection produces NOP
//*/
// template<int capacity>

class InputCollection {
protected:
    // A queue that needs to know its recently added element
    using Collection = std::deque<InputEntry>;
    static inline bool str_entry_whitespace(const std::string& str) {
        return str.length() <= 2;
    }
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
     * collection. This function also attempts to move InputEntry instead
     * of copying and deallocating.
     */
    void add_entry(const GameInput& input, const double& delay);
    /*
     * Adds an entry to the collection. Does not have indications that
     * this function attempts to move.
     */
    void add_entry(const InputEntry& entry);
    /* Add all entries from serialized input_collection.*/
    void add_entries(const std::string& serialized_collection);

    /* Overwrites entries from serialized input_collection. This does not
     * flush the forwarding elements, nor does it shrink size. This
     * does grow in size to append all entries.
     * Returns the amount of entries overwritten.
     */
    size_t overwrite_entries_no_shrink(const std::string& serialized_collection);
    /* Overwrites entries from serialized input_collection. This can flush
     * unused. This will shrink in size if user specifiy it to. This
     * does grow in size to append all entries.
     * Returns the amount of entries overwritten.
     */
    template <bool shrink = true>
    size_t overwrite_entries(const std::string& serialized_collection) {
        size_t retval = overwrite_entries_no_shrink(serialized_collection);
        if (shrink) {
            collection.resize(retval);
        }
        return retval;
    }
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

    /*
     * Make string that can be deserialized.
     * string should be "empty" string as it will be appended through
     * the process.
     *
     * The resulting string will be appended with line-separated
     * entries with trailing newline but no leading newline.
     *
     * Serialized entries are retained
     */
    void serialize_self_retain(std::string& string) const;
    void clear();
    /*
     * Returns the most recent entry without removing it.
     */
    InputEntry& most_recent();
    /*
     * Returns the least recent entry without removing it.
     */
    InputEntry& least_recent();
    /*
     * Removes the most recent entry of the collection.
     */
    void remove_most_recent();
    /*
     * Removes the most recent entry of the collection.
     */
    void remove_least_recent();
};
} // namespace Tetris
#include "game_input.ipp"