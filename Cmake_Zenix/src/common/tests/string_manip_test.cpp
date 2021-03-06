#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
#include <stdint.h>
#include "common/assertion.hpp"
#include "common/keyboard_input.h"
#include "common/path_consts.hpp"
#include "common/string_manip.h"
#include "test_utils/test_utils.hpp"

//#define INCLUDE_RYU_TEST
union FloatAsBits {
    std::uint64_t bits;
    double fp;
};

struct Result {
    std::uint64_t bits;
    double d_from_to_string;
    double d_from_precise;
    int result;
};

Result check(std::uint64_t bits, const double& to_str, const double& precise)
{
    int result = 0;
    FloatAsBits f = {bits};
    double d = std::move(f.fp);
    if (Common::ZMath::approx_equals(to_str, d)) {
        result |= 1 << 1;
    }
    if (Common::ZMath::approx_equals(precise, d)) {
        result |= 1 << 0;
    }
    return {bits, to_str, precise, result};
}

constexpr int PRINT_EVERY = 10000;

void precision_test(int& to_string, int& precise)
{
    std::uint64_t i = std::numeric_limits<std::uint64_t>::max();
    // std::vector<Result> incorrects;
    std::uint64_t to_str_incorrects = 0;
    std::uint64_t precise_incorrects = 0;
    std::uint64_t precise_str_comp = 0; // number of str comparison fails
#ifdef INCLUDE_RYU_TEST
    std::uint64_t ryu_incorrects = 0;
    std::uint64_t ryu_approx_incorrects = 0;
    char ryu_str[Common::RYU_BUFFER_SIZE];
#endif
    do {
        ++i; // Since i wraps around 0 to max, first iteration is i == 0
        FloatAsBits fp = {i};
        double d = std::move(fp.fp);
        if (std::isnan(d)) {
            // Disregard NaNs
            continue;
        }
        std::string to_str = std::to_string(d);
        std::string precise = Common::precise_to_string(d);
#ifdef INCLUDE_RYU_TEST
        Common::ryu_d2s_buffered(d, ryu_str);
#endif
        // std::cout << i << "\tstd: " << to_str << "\tprecise: " << precise;
        double d_to_str = Common::crack_atof(to_str.data());
        double d_precise = Common::crack_atof(precise.data());
#ifdef INCLUDE_RYU_TEST
        double d_ryu = Common::ryu_cstr2d(ryu_str);
#endif
        Result result = check(i, d_to_str, d_precise);
        // std::cout << "\tresult: " << result.result << "\n";
        if ((i % PRINT_EVERY) == 0) {
            std::cout << "to_str: " << to_str_incorrects << "\tprecise: " << precise_incorrects
                      << "\tprecise_str: " << precise_str_comp << "\t"
#ifdef INCLUDE_RYU_TEST
                      << "\tryu: " << ryu_incorrects << "\tryu_approx: " << ryu_approx_incorrects
#endif
                      << "(" << i << ")" << std::endl;
        }
        if (result.result & 1) {
            // precise correct
            // std::cout << "Precise\t";
        }
        else {
            precise_incorrects++;
        }
        if (result.result & (1 << 1)) {
            // to_str correct
            // std::cout << "To_str";
        }
        else {
            to_str_incorrects++;
        }
        if (precise != Common::precise_to_string(d_precise)) {
            precise_str_comp++;
        }
#ifdef INCLUDE_RYU_TEST
        if (!Common::ZMath::approx_equals(d_ryu, d)) {
            ryu_approx_incorrects++;
        }
        if (d_ryu != d) {
            ryu_incorrects++;
        }
#endif
        // if (result.result != 3) {
        //    incorrects.push_back(result);
        //}
        // std::cout << std::endl;
    } while (i != std::numeric_limits<std::uint64_t>::max());
    // 100000);
    std::cout << "Program exited." << std::endl;
    std::cout << "to_str: " << to_str_incorrects << "\tprecise: " << precise_incorrects
              << "\tprecise_str: " << precise_str_comp << "\t"
#ifdef INCLUDE_RYU_TEST
              << "\tryu: " << ryu_incorrects << "\tryu_approx: " << ryu_approx_incorrects
#endif
              << "(" << i << ")" << std::endl;
    to_string = to_str_incorrects;
    precise = precise_incorrects;
}

