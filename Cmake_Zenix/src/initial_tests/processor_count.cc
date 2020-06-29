// Cmake_Zenix.h : Include file for standard system include files,
// or project specific include files.

#pragma once

//#include <iostream>

// TODO: Reference additional headers your program requires here.

#if !defined(CMAKE_N_PROCESSOR_DETECTED)
#define N_PROCESSOR_DETECTED 1 // defaults to single core performance
#else
#define N_PROCESSOR_DETECTED CMAKE_N_PROCESSOR_DETECTED
#endif
#include <vector>
#include "../test_utils/test_utils.hpp"

bool double_ref_test() {
    double x = 10.0;
    double* modifier = &x;
    *modifier = 100.0;
    TEST_EQ(x, 100.0);
    std::vector<double*> modifier_vector;
    modifier_vector.push_back(&x);
    *modifier_vector[0] = 10.0;
    TEST_EQ(x, 10.0);
    return true;
}
template <typename FP_Type = double>
struct FPWrapper {
    FP_Type f;
};
template <typename FP_Type = double>
class FPWrapperOut {
public:
    std::vector<FPWrapper<FP_Type>> vect;
    inline void refs_out(std::vector<FP_Type*> & refs) {
        for (int i = 0; i < vect.size(); i++) {
            refs.push_back(&(this->vect[i].f));
        }
    }
};
bool double_ref_template_encapsulation_test() {
    FPWrapperOut<double> out;
    out.vect.push_back({10.0});
    double* modifier = &(out.vect[0].f);
    *modifier = 100.0;
    TEST_EQ(out.vect[0].f, 100.0);
    std::vector<double*> modifier_vect;
    modifier_vect.push_back(&(out.vect[0].f));
    *modifier_vect[0] = 10.0;
    TEST_EQ(out.vect[0].f, 10.0);

    out.refs_out(modifier_vect);
    *modifier_vect[1] = 69.420;
    TEST_EQ(out.vect[0].f, 69.420);
    return true;
}

int main() {
    TEST_INITIALIZE;
    DO_TEST(double_ref_test);
    DO_TEST(double_ref_template_encapsulation_test);
    TEST_RESULTS;
    return 0;
}