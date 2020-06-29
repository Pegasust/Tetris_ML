#include "../../test_utils/test_utils.hpp"
#include "../rng.h"
#include "../zmath.h"
using namespace CZMath;
bool sigmoid_test() {
    // TEST_(sigmoid(0.0) == 0.5);
    // TEST_(sigmoid(1000.0) == 1.0);
    // TEST_(sigmoid(-1000.0) == -1.0);
    TEST_EQ(sigmoid(0.0), 0.5);
    TEST_EQ(sigmoid(1000.0), 1.0);
    TEST_EQ(sigmoid(-1000.0), 0);
    return true;
}
bool rng_test() {
    const int evals = 10000000;
    constexpr int range = 10;
    constexpr int offset = -5;
    int rdm_range[range] = {0};
    int rdm_offset[range] = {0};
    for (int i = 0; i < evals; i++) {
        int random = CZMath::rng_instance.get_range(range);
        TEST_(random >= 0 && random < range);
        rdm_range[random] += 1;
        int binded_rand = CZMath::rng_instance.get_range(offset, offset + range);
        TEST_(random >= offset, random < offset + range);
        rdm_offset[binded_rand - offset] += 1;
        // range += 1;
        // offset -= 1;
    }
    std::cout << ("rdm_range: ");
    for (int i = 0; i < range; i++) {
        std::cout << rdm_range[i] << ", ";
    }
    std::cout << ("\nrdm_offset: ");
    for (int i = 0; i < range; i++) {
        std::cout << rdm_offset[i] << ", ";
    }
    std::cout << std::endl;
    return true;
}

int main() {
    TEST_INITIALIZE;
    DO_TEST(sigmoid_test);
    DO_TEST(rng_test);
    TEST_RESULTS;
    return 0;
}