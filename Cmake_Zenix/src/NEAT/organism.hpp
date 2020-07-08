#pragma once
/*
 * An organism contains its historical marking (in its genotype) and the "performer" or agent
 * that acts on the environment (the phenotype). As a whole, the organism will determine whether
 * or not it will cross-breed with another organism, producing a child with structure shared
 * by both parents.

 * In this file, you can find the calculations for DNA difference based on both the genotype
 * and the phenotype.

 * TODO: make a version that keep track of the history of fitness.
 */
#include "genotype.hpp"
namespace NEAT {
template <typename FP_Type = double, typename Neuron_Type = NEAT::NeuronType,
          typename Index_Type = int>
class Organism {
    using T_Genotype = Genotype<Index_Type, FP_Type, Neuron_Type>;
    using T_Organism = typename Organism<FP_Type, Neuron_Type, Index_Type>;
    T_Genotype genotype;
    FP_Type fitness;
    FP_Type highest_fitness;
    FP_Type lowest_fitness;
    FP_Type mean_fitness;
    FP_Type sharing_fitness;
    Index_Type fitness_epochs;

public:
    // Returns the sum of the fitnesses recorded
    FP_Type record_fitness(const FP_Type new_fitness) {
        fitness = new_fitness;
        if (fitness_epochs == 0) {
            highest_fitness = fitness;
            lowest_fitness = fitness;
            mean_fitness = fitness;
            fitness_epochs++;
            return fitness;
        } else {
            if (fitness > highest_fitness) {
                highest_fitness = fitness;
            } else if (fitness < lowest_fitness) {
                lowest_fitness = fitness;
            }
            FP_Type sum_fitness = fitness + (mean_fitness)*fitness_epochs;
            mean_fitness = sum_fitness / (++fitness_epochs);
            return sum_fitness;
        }
    }

    // Resets the records of this organism to be "replaced" by another organism with
    // different params.
    inline void reset() {
        fitness_epochs = 0;
        // Let fitnesses be undefined behavior.
        UNIMPLEMENTED_FEATURE_MSG("Need to reset genotype.");
    }
    void set_sharing_fitness(FP_Type shr_fit) {
        sharing_fitness = shr_fit;
    }
    FP_Type get_fitness() {
        return fitness;
    }
    // Functor that returns whether left org is less than right org in terms of shared
    // fitness.
    struct less_than_shared {
        inline bool operator()(const T_Organism& left, const T_Organism& right) {
            return left.sharing_fitness < right.sharing_fitness;
        }
    };
    // Functor that returns whether left org is less than right org in terms of the 
    // raw fitness score
    struct less_than_raw {
        inline bool operator()(const T_Organism& left, const T_Organism& right) {
            return left.fitness < right.fitness;
        }
    };
};
} // namespace NEAT