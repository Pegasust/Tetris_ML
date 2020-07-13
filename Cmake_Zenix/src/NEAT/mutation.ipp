#define TEMPLATE_MUTATION Mutation TEMPLATE_SIGNATURE
#define _MUTATION_(field) TEMPLATE_MUTATION ::field
namespace NEAT {
template TEMPLATE_SIGNATURE
Ret_Status _MUTATION_(mutate_add_link)(
    typename _MUTATION_(TInnovDB) & innov_db, typename _MUTATION_(TGenotype) & genotype,
    const Index_Type innov_from, const Index_Type innov_to, const FP_Type weight)
{
    using TInnovDB = typename _MUTATION_(TInnovDB);
    using TGenotype = typename _MUTATION_(TGenotype);

    Index_Type innov_num = innov_db.add_connection_innovation<true>(innov_from, innov_to, genotype);
    genotype.add_link_pheno(innov_num, innov_from, innov_to, weight);
    return _MUTATION_(SUCCESS);
}

template TEMPLATE_SIGNATURE
Ret_Status _MUTATION_(mutate_add_neuron_between)(
    typename _MUTATION_(TInnovDB) & innov_db, typename _MUTATION_(TGenotype) & genotype,
    const Index_Type innov_between, const FP_Type weight_in, const FP_Type weight_out,
    const bool disabled_in, const bool disabled_out, const FP_Type actvn_resp, const FP_Type out_t0)
{
    using TInnovDB = typename _MUTATION_(TInnovDB);
    using TGenotype = typename _MUTATION_(TGenotype);
    Ret_Status retval = _MUTATION_(SUCCESS);
    Index_Type innov_neuron = innov_db.add_node_between(
        innov_between, genotype, weight_in, weight_out, disabled_in, disabled_out);
    CAN_BE_OPTIMIZED_MSG("Maybe change add_link_pheno() and add_neuron_pheno() to mutate_pheno()");
    genotype.add_neuron_pheno(innov_neuron, innov_db.neurtype_at(innov_neuron), actvn_resp, out_t0);
    CAN_BE_OPTIMIZED_MSG(
        "If add_node_between return iterator to previous innov lookup, no find will be needed.");
    const std::array<Index_Type, 2> arr_innovs = (innov_db.add_node_innov_lookup[innov_between]);
    constexpr std::size_t in = 0, out = 1;
    // Add inwards innovation
    InnovationInfo<Index_Type> info = innov_db.innov_at(arr_innovs[in]);
    genotype.add_link_pheno(arr_innovs[in], info.in_node, info.out_node, weight_in);
    // Add outwards innovation
    info = innov_db.innov_at(arr_innovs[out]);
    genotype.add_link_pheno(arr_innovs[out], info.in_node, info.out_node, weight_out);
    return retval;
}
} // namespace NEAT
#undef _MUTATION_
#undef TEMPLATE_MUTATION