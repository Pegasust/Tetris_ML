#pragma once
/* Genotype of a NEAT instance works closely with innovation so that the genes can
 * track its historical markings (via innovation system). Two genes with the same
 * historical origin must represent the same structure (although possibly with different
 * weights).

 * A genotype can be cross-bred with other genotypes by lining up the innovation numbers.
 * Excess and disjoints are taken, if one innovation is disabled by either parent, the child
 * will have its innovation disabled, too.

*/

#include <tuple>
#include "innovation.hpp"
#include "neural_info.hpp"
#include "phenotype.hpp"
namespace NEAT {
// The collection that the genotype uses to store genomic encoding. The key type is innovation
// number.
template <typename Index_Type = int, typename FP_Type = double>
// Maps innov num to InnovLink
using InnovationChain = std::map<Index_Type, InnovationLink<Index_Type, FP_Type>>;
/* emplace example:
 * InnovationChain<> ic;
 * ic.emplace(std::piecewise_construct, std::forward_as_tuple(innov_num),
                std::forward_as_tuple(innov_num));
*/
template <typename Index_Type = int, typename FP_Type = double,
          typename Neuron_Type = NEAT::NeuronType>
class Genotype {
public:
    // This structure can be seen as a DNA.
    using InnovationLinksChain = InnovationChain<Index_Type, FP_Type>;
    using iterator = typename InnovationLinksChain::iterator;
    using const_iterator = typename InnovationLinksChain::const_iterator;
    using Phenotype = PhenotypeTemplate<FP_Type, Neuron_Type, Index_Type>;

private:
    InnovationLinksChain genome;
    Phenotype phenotype;
    Index_Type innovs_disabled;

public:
    Genotype() : genome(), phenotype(), innovs_disabled(0) {}
    inline const InnovationLinksChain& c_link_chain() const noexcept {
        return genome;
    }
    inline InnovationLinksChain& link_chain() {
        return genome;
    }
    // Returns the iterator towards where @param innov_num should locate.
    // Use this->qnot_exist() to find out of the innovation really exists there.
    // Then, if it does exist, do iterator->second to get the InnovationLink.
    inline iterator locate(Index_Type innov_num) {
        return genome.lower_bound(innov_num);
    }
    // Returns whether @param innov_num exists in genome given the supposedly location
    // (@param loc) of innov_num in genome.
    inline bool qnot_exist(const iterator& loc, const Index_Type innov_num) const {
        return loc == genome.end() || loc->first != innov_num;
    }
    // Determines whether @param innov_num exists in genome.
    inline bool not_exist(const Index_Type innov_num) const {
        iterator loc = locate(innov_num);
        return qnot_exist(loc, innov_num);
    }
    // Returns the InnovationLink given an iterator.
    inline InnovationLink<Index_Type, FP_Type>& operator[](const iterator& loc) {
        return loc->second;
    }
    // Returns the InnovationLink given an innovation number. This method is slower than
    // the iterator. It is also unsafe (std::vector out of bounds if not found).
    InnovationLink<Index_Type, FP_Type>& operator[](const Index_Type innov_num) {
        return (*this)[locate(innov_num)];
    }
    // emplace, will replace the previously binded innov_num.
    // @param hint_loc should be the result of this->locate(innov_num).
    inline iterator emplace_hint(const iterator& hint_loc, const Index_Type innov_num,
                                 const FP_Type link_weight = static_cast<FP_Type>(0), 
                                 const bool disabled = false) {
        if (disabled) {
            innovs_disabled += 1;
        }
        return genome.emplace_hint(hint_loc, std::piecewise_construct,
                                   std::forward_as_tuple(innov_num),
                                   std::forward_as_tuple(innov_num, link_weight, disabled));
    }
    // Emplace with the given parameters iff the innovation_num does not exist in genome.
    // @param hint_loc should be the result of this->locate(innov_num).
    inline iterator& emplace_hint_s(iterator& hint_loc, const Index_Type innov_num,
                                    const FP_Type link_weight = static_cast<FP_Type>(0), 
                                    const bool disabled = false) {
        if (qnot_exist(hint_loc, innov_num)) {
            hint_loc = emplace_hint(hint_loc, innov_num, link_weight, disabled);
        }
        return hint_loc;
    }
    // Emplace the given paramters iff the innovation_num does not exist in genome.
    inline iterator emplace_s(const Index_Type innovation_num,
                              const FP_Type link_weight = static_cast<FP_Type>(0),
                              const bool disabled = false) {
        iterator hint_loc = locate(innovation_num);
        return emplace_hint_s(hint_loc, innovation_num, link_weight, disabled);
    }
    // emplace, will replace the previously binded innov_num.
    inline iterator emplace(const Index_Type innov_num,
                            const FP_Type link_weight = static_cast<FP_Type>(0),
                            const bool disabled = false) {
        iterator hint_loc = locate(innov_num);
        return emplace_hint(hint_loc, innov_num, link_weight, disabled);
    }
    // Returns the size of innovations (how many innovations does this genotype has
    // in its history?)
    inline Index_Type innov_size() {
        return genome.size();
    }
    // clears the history
    inline void clear_history() noexcept {
        genome.clear();
    }
    inline void clear() noexcept {
        clear_history();
        innovs_disabled = 0;
        UNIMPLEMENTED_FEATURE_MSG("Need to also clear pphenotype.");
    }
    inline void disable_unsafe(const iterator& loc) {
        (*this)[loc].toggle_enabled();
        innovs_disabled += 1;
    }
    inline void enable_unsafe(const iterator& loc) {
        (*this)[loc].toggle_enabled();
        innovs_disabled -= 1;
    }
    // Returns the number of edges that can be created given the current structure
    Index_Type unconnected_edges() const {
        return phenotype.possible_edges() - phenotype.graph.edges_count() - innovs_disabled;
    }
    // *this is parent 1, other is parent 2. Returns the discourse between the two.
    // Min = 0.0; Max = FP_Type.Max
    FP_Type gene_diff_score(const Genotype& other, FP_Type disjoint_coef = 1,
                               FP_Type excess_coef = 1, const FP_Type weight_coef = 1);
};
} // namespace NEAT

#include "genotype.ipp"