void precise_str()
{
    // const double test_values[] = {
    //    1.0,
    //    3.1415926535897932,
    //    2.71828182845904523,
    //};
    const std::uint64_t TESTS = 10000;
    double test_values[TESTS + 3] = {
        1.0,
        3.1415926535897932,
        2.71828182845904523,
    };
    for (std::uint64_t i = 0; i < TESTS; i++) {
        test_values[i + 3] = Common::ZMath::long_bits_2_double(i);
    }
    for (int i = 0; i < (sizeof(test_values) / sizeof(double)); i++) {
        std::cout << "value:   \t" << test_values[i] << "\n";
        std::cout << "precise: \t" << Common::precise_to_string(test_values[i]) << "\n";
        double parsed = Common::crack_atof(Common::precise_to_string(test_values[i]).data());
        std::cout << "parsed:  \t" << parsed << "\n";
        std::cout << "pparsed: \t" << Common::precise_to_string(parsed) << "\n";
        bool precise_is_parsed = parsed == test_values[i];
        std::cout << "same_val:\t" << precise_is_parsed << "\n";
        std::cout << std::endl;
    }
}

void ryu_test()
{
    // const double test_values[] = {
    //    1.0,
    //    3.1415926535897932,
    //    2.71828182845904523,
    //};
    const std::uint64_t TESTS = 10000;
    double test_values[TESTS + 3] = {
        1.0,
        3.1415926535897932,
        2.71828182845904523,
    };
    for (std::uint64_t i = 0; i < TESTS; i++) {
        test_values[i + 3] = Common::ZMath::long_bits_2_double(i);
    }
    for (int i = 0; i < (sizeof(test_values) / sizeof(double)); i++) {
        std::cout << "value:   \t" << test_values[i] << "\n";
        std::cout << "precise: \t" << Common::ryu_d2s(test_values[i]) << "\n";
        double parsed = Common::ryu_s2d(Common::ryu_d2s(test_values[i]).data());
        std::cout << "parsed:  \t" << parsed << "\n";
        std::cout << "pparsed: \t" << Common::ryu_d2s(parsed) << "\n";
        bool precise_is_parsed = parsed == test_values[i];
        std::cout << "same_val:\t" << precise_is_parsed << "\n";
        std::cout << std::endl;
    }
}

void assert_format_move_filename(
    const char* expected, const char* const player,
    const Common::ZMath::UInt64RNG::RNGSeed& game_seed, const char* const suffix = "")
{
    std::string result = Common::PathConsts::format_move_filename(player, game_seed, suffix);
    std::cout << "format_move_filename gives: " << result << std::endl;
    ASSERT(std::string(expected) == result, "format mismatch!");
}
void format_move_filename_test()
{
#define ASSERT_FILENAME(...) assert_format_move_filename(__VA_ARGS__);
    ASSERT_FILENAME("username_deadbeef_", "username", 0xdeadbeef);
    ASSERT_FILENAME("username_12_human_player", "username", 0x12, "human_player");
#undef ASSERT_FILENAME
}

void hex_str2decimal_test()
{
    std::uint64_t result = Common::hex_str2decimal<std::uint64_t>("ff");
    ASSERT(
        result == Common::hex_str2decimal<std::uint64_t>("0xff"),
        "result doesn't match for 0x and not");
    ASSERT(result == 0xff, "hex string parsed wrongly");
}

bool int_2_cstr_test()
{
    char buffer[64];

    Common::int_2_cstr(static_cast<int>(42), buffer, 64);
    TEST_EQ(strcmp(buffer, "42"), 0);
    Common::int_2_cstr(static_cast<unsigned int>(4294967295), buffer);
    TEST_EQ(strcmp(buffer, "4294967295"), 0);
    Common::int_2_cstr(-420, buffer);
    TEST_EQ(strcmp(buffer, "-420"), 0);
    return true;
}
int main()
{
    // for (auto i = incorrects.begin(); i != incorrects.end(); ++i) {
    //    std::cout << i->bits << "\tstd: " << i->d_from_to_string;
    //    std::cout << "\tprecise: " << i->d_from_precise;
    //    std::cout << "\tresult: " << i->result;
    //    std::cout << std::endl;
    //}
    // precise_str();
    // int t, p;
    // precision_test(t, p);
    // ryu_test();
    format_move_filename_test();
    hex_str2decimal_test();
    TEST_INITIALIZE;
    DO_TEST(int_2_cstr_test);

    TEST_RESULTS;
    Common::KeyboardListener<false, 0, std::chrono::microseconds>::get_key();

    return 0;
}