#include "game_input.h"

Tetris::InputCollection::InputCollection()
    : collection() //, empty(true)
{}

Tetris::InputCollection::InputCollection(const std::string& serialized_collection) : collection() {
    add_entries(serialized_collection);
}

void Tetris::InputCollection::add_entry(const GameInput& input, const double& delay) {
    if (collection.empty()) {
        collection.push_back({input, delay});
        return;
    }
#ifdef OPTIMIZE_INPUT_DELAY
    // Optimize delay time
    if (collection.back().input == input) {
        collection.back().delay += delay;
    } else {
#endif
        // Add a new entry
        collection.push_back({input, delay});
#ifdef OPTIMIZE_INPUT_DELAY
    }
#endif
}

void Tetris::InputCollection::add_entry(const InputEntry& entry) {
    collection.push_back(entry);
}

std::string Tetris::entry_string(const InputEntry& entry) {
#ifdef HUMAN_READABLE_SERIALIZATION
    std::string retval = std::to_string(static_cast<int>(entry.input));
    retval.append(" ");
    retval.append(Common::ryu_d2s(entry.delay));
    return retval;
#else
#error(Unimplemented)
#endif
}

void Tetris::deserialize_entry(const std::string& str, GameInput& out_input, double& out_delay) {
#ifdef HUMAN_READABLE_SERIALIZATION
    std::vector<std::string> tokens = Common::split(str, ' ');
    out_input = static_cast<GameInput>(std::stoi(tokens[0]));
    out_delay = Common::ryu_s2d(tokens[1]);
#else
#error(Unimplemented)
#endif
}

Tetris::InputEntry Tetris::deserialize_entry(const std::string& string) {
    GameInput input;
    double delay;
    deserialize_entry(string, input, delay);
    return {input, delay};
}

void Tetris::InputCollection::serialize_self(std::string& string) {
#ifdef HUMAN_READABLE_SERIALIZATION
    while (!collection.empty()) {
        std::string entry_str = entry_string(collection.front());
        collection.pop_front();
        string.append(entry_str).append("\n");
    }
#else
#error(Unimplemented)
#endif
}

void Tetris::InputCollection::serialize_self_retain(std::string& string) const {
#ifdef HUMAN_READABLE_SERIALIZATION
    for (auto entry = collection.cbegin(); entry != collection.cend(); ++entry) {
        std::string entry_str = entry_string((*entry));
        string.append(entry_str).append("\n");
    }
#else
#error(Unimplemented)
#endif
}

void Tetris::InputCollection::add_entries(const std::string& string) {
#ifdef ASYNC_PARSE
#error(Unimplemented)
#else
    std::vector<std::string> entry_strs = Common::split(string, '\n');
    for (std::vector<std::string>::iterator iter = entry_strs.begin(); iter != entry_strs.end();
         ++iter) {
        if (str_entry_whitespace(*iter)) {
            continue;
        }
        GameInput input;
        double delay;
        Tetris::deserialize_entry((*iter), input, delay);
        add_entry(input, delay);
    }
#endif
}

size_t Tetris::InputCollection::overwrite_entries_no_shrink(const std::string& string) {
    std::vector<std::string> entry_strs = Common::split(string, '\n');
    size_t retval;
    size_t min_size = std::min(entry_strs.size(), collection.size());
    // Overwrite until the size of min_size
    for (retval = 0; retval < min_size; retval++) {
        if (str_entry_whitespace(entry_strs[retval])) {
            continue;
        }
        // overwrite collection
        collection[retval] = Tetris::deserialize_entry(entry_strs[retval]);
    }
    // Add more entries if there exists left-over string entries.
    for (; retval < entry_strs.size(); retval++) {
        if (str_entry_whitespace(entry_strs[retval])) {
            continue;
        }
        GameInput input;
        double delay;
        Tetris::deserialize_entry(entry_strs[retval], input, delay);
        add_entry(input, delay);
    }
    return retval;
}

void Tetris::InputCollection::clear() {
    collection.clear();
}

Tetris::InputEntry& Tetris::InputCollection::most_recent() {
    return collection.back();
}

Tetris::InputEntry& Tetris::InputCollection::least_recent() {
    return collection.front();
}

void Tetris::InputCollection::remove_most_recent() {
    collection.pop_back();
}
void Tetris::InputCollection::remove_least_recent() {
    collection.pop_front();
}