#include "../../common/common.hpp"
#include "../game_input.h"

int hardcoded_input_entry_serialize_test() {
    Tetris::InputEntry entry = {Tetris::NONE, 0.314};
    std::string entry_str = Tetris::entry_string(entry);
    std::cout << "entry_str: " << entry_str << std::endl;
    ASSERT(entry_str == std::string("0 ") + Common::ryu_d2s(0.314), "entry_str mismatch!");
    return 0;
}

int hardcoded_input_entry_deserialize_test() {
    Tetris::InputEntry entry = {Tetris::LEFT, 0.0003154};
    std::string entry_str = Tetris::entry_string(entry);
    std::cout << "entry_str: " << entry_str << std::endl;
    std::cout << "Deserializing..." << std::endl;
    Tetris::InputEntry d = Tetris::deserialize_entry(entry_str);
    std::cout << "deserialzied : " << Tetris::entry_string(d) << std::endl;
    ASSERT(d.delay == entry.delay && d.input == entry.input,
           "Serialized entry does not match with original entry!");
    return 0;
}

int hardcoded_input_collection_test() {
    Tetris::InputEntry entries[] = {{Tetris::NONE, 0.3341},      {Tetris::LEFT, 0.1435},
                                    {Tetris::CAST_DOWN, 1.5535}, {Tetris::NONE, 1.55},
                                    {Tetris::RIGHT, 0.33},       {Tetris::RIGHT, 0.33}};
    Tetris::InputCollection input_collection;
    for (int i = 0; i < Common::array_size(entries); i++) {
        input_collection.add_entry(entries[i].input, entries[i].delay);
    }
    std::string serialized_collection;
    input_collection.serialize_self(serialized_collection);
    std::cout << "Serialized collection: \n" << serialized_collection << std::endl;
    input_collection.clear();
    input_collection.add_entries(serialized_collection);
    for (int i = 0; i < Common::array_size(entries); i++) {
        std::cout << "Current i = " << i << std::endl;
        std::cout << "least_recent: " << Tetris::entry_string(input_collection.least_recent())
                  << std::endl;
        ASSERT(input_collection.least_recent().delay == entries[i].delay &&
               input_collection.least_recent().input == entries[i].input, "Entry mismatch.");
        input_collection.remove_least_recent();
    }
    return 0;
}

int main() {
    hardcoded_input_entry_serialize_test();
    hardcoded_input_entry_deserialize_test();
    hardcoded_input_collection_test();
    return 0;
}