#pragma once
template <typename Index_Type, typename FP_Type, typename Neuron_Type>
FP_Type NEAT::Genotype<Index_Type, FP_Type, Neuron_Type>::gene_compatibility(
    const Genotype& other, FP_Type disjoint_coef, FP_Type excess_coef, const FP_Type weight_coef) {
    iterator this_iter = c_link_chain().begin();
    iterator other_iter = other.c_link_chain().begin();
    FP_Type discourse = static_cast<FP_Type>(0);
    // normalize disjoin excess coefficient
    Index_Type max_size = (std::max(innov_size(), other.innov_size()));
    // For coefficients for disjoint and excess, we need to normalize it to the innovation
    // size of the parents.
    FP_Type normalizer = 1.0 / static_cast<double>(max_size == 0 ? 1 : max_size);
    disjoint_coef *= normalizer;
    excess_coef *= normalizer;

    while (this_iter != c_link_chain().end() && other_iter != other.c_link_chain().end()) {
        if ((*this)[this_iter].innovation_number() > other[other_iter].innovation_number()) {
            // disjoint, but advance other
            discourse += disjoint_coef;
            ++other_iter;
        } else if ((*this)[this_iter].innovation_number() < other[other_iter].innovation_number()) {
            // disjoint, but advance only this
            discourse += disjoint_coef;
            ++this_iter;
        } else {
            // equals. Discourse is the difference between two weights * weight_coef.
            discourse +=
                (std::fabs((*this)[this_iter].get_weight() -= other[other_iter].get_weight())) *=
                weight_coef;
            ++other_iter;
            ++this_iter;
        }
    }
    while (this_iter != c_link_chain().end()) {
        // excess
        discourse += excess_coef;
        ++this_iter;
    }
    while (other_iter != other.c_link_chain().end()) {
        // excess
        discourse += excess_coef;
        ++other_iter;
    }
    return discourse;
}