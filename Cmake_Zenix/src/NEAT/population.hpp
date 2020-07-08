/*
 * This file contains impolementation of each species (which contains many organisms), and the
 * entire population (which contains species).
 */

#pragma once
#include <vector>
#include "../common/assertion.hpp"
#include "organism.hpp"
#include "neural_info.hpp"
namespace NEAT {

// Species should consist of many organisms that will do shared-fitness.
template <typename FP_Type = double, typename Neuron_Type = NEAT::NeuronType,
          typename Index_Type = int>
class Species {
public:
    using TOrganism = NEAT::Organism<FP_Type, Neuron_Type, Index_Type>;
    using TParams = NEAT::SpeciesParams<FP_Type, Index_Type>;

private:
    std::vector<TOrganism> organisms;
    // The highest fitness this species achieved (raw)
    FP_Type raw_best_fitness;
    // The number of generations that raw_best_fitness
    Index_Type gens_no_improve;
    // The age of this species in generations
    Index_Type gens_total_age;
    // This species is the fittest in the whole population
    bool is_best_species;
    void update_best_fitness(FP_Type new_best_fitness) {
        ASSERT(new_best_fitness > raw_best_fitness,
               "new fitness is not higher than raw_best_fitness");
        raw_best_fitness = new_best_fitness;
        gens_no_improve = 0;
    }
    // Assign shared fitness to each organism in this->organisms. Returns the total shared fitness
    // of this particular species.
    FP_Type assign_shared_fitness(const TParams& params);
    // sort this by shared fitness by non-descending order (s_fitness[n+1] >= s_fitness[n]).
    // This gives this->organisms.begin() the organism with the least shared fitness
    // and this->organisms.end() the organism with the most shared fitness.
    void sort_by_shared_fitness();
};
template<typename FP_Type = double, typename Neuron_Type = NEAT::NeuronType, typename Index_Type = int>
class Population {

};
} // namespace NEAT
#include "population.ipp"