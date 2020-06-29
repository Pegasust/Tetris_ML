/*
A header file that contains stuffs useful for writing tests.

Useful macros:
TEST_MSG(bool, msg): test whether bool resolves to true. If not, prints to std::cout msg.
This function requires msg to be string literal. This should be placed in a bool-returning,
0-argument test function.

TEST_(bool): test whether bool resolves to true. If not, prints to std::cout which line
    went wrong. This should be placed in a bool-returning, 0-argument test function.

TEST_INITIALIZE: to be used in the test collection function like main().

DO_TEST(test_func_name): given a test function named test_func_name that returns bool, 
    and takes in 0 arg perform the test. Also prints out whether the test passed or not
    (if test_func_name resolves to true, the test passes, otherwise, the test fails).

Scroll to bottom for example usage.
*/
#pragma once
#ifdef TEST_
#error(TEST_ already defined, cannot use test_utils.hpp)
#else
#include <iostream> // Dependency.
#include <vector>
#define TEST_MSG(bool_statement, msg)                                                              \
    if (!(bool_statement)) {                                                                       \
        std::cout << msg << std::endl;                                                             \
        return false;                                                                              \
    }

#define TEST_(bool_statement)                                                                       \
    TEST_MSG(bool_statement, #bool_statement " evaluates to false at line " << __LINE__)
#endif

#define TEST_EQ(lhs, rhs)\
TEST_MSG(lhs == rhs, #lhs "(" <<lhs<< ") does not equal " #rhs "(" <<rhs<<") at line " << __LINE__)

// Test whether lhs != rhs returns true.
#define TEST_NE(lhs, rhs)\
TEST_MSG(!(lhs == rhs), #lhs "(" << lhs << ") does not not equal(!(lhs==rhs)) " #rhs "(" << rhs << ") at line " << __LINE__)
#define TEST_INITIALIZE bool __test_passed_42__ = false; std::vector<bool> __results_42__;
#define DO_TEST(test_func)                                                                         \
    std::cout << "\n\n\n\n\n" << #test_func << ":" << std::endl;                                   \
    __test_passed_42__ = ##test_func(); __results_42__.push_back(__test_passed_42__);                                                                  \
    std::cout << #test_func << (__test_passed_42__ ? " passed" : " failed") << std::endl;

#define TEST_RESULTS bool __all_passed__ = true; std::cout <<"\n\n\n\n\n" << std::endl;\
for (int i = 0; i < __results_42__.size(); i++) {                                                     \
if(!__results_42__[i]){__all_passed__ = false; std::cout<< "Test " << i << " failed." << std::endl;}\
    }\
if (__all_passed__) {                                                                          \
    std::cout << "All tests passed." << std::endl;                                                                            \
    }
/*
// example_test.cpp

bool test_pointer() {
    int num = 42;
    int num_ptr = &num;
    // If this is wrong, the function returns false and prints this line#
    TEST_(*num_ptr == num); 
    num = 65;
    // If this is wrong, the function returns false and prints
    // "Something is wrong with the compiler" (no '\"') on std::cout.
    TEST_MSG(num == 65, "Something is wrong with the compiler");
    TEST_(*num_ptr == num);
    // We have passed all tests.
    return true;
}

int main() {
    std::cout << "Test cases are being presented." << std::endl;
    TEST_INITIALIZE;
    DO_TEST(test_pointer);
}

*/