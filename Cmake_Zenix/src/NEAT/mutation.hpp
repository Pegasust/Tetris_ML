#include "neural_info.hpp"
#include "innovation_database.hpp"
#include "genotype.hpp"
#include "common/assertion.hpp"
#include <array>
namespace NEAT {

#define TEMPLATE_SIGNATURE \
<typename Index_Type, typename FP_Type, typename Neuron_Type, typename Ret_Status>

template <
    typename Index_Type = int, typename FP_Type = double, typename Neuron_Type = NEAT::NeuronType,
    typename Ret_Status = Index_Type>
class Mutation {
protected:
    using TInnovDB = InnovationDatabase<Index_Type, FP_Type, Neuron_Type>;
    using TGenotype = Genotype<Index_Type, FP_Type, Neuron_Type>;

public:
    static constexpr Ret_Status SUCCESS = static_cast<Ret_Status>(0);

    // Add a link from @param innov_from to @param innov_to with @param weight
    // on @param innov_db and @param genotype.
    // Pre-condition: @param innov_from and @param innov_to exist in innov_db, and
    // their graph counterparts exist in @param genotype
    // Returns SUCCESS.
    static Ret_Status mutate_add_link(
        TInnovDB& innov_db, TGenotype& genotype, const Index_Type innov_from,
        const Index_Type innov_to, const FP_Type weight = static_cast<FP_Type>(1));

    // Add a neuron in between an innovation link (innovation num @param innov_between)
    // on @param innov_db, @param genotype. @param _in and _out are respectively
    // the connection between the in node to new neuron and the new neuron
    // to the out node.
    // Pre-condition: @param innov_between exist in innov_db and genotype and the
    // genotype's grpah edge should not be disabled.
    // Post-condition: @param genotype will have graph edge of @param innov_between
    // disabled, and 2 new edges (in -> new_node, new_node -> out) are added to @param genotype.
    // Returns SUCCESS.
    static Ret_Status mutate_add_neuron_between(
        TInnovDB& innov_db, TGenotype& genotype, const Index_Type innov_between,
        const FP_Type weight_in = static_cast<FP_Type>(1),
        const FP_Type weight_out = static_cast < FP_Type(1), const bool disabled_in = false,
        const bool disabled_out = false, const FP_Type actvn_resp = static_cast<FP_Type>(1),
        const FP_Type out_t0 = static_cast<FP_Type>(0));
};
} // namespace NEAT
#include "mutation.ipp"

#undef TEMPLATE_SIGNATURE