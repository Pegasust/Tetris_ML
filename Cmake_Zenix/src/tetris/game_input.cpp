#include "game_input.h"

Tetris::InputCollection::InputCollection() : collection(), empty(true) {}

void Tetris::InputCollection::add_entry(const GameInput& input, const double& delay) {
    if (empty) {
        collection.push_front({input, delay});
        empty = false;
        return;
    }
    // Optimize delay time
    if (collection.front().input == input) {
        collection.front().delay += delay;
    } else {
        // Add a new entry
        collection.push_front({input, delay});
    }
}

std::string Tetris::entry_string(const InputEntry& entry) {
#ifdef HUMAN_READABLE_SERIALIZATION
    std::string retval = std::to_string(entry.input);
    retval.append(" ");
    retval.append(Common::precise_to_string(entry.delay));
    return retval;
#else
#error(Unimplemented)
#endif
}

Tetris::InputEntry Tetris::deserialize_entry(const std::string& string) {
#ifdef HUMAN_READABLE_SERIALIZATION

#else
#error(Unimplemented)
#endif
}

void Tetris::InputCollection::serialize_self(const std::string& string) {
#ifdef HUMAN_READABLE_SERIALIZATION

#else
#error(Unimplemented)
#endif
}