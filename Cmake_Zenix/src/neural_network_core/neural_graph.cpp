#include "neural_network_core.hpp"

namespace NeuralNetwork {
void neuron_type_2_cstr(const NeuronType& type, char cstr[]) {
    static const char* const lookup[] = {"in", "bias", "<>", "out", "?"};
    STR_CPY(cstr, lookup[type]);
}
}