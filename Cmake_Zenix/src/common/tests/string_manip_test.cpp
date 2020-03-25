#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
#include <stdint.h>
#include "common/keyboard_input.h"
#include "common/string_manip.h"


#define INCLUDE_RYU_TEST
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

Result check(std::uint64_t bits, const double& to_str, const double& precise) {
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

void precision_test(int& to_string, int& precise) {
    std::uint64_t i = std::numeric_limits<std::uint64_t>::max();
    // std::vector<Result> incorrects;
    std::uint64_t to_str_incorrects = 0;
    std::uint64_t precise_incorrects = 0;
    std::uint64_t precise_str_comp = 0; // number of str comparison fails
    #ifdef INCLUDE_RYU_TEST
    std::uint64_t ryu_incorrects = 0;
    std::uint64_t ryu_approx_incorrects = 0;
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
        std::string ryu_str = Common::ryu_d2s(d);
        #endif
        // std::cout << i << "\tstd: " << to_str << "\tprecise: " << precise;
        double d_to_str = Common::crack_atof(to_str.data());
        double d_precise = Common::crack_atof(precise.data());
        #ifdef INCLUDE_RYU_TEST
        double d_ryu = Common::ryu_s2d(ryu_str);
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
        } else {
            precise_incorrects++;
        }
        if (result.result & (1 << 1)) {
            // to_str correct
            // std::cout << "To_str";
        } else {
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

void precise_str() {
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

void ryu_test() {
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

int main() {

    // for (auto i = incorrects.begin(); i != incorrects.end(); ++i) {
    //    std::cout << i->bits << "\tstd: " << i->d_from_to_string;
    //    std::cout << "\tprecise: " << i->d_from_precise;
    //    std::cout << "\tresult: " << i->result;
    //    std::cout << std::endl;
    //}
    // precise_str();
    int t, p;
    precision_test(t, p);
    //ryu_test();
    Common::KeyboardListener<false, 0, std::chrono::microseconds>::get_key();

    return 0;
}