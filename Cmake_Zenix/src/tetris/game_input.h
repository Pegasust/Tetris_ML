/*
 * Stores information on game input. I initially wanted to name this file
 * input.h, but since "input" is considered a keyword in CMake, I decided
 * not to.

 * GameInput and InputCollection are declared in this file.
*/
#pragma once

// Force human readable serialization
#define HUMAN_READABLE_SERIALIZATION

#ifndef HUMAN_READABLE_SERIALIZATION
#ifdef DEBUG_DEFINED
#define HUMAN_READABLE_SERIALIZATION
#endif
#endif

#include <queue>
#include <string>
#include "../common/string_manip.h"

namespace Tetris {
/*
 * List of inputs supported by Tetris::GameModule.
*/
enum GameInput :unsigned char{ NONE, DOWN, UP, LEFT, RIGHT, CAST_DOWN, ROTATE_FORTH, ROTATE_BACK, PROGRAM_EXIT };
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
//If capacity is -1, there is no capacity.
//If capacity is 0, the collection produces NOP
//*/
//template<int capacity>
class InputCollection {
private:
    // A queue that needs to know its recently added element
    using Collection = std::deque<InputEntry>;
    Collection collection;
    bool empty;
    // int count;

public:
    InputCollection();
    InputCollection(const std::string& serialized_collection);
    /*
     * The function takes in 2 args that are supposedly passed in to
     * the GameModule's update function. This method adds an entry to the
     * collection.
    */
    void add_entry(const GameInput& input, const double& delay);
    /*
    * Make string that can be deserialized.
    * string should be "empty" string as it will be appended through
    * the process.
    */
    void serialize_self(const std::string& string);
};
}
#include "game_input.ipp"