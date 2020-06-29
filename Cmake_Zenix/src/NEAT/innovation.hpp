#pragma once
/* Innovation is defined at http://nn.cs.utexas.edu/downloads/papers/stanley.gecco02_1.pdf as
 * the mapping of a number (innovation 1, for example), to the network's topological innovation
 * (mutation) like (neuron 1 connects to neuron 2 (weight can be contained, or be unique for
 * each genotype)), and whether it is disabled or not by the genotype.
 * This file contains an innovation system such that whenever there is a mutation in a NEAT
 * instance, that innovation will be assigned the <b>increment</b> of the <i>global innovation
 * number</i> so that we can track the history of a gene.

 * Types of mutations that innovation.hpp should include:
 * NEAT:
 *  add_connection(neuron0, neuron1): Adding the connection between previously UNCONNECTED nodes.
 *  add_node(link): The innovation in which link parttakes in will be disabled, while a new neuron
            will be inserted to that link by creating two new innovations (link.in.out = new_neuron;
            link.out.in = new_neuron).
*/
#include <map>
#include <unordered_map>
#include <vector>
#include "neural_info.hpp"
namespace NEAT {
// InnovationInfo is the one that Innovation database, the Akashic records.
template <typename Index_Type = int>
struct InnovationInfo {
    Index_Type in_node;
    Index_Type out_node;
    bool operator==(const InnovationInfo<Index_Type>& other) const noexcept {
        return in_node == other.in_node && out_node == other.out_node;
    }
};
template <typename Index_Type = int>
std::ostream& dump(std::ostream& o, const InnovationInfo<Index_Type> info) {
    return o << "[in: " << info.in_node << "; y: " << info.out_node << "]";
}

template <typename Index_Type = int>
std::ostream& operator<<(std::ostream& o, const InnovationInfo<Index_Type>& a) {
    return dump(o, a);
}
struct innov_info_hash_fn {
    template <typename Index_Type = int>
    std::size_t operator()(const InnovationInfo<Index_Type>& innov_info) const noexcept {
        static constexpr std::size_t HALF_SIZE_T = ((sizeof(std::size_t) * 8) / 2) - 1;
        std::size_t in_hash = std::hash<Index_Type>{}(innov_info.in_node);
        in_hash ^= (in_hash >> HALF_SIZE_T);
        std::size_t out_hash = std::hash<Index_Type>{}(innov_info.out_node);
        out_hash ^= (out_hash << HALF_SIZE_T);
        return in_hash ^ out_hash;
    }
};

// Innovation is the one that Genome/Genotype holds (not the official innovation)
template <typename Index_Type = int, typename FP_Type = double>
class InnovationLink {
    static constexpr Index_Type LAST_BIT_POSITION = (sizeof(Index_Type) * 8) - 1;
    static constexpr Index_Type GET_ENABLED_MASK = 1 << (LAST_BIT_POSITION);
    // The number of innovation (for O(1) access for innov_db). This value is unique per database.
    Index_Type info;
    FP_Type weight;

private:
    InnovationLink() = delete;

public:
    // By default, the link is not disabled.
    InnovationLink(const Index_Type innovation_num,
                   const FP_Type link_weight = (static_cast<FP_Type>(0.0)), 
                   const bool disabled = false)
        : info(innovation_num), weight(link_weight) {
        if (disabled) {
            set_bit();
        }
    }
    static constexpr Index_Type MAX_INNOVATION_NUMBER = ~GET_ENABLED_MASK;
    // By default, the last bit is 0, which means it's true.
    // 0 if false, 1 if true.
    bool is_enabled() const {
        return !(info & GET_ENABLED_MASK);
    }
    void toggle_enabled() {
        info ^= GET_ENABLED_MASK;
    }
    // innovation_number is const, it cannot be negative.
    Index_Type innovation_number() const {
        return info & (~GET_ENABLED_MASK);
    }

private:
    // Clear the "enabled" bit, this means (in layman's terms), setting it enabled.
    void clear_bit() {
        info &= (~GET_ENABLED_MASK);
    }
    void set_bit() {
        info |= GET_ENABLED_MASK;
    }

public:
    inline void enable() {
        clear_bit();
    }
    inline void disable() {
        set_bit();
    }
    void set_enabled(const bool enabled) {
        enabled ? enable() : disable();
    }
    inline FP_Type get_weight() const {
        return weight;
    }
    inline void set_weight(const FP_Type w) {
        weight = w;
    }
};

struct innov_link_less_cmp {
    template <typename Index_Type = int, typename FP_Type = double>
    bool operator()(const InnovationLink<Index_Type, FP_Type>& lhs,
                    const InnovationLink<Index_Type, FP_Type>& rhs) {
        return lhs.innovation_number < rhs.innovation_number;
    }
};

} // namespace NEAT