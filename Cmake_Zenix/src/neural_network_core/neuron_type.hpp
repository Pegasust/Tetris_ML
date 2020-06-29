#pragma once
#include <string>
#include "../common/common.hpp"
namespace NeuralNetwork {
// The type of Neuron: Input, which reads from list of inputs, Bias, which is const
// during simulation, Hidden, which is a "hidden" node representing the intelligence
// or a complexity to resolve from inputs, Output, which is the node to be outputted
// to the program. The order of declaring NeuronType is vital; otherwise, Graph::rearrange
// will not work correctly.
enum NeuronType : unsigned char { INPUT = 0, BIAS = 1, HIDDEN = 2, OUTPUT = 3, UNDEFINED = 4 };
const char* const nn_neuron_type_lookup[] = {"in", "bias", "<>", "out", "?"};

// Assigns neuron type to cstring. Requires at least length of 8.
template <typename Neuron_Type = NeuronType>
void neuron_type_2_cstr(const Neuron_Type& type, char cstr[], const char* const lookup[]) {
    STR_CPY(cstr, lookup[type]);
}
template <typename Neuron_Type = NeuronType>
void neuron_type_2_const_cstr(const Neuron_Type& type, char* const cstr,
                              const char* const lookup[]) {
    cstr = lookup[type];
}
template <typename Neuron_Type>
void neuron_type_2_cstr(const Neuron_Type& type, char cstr[]) {
    neuron_type_2_cstr(type, cstr, nn_neuron_type_lookup);
}

template <typename T>
// Simple wrapper around any type that contains public method in signature:
// void append_str(std::string&) const
std::string to_string(const T&);

} // namespace NeuralNetwork