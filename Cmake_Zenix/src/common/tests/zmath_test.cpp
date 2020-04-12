#include "../zmath.h"
#include "../../test_utils/test_utils.hpp"
using namespace CZMath;
bool sigmoid_test() {
    //TEST_(sigmoid(0.0) == 0.5);
    //TEST_(sigmoid(1000.0) == 1.0);
    //TEST_(sigmoid(-1000.0) == -1.0);
    TEST_EQ(sigmoid(0.0), 0.5);
    TEST_EQ(sigmoid(1000.0), 1.0);
    TEST_EQ(sigmoid(-1000.0), 0);
    return true;
}

int main() {
    TEST_INITIALIZE;
    DO_TEST(sigmoid_test);
	return 0;
}