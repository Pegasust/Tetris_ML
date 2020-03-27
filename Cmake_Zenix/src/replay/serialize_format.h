/*
 * Replay file format:

 * IGNORES: (Not yet support)
 * '##' ignores the whole line                   (equivalent to C's //)
 * '$$' ignores the word (next " \t\n\r\f")      (equivalent to C's / *word * /), not supported yet
 * '//' ignores everything until we find '//'    (equivalent to C's /* )
 * FORMAT:
 * <Line 1>: first non-ignore is hexadecimal seed (no 0x suffix)
 * <As many blank lines as needed>
 * <Line-separated move sequences>
*/

#pragma once
#include <istream>
#include <string>
#include "../common/common.hpp"
#include "../tetris/game_input.h"



namespace TetrisReplay {
constexpr unsigned int SEED_MOVES_SPACE = 2;
constexpr int DESERIALIZE_BUFFER = 64; // MUST BE EVEN NUMBER
constexpr int ALLOCATE_MORE_THRESHOLD = DESERIALIZE_BUFFER / 8;
constexpr char LINE_IGNORE = '#';
constexpr char WORD_IGNORE = '$';
constexpr char BOUND_IGNORE = '/';
using RandomSeed = Common::ZMath::UInt64RNG::RNGSeed;
/*outputs the replay format to out_serialize_string*/
void serialize_replay(std::string& out_serialize_string,
                      const RandomSeed& initial_seed,
                      const std::string& input_collection_serialized);
/*
 * Outputs the replay format to out_serialize_string.
 */
template <bool retain = true>
void serialize_replay(std::string& out_serialize_string,
                      const RandomSeed& initial_seed,
                      Tetris::InputCollection& collection) {
    std::string serialized_input;
    if (retain) {
        collection.serialize_self_retain(serialized_input);
    } else {
        collection.serialize_self(serialized_input);
    }
    serialize_replay(out_serialize_string, initial_seed, serialized_input);
}

namespace __private__ {
/*
 * Takes in input stream from serialized_stream. This does not close the stream.
 */
void deserialize_replay(RandomSeed& out_initial_seed,
                        std::string& out_input_collection_serialized,
                        std::istream& serialized_stream);
/*
 * Get one line out of serialized_stream. Attempt to sanitize input out of IGNORES.
 */
void parse_ignore(bool& out_success, char out_buffer[TetrisReplay::DESERIALIZE_BUFFER],
                  int& out_buffer_length, std::istream& serialized_stream);
} // namespace __private__

inline void deserialize_replay(RandomSeed& out_initial_seed,
                               std::string& out_input_collection_serialized,
                               std::istream& serialized_stream) {
    __private__::deserialize_replay(out_initial_seed, out_input_collection_serialized,
                                    serialized_stream);
}
inline void deserialize_replay(RandomSeed& out_initial_seed,
                               std::string& out_input_collection_serialized,
                               const std::string& string) {
    deserialize_replay(out_initial_seed, out_input_collection_serialized,
                                    std::istringstream(string));
}
template <bool close_stream = true>
void deserialize_replay(RandomSeed& out_initial_seed,
                        std::string& out_input_collection_serialized,
                        std::ifstream& serialized_stream) {
    deserialize_replay(out_initial_seed, out_input_collection_serialized, serialized_stream);
    if (close_stream) {
        serialized_stream.close();
    }
}
} // namespace TetrisReplay