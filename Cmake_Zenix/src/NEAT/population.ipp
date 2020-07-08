#include "population.hpp"
#pragma once
template <typename FP_Type, typename Neuron_Type, typename Index_Type>
FP_Type NEAT::Species<FP_Type, Neuron_Type, Index_Type>::assign_shared_fitness(
    const TParams& params) {
    ASSERT(this->organisms.size() > 0, "Attempting to adjust shared fitness on empty spcies");
    FP_Type fitness_coef = 1.0;
    if (this->gens_total_age < params.gens_young_threshold) {
        // boost fitness coef if young
        fitness_coef *= params.young_fitness_coef;
    }
    if (this->gens_total_age > params.gens_old_threshold) {
        // boost fitness coef if old
        fitness_coef *= params.old_fitness_coef;
    }
    if (this->gens_no_improve > params.gens_no_improve_threshold) {
        // apply punishment if applied.
        if (!params.preserve_best_species || !is_best_species) {
            fitness_coef /= params.no_improve_punish_coef;
        }
    }
    // explicit fitness sharing: divide by the number of individuals in the species.
    const FP_Type n_indivs = static_cast<FP_Type>(organisms.size());
    fitness_coef /= n_indivs;
    FP_Type total_shared_fitness = 0.0;
    // Adjusts sharing fitness foreach individual.
    for (Index_Type i = 0; i < organisms.size(); i++) {
        FP_Type org_fitness = organisms[i].get_fitness;
        if (org_fitness > this->raw_best_fitness) {
            this->update_best_fitness(org_fitness);
        }
        org_fitness *= fitness_coef;
        organisms[i].set_sharing_fitness(org_fitness);
        total_shared_fitness += org_fitness;
    }
    return total_shared_fitness;
}

template <typename FP_Type, typename Neuron_Type, typename Index_Type>
inline void NEAT::Species<FP_Type, Neuron_Type, Index_Type>::sort_by_shared_fitness() {
    std::sort(organisms.begin(), organisms.end(), TOrganism::less_than_shared());
}
