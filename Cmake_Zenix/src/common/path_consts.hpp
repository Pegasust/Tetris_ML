#pragma once
#include "file_managing.hpp"
#include "rng.h"
#include "string_manip.h"

namespace Common {
namespace PathConsts {
#define PATH_CONCAT(root, file) STR_CONCAT(root, file)

// Verbosity
#define RELATIVE_LOG_ROOT "./logs/"
#define LOG_ROOT(x) (RELATIVE_LOG_ROOT x)
// Moves
#define _PP_RELATIVE_MOVE_ROOT_ "./moves/"
constexpr char RELATIVE_MOVE_ROOT[] = _PP_RELATIVE_MOVE_ROOT_;
#define MOVE_ROOT(filename) PATH_CONCAT(_PP_RELATIVE_MOVE_ROOT_, filename)
/*
 * Returns string in the format:
 * %player%_%lower_case_hex_game_seed%_%suffix%
 */
inline std::string format_move_filename(const std::string& player, const std::string& hex_seed,
                                        const std::string& suffix) {
    std::string retval(player);
    retval.append("_").append(hex_seed).append("_").append(suffix);
    return retval;
}
/*
 * Returns string in the format:
 * %player%_%lower_case_hex_game_seed%_%suffix%
 */
inline std::string format_move_filename(const char* const player,
                                        const Common::ZMath::UInt64RNG::RNGSeed& game_seed,
                                        const char* const suffix = "") {
    return format_move_filename(std::string(player), 
        Common::decimal2hex_str<Common::ZMath::UInt64RNG::RNGSeed>(game_seed),
        std::string(suffix));
}
} // namespace PathConsts
} // namespace Common