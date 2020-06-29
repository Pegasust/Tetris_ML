/*
 * This file contains information on the NEAT project, for example, the NeuralType it
 * uses, and the underlying structures of Neurons and Links for Genomes.
 */
#pragma once
#include <array>
#include <set>
#include "../neural_network_core/neural_network_core.hpp"
namespace NEAT {
// ==== ORDER: INPUT, BIAS, OUTPUT, HIDDEN, UNDEFINED ====
// Putting HIDDEN after OUTPUT because INPUT and OUTPUT are const (for classic NEAT)
// for each problem.
enum NeuronType : unsigned char { INPUT = 0, BIAS = 1, OUTPUT = 2, HIDDEN = 3, UNDEFINED = 4 };
const char* const neat_neuron_type_lookup[] = {"in", "bias", "out", "<>", "?"};
// The neuron type to be used by the genotype.
template <typename Neuron_Type = NEAT::NeuronType>
struct GenomeNeuron {
    const Neuron_Type type;
    GenomeNeuron(const Neuron_Type neuron_type) : type(neuron_type) {}
};
template <typename FP_Type = double, typename Index_Type = int>
struct SpeciesParams {
    // if the age of the species < gens_young_threshold, it is considered young
    Index_Type gens_young_threshold;
    // the multiplier that is applied to the shared fitness if species is considered young
    FP_Type young_fitness_coef;
    // if the age of the species > gens_old_threshold, it is considered old
    Index_Type gens_old_threshold;
    // the multiplier applied to shared fitness of an individual if species is considered old
    FP_Type old_fitness_coef;
    // how many gens of no improvement will the species be attempted to be wiped out
    Index_Type gens_no_improve_threshold;
    // the fitness will be divided by this value. The higher this is, the more likely the
    // individual will not have offsprings, and thus, extinct.
    FP_Type no_improve_punish_coef;
    // if we preserve best species, if the best species does not have improvements, the 
    // no_improve_punish_coef will not be applied
    bool preserve_best_species;
};
//// The link type to be used by the genotype.
// template <typename FP_Type = double, typename Index_Type = int>
// struct GenomeLink {
//    FP_Type weight;
//    // The index on the Innovation Database's genotype_neurons.
//    Index_Type node_in_index;
//    Index_Type node_out_index;
//};

//// Genotype link node (To be replaced with InnovationLink).
// template <typename FP_Type = double, typename Index_Type = int>
// struct GenotypeLink {
//    FP_Type weight;
//    // The index on the Innovation Database's genotype_neurons (maybe reduntdant
//    // due to GenotypeLinkLookup).
//    Index_Type node_in_index;
//    // The index on the Innovation Database's genotype_neurons (maybe reduntdant
//    // due to GenotypeLinkLookup).
//    Index_Type node_out_index;
//};
} // namespace NEAT

//// The collection that the genotype uses to store genomic encoding. (Moved to innovation.hpp)
// template<typename Innov, typename Comp_Func>
// using InnovationChain = std::set<Innov, Comp_Func>;

namespace NeuralNetwork {
// Override neuron_type_2_cstr
template <>
void neuron_type_2_cstr<NEAT::NeuronType>(const NEAT::NeuronType& type, char cstr[]) {
    neuron_type_2_cstr(type, cstr, NEAT::neat_neuron_type_lookup);
}
} // namespace NeuralNetwork