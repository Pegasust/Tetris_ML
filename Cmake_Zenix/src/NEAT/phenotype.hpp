#pragma once
/* A phenotype is the part of the AI that interacts with the environment.
 * It is not capable of serializing itself, and requires the genotype to
 * encode, serialize, and deserialize this phenotype.
 * It is basically the decision-maker and the sensor-receiver of the algorithm
 */
#include <vector>
#include "../neural_network_core/neural_network_core.hpp"
#include "innovation.hpp"
#include "neural_info.hpp"
namespace NEAT {
template <typename FP_Type = double, typename Neuron_Type = NeuronType, typename Index_Type = int>
class PhenotypeMetaTemplate {
public:
    using NGraph = NeuralNetwork::VolatileGraph<FP_Type, Neuron_Type, Index_Type>;
    // using NewNeuron = typename NGraph::VertexCreate;
    // using NewLink = typename NGraph::EdgeCreate;
    //using NewNeurons = typename NGraph::TypeEndArray;
    //struct NewLink {
    //    Index_Type in_neuron_idx;
    //    Index_Type out_neuron_idx;
    //    FP_Type weight;

    //};
    // Collection of neurons to be initialized
    //using NewNeurons = std::vector<NewNeuron>;
    // Collection of links to be initialized.
    //using NewLinks = std::vector<NewLink>;
    /* The following threw error (without typename)
    using NewNeurons = std::vector<Instance::VertexCreate>;
    using NewLinks = std::vector<Instance::EdgeCreate>;
    */
    // The floating point being used to represent the floats
    using FP = FP_Type;
    // The vector of floating point values
    using FPVector = std::vector<FP>;
    // Vector of FP addresses of node's outputs
    //using NodeSyncFPVector = std::vector<FP*>;
    using TypeEndArray = typename NGraph::TypeEndArray;
};
/*
 * This phenotype template, unlike neural graph, will make the order: INPUT, BIAS,
 *
 */
template <typename FP_Type = double, typename Neuron_Type = NeuronType, typename Index_Type = int>
class PhenotypeTemplate {
private:
    using Meta = typename PhenotypeMetaTemplate<FP_Type, Neuron_Type, Index_Type>;

public:
    using NGraph = typename Meta::NGraph;
    //using NodeSyncFPVector = typename Meta::NodeSyncFPVector;
    // using NewNeuron = typename Meta::NewNeuron;
    //using NewLink = typename Meta::NewLink;
    // using NewNeurons = typename Meta::NewNeurons;
    //using NewLinks = typename Meta::NewLinks;
    using FP = typename Meta::FP;
    using FPVector = typename Meta::FPVector;
    constexpr Neuron_Type TypeArraySize = Neuron_Type::UNDEFINED;
    //using TypeEndArray = std::array<Index_Type, TypeArraySize>;
    using TypeEndArray = typename NGraph::TypeEndArray;
    using NNeuron = typename NGraph::Vertex;
    using NLink = typename NGraph::Edge;

public:
    NGraph graph;
    // According to NEAT::NeuronType, only the last end will be extended.
    TypeEndArray type_end;

protected:
    /*
     * Get the starting index of the vertex of @param type.
     */
    inline Index_Type get_start(Neuron_Type type) {
        if (type == static_cast<Neuron_Type>(0)) {
            return 0;
        }
        return type_end[(static_cast<Index_Type>(type) - 1)];
    }
    inline Index_Type get_length(Neuron_Type type) {
        if (type == (Neuron_Type::UNDEFINED - 1)) {
            return graph.vertices_count() - graph.type_end[type];
        }
        return type_end[type + 1] - type_end[type];
    }
    inline TypeEndArray& get_type_length_arr(TypeEndArray& out_type_length) {
        for (Index_Type i = 0; i < out_type_length.size(); i++) {
            out_type_length[i] = get_length(static_cast<Neuron_Type>(i));
        }
        return out_type_length;
    }

public:
    // Returns the max number of edges the current graph can have (does not account for
    // disabled edges) nor the total amount of edges presenting in the graph.
    Index_Type possible_edges() {
        Index_Type n_inputs = get_length(Neuron_Type::INPUT);
        Index_Type n_biases = get_length(Neuron_Type::BIAS);
        Index_Type n_outputs = get_length(Neuron_Type::OUTPUT);
        Index_Type n_hiddens = get_length(Neuron_Type::HIDDEN);

        Index_Type from_input = n_inputs * (n_hiddens + n_outputs);
        Index_Type from_bias = n_biases * (n_hiddens + n_outputs);
        Index_Type from_hidden = n_hiddens * (n_hiddens + n_outputs); // Can self-loop
        Index_Type from_output = n_outputs * (n_hiddens); // output can go to hidden
        Index_Type total = from_input + from_bias + from_hidden + from_output;
        return total;
    }
    // PhenotypeTemplate(const NewNeurons& neurons, const NewLinks& links)
    //PhenotypeTemplate(TypeEndArray& type_end, const bool synchronize = false)
    //    : graph(), io_sync(synchronize), type_end(type_end), inputs_out(), outputs_out(),
    //      edge_idx_2_innov_num() {
    //    TypeEndArray type_length;
    //    get_type_length_arr(type_length);
    //    // Add basic inputs, outputs, and bias.
    //    graph.add_vertices(type_length);
    //    if (synchronize) {
    //        graph.input_addresses(inputs_out);
    //        graph.output_addresses(outputs_out);
    //    }
    //}
    //// Notice, the two given two indices are from graph_node, and not the Akashic records.
    //// This method would require the genotype to build a sort of lookup map to pass in
    //// the correct indices. Also, might want to store this, just in case the neural network
    //// sneakily expands in side and messes up with
    //template <bool add_to_graph = true>
    //NewLink add_link(const Index_Type graph_node_from_idx, const Index_Type graph_node_to_idx,
    //                 const FP_Type weight = static_cast<FP_Type>(0)) {
    //    NewLink edge_create(graph_node_from_idx, graph_node_to_idx, weight);
    //    if (add_to_graph) {
    //        edge_create.apply_vertex_list(graph.)
    //    }
    //    return edge_create;
    //}


};

template <typename FP_Type = double>
using Phenotype = PhenotypeMetaTemplate<FP_Type>;
template <typename FP_Type = double>
using PhenotypeInstance = PhenotypeTemplate<FP_Type>;

} // namespace NEAT