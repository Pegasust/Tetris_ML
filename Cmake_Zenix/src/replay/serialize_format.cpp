#include "serialize_format.h"

void TetrisReplay::serialize_replay(std::string& out, const TetrisReplay::RandomSeed& seed,
                                    const std::string& input_collection) {
    out.append(Common::decimal2hex_str<TetrisReplay::RandomSeed>(seed));
    for (int i = 0; i < SEED_MOVES_SPACE; i++) {
        out.append("\n");
    }
    out.append(input_collection);
}

void TetrisReplay::__private__::deserialize_replay(TetrisReplay::RandomSeed& out_initial_seed,
                                                   std::string& out_input_collection_serialized,
                                                   std::istream& serialized_stream) {
    using State = int;
    constexpr State GET_SEED = 1;
    constexpr State GET_MOVE = 2;
    constexpr State FORCE_STOP = std::numeric_limits<State>::max();
    char buffer[TetrisReplay::DESERIALIZE_BUFFER];
    int buffer_len;
    bool out_success;
    State progress = GET_SEED;
    do {
        parse_ignore(out_success, buffer, buffer_len, serialized_stream);
        if (buffer_len == 0) {
            // Empty line, ignore.
            continue;
        }
        // Not empty line.
        switch (progress) {
        case GET_SEED:
            out_initial_seed = Common::hex_str2decimal<TetrisReplay::RandomSeed>(buffer);
            progress++;
            break;
        case GET_MOVE:
            out_input_collection_serialized.append(buffer).append("\n");
            // Currently, there is no way to know this is completed.
            // progress++;
            break;
        case FORCE_STOP:
            return;
        default:
            return;
        }
    } while (!serialized_stream.eof());
}

void TetrisReplay::__private__::parse_ignore(bool& out_success,
                                             char buffer[TetrisReplay::DESERIALIZE_BUFFER],
                                             int& out_buffer_length,
                                             std::istream& serialized_stream) {
// ISTREAM_DELIM_NOT_FOUND returns true if serialized_stream did parse full block
// of buffer, but did not get to a delimiter because buffer is full.
// If serialized_stream fails (eof, stream corruption/injection), this returns false.
#define ISTREAM_DELIM_NOT_FOUND                                                                    \
    serialized_stream.fail() && buffer[0] != '\0' && !(serialized_stream.bad())
// Clear the specified error_bit (should be in the form of std::ios_base::__bit like failbit)
#define ISTREAM_UNSET_BIT(error_bit)                                                               \
    auto state = serialized_stream.rdstate();                                                      \
    state &= ~error_bit;                                                                           \
    serialized_stream.clear(state);
    // end defines

    if (!serialized_stream) {
        out_success = false;
        return;
    }
    serialized_stream.getline(buffer, TetrisReplay::DESERIALIZE_BUFFER);
    out_success = !ISTREAM_DELIM_NOT_FOUND;
    //// Filter buffer out of ignored stuff
    // if (!ISTREAM_DELIM_NOT_FOUND) {
    //    return;
    //}
    // If '\n' is not found after reading DESERIALIZE_BUFFER
    // amount of character on stream.
    for (int i = 0; i < DESERIALIZE_BUFFER; i += 2) {
        switch (buffer[i]) {
        case LINE_IGNORE:
        case WORD_IGNORE:
        case BOUND_IGNORE:
            ++i;
            break;
        }
        if ((i % 2) == 0) {
            //// There is no ignore here
            //// the file contains too large of an input?
            // if (!out_success) {
            //    UNIMPLEMENTED_FEATURE_MSG("Input from stream is too large for buffer");
            //    return;
            //}
            // Continue parsing if buffer retrieved cleanly.
            continue;
        }
        // If odd bit is set, which means
        // buffer[i-1] or buffer[i-2] is one of the ignoring char
        if (buffer[i] != buffer[i - 1]) {
            if (i != 1) {
                i -= 1;
                // check for buffer
                if (buffer[i - 1] != buffer[i]) { // if no sign of ignore, continue
                    continue;
                }
            } else {
                i -= 1;   // Unset odd bit so that it gets back on 2x search.
                continue; // Don't ignore out any word.
            }
        }
        // the ignoring sequence is signalled (2 chars in a sequence)
        // Null terminate the start of the "dirty" part (dirty = ignore)
        buffer[i - 1] = '\0'; // Anything before this is clean. Remember this.
        out_buffer_length = i - 1;
        switch (buffer[i]) {
        case LINE_IGNORE: {
            out_success = true; // found line ignore within overflowed buffer.
            // jackpot!
            CAN_BE_OPTIMIZED_MSG("Memory or read speed?");
            // Loop until find a line break or until eof
            // less readings needed
            char sub_buffer[DESERIALIZE_BUFFER];
            int sub_buffer_size = DESERIALIZE_BUFFER;
            ISTREAM_UNSET_BIT(std::ios_base::failbit);
            // more memory efficient & conform to DESERIALIZE_BUFFER.
            // char* sub_buffer = buffer + i;
            // int sub_buffer_size = DESERIALIZE_BUFFER - i - 1;
            // Since we know for sure there is no newline in this
            // buffer...
            while (serialized_stream) {
                serialized_stream.getline(sub_buffer, sub_buffer_size);
                if (ISTREAM_DELIM_NOT_FOUND) {
                    // Just keep reading unless you face eof or error
                    ISTREAM_UNSET_BIT(std::ios_base::failbit);
                } else {
                    // newline found, that's good.
                    // buffer stays the same.
                    return;
                }
            }
            // EOF without ever reaching a newline...
            // Let's just return for now.
            POTENTIAL_BUG_POINT_MSG("Very negligable since it's told to ignore anyways");
            return;
        } break;
        case WORD_IGNORE: {
            // Actually do the same thing for LINE_IGNORE, but
            // delim is " \t\n\r\f"
            UMIMPLEMENTED_FEATURE;
            return;
        } break;
        case BOUND_IGNORE: {
            // Sub buffer exists after the null terminator of buffer
            // until the end of allocated memory of buffer.
            char* sub_buffer1 = buffer + i;
            int sub_buffer_size1 = DESERIALIZE_BUFFER - i - 1;
            // Whether sub buffer is reallocated out of buffer.
            bool reallocated = false;

            bool previously_found = false; // whether we previously found an ignore char
            bool bound_resolved = false; // whether the ignore bound is resolved and we
            // should copy any thing past the bound but before the line end to buffer.
            while (true) {
                for (int j = 0; j < sub_buffer_size1 - 1; j++) {
                    if (!bound_resolved) {
                        if ((sub_buffer1[j]) != BOUND_IGNORE) {
                            previously_found = false;
                            continue;
                        }
                        if (!previously_found) {
                            previously_found = true;
                            continue;
                        }
                        j += 1;
                        i -= 1;
                    }
                    // Copy all string from right hand side of the
                    // current position of sub_buffer1 to buffer (from null-terminator)
                    POTENTIAL_BUG_POINT_MSG("buffer might not have enough room for sub_buffer1");
                    CAN_BE_OPTIMIZED_MSG("Is assigning out of null necessary?");
                    buffer[i] = ' '; // Assign the previously null-terminator out of null.
                    // The buffer that is the right of the right most ignore character.
                    char* right_sub_buffer1 = (sub_buffer1 + j);
                    auto right_buffer_size = strlen(right_sub_buffer1);
                    out_buffer_length += right_buffer_size;
                    bool enough_size = out_buffer_length <= DESERIALIZE_BUFFER;
                    out_success = enough_size && !ISTREAM_DELIM_NOT_FOUND;
                    ASSERT(enough_size, "Otoke... buffer doesn't have enough size to store.");
                    // Destination is to overwrite the last-placed null terminator.
                    char* dest = (buffer + i);
                    strcpy(dest, right_sub_buffer1);
                    i = out_buffer_length;
                    if (out_success) {
                        if (reallocated) {
                            delete[] sub_buffer1;
                        }
                        return;
                    }
                    bound_resolved = true;
                    if (!reallocated) {
                        sub_buffer1 = buffer + i + 1;
                        sub_buffer_size1 = DESERIALIZE_BUFFER - i;
                    }
                    break;
                }
                if (ISTREAM_DELIM_NOT_FOUND) { // Doesn't matter if it finds new line or
                                               // not.
                    ISTREAM_UNSET_BIT(std::ios_base::failbit);
                }
                if (!serialized_stream) {
                    // problematic, don't try to parse more!
                    break;
                }
                // Allocate more buffer to read quicker. This makes sub_buffer a
                // separate block of memory.
                if (!reallocated && sub_buffer_size1 < ALLOCATE_MORE_THRESHOLD) {
                    reallocated = true;
                    POTENTIAL_BUG_POINT_MSG("Remember to flush sub_buffer1!");
                    sub_buffer1 = new char[DESERIALIZE_BUFFER];
                    // strcpy(sub_buffer1, (buffer + i));
                    sub_buffer_size1 = DESERIALIZE_BUFFER;
                    sub_buffer1[DESERIALIZE_BUFFER - 1] = '\0'; // Null terminate it.
                }
                serialized_stream.getline(sub_buffer1, sub_buffer_size1);
            }
            // Reached eof or faced some error here.
            POTENTIAL_BUG_POINT_MSG("Can be quite troublesome because user used bounded_ignore.");
            // let it slide for now.
            if (reallocated) {
                delete[] sub_buffer1;
            }
            return;
        } break;
        }
    }
    // ISTREAM_UNSET_BIT(std::ios_base::failbit);
    out_buffer_length = strlen(buffer);
#undef ISTREAM_DELIM_NOT_FOUND
#undef ISTREAM_UNSET_BIT
}
