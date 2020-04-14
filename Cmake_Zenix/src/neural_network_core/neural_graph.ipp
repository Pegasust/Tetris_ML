#include "neural_network_core.hpp"
#pragma once
#include "neural_network_core.hpp"

using namespace NeuralNetwork;
template <typename FP_Type, typename NeuronType, typename index_type>
void Graph<FP_Type, NeuronType //,Neuron_Enum
           ,
           index_type>::Edge::append_str(std::string& str2append) const {
    str2append.append(">==>==> ")
        .append(Common::ryu_d2s(static_cast<double>(weight)))
        .append(" >==>==>");
}
template <typename FP_Type, typename NeuronType, typename index_type>
void Graph<FP_Type, NeuronType //, Neuron_Enum
           ,
           index_type>::Vertex::append_str(std::string& str2append) const {
    static char buffer[8];
    neuron_type_2_cstr(neuron_type, buffer);
    str2append.append("[a(")
        .append(Common::ryu_d2s(static_cast<double>(activation_response)))
        .append(") ")
        .append(buffer)
        .append(" o(")
        .append(Common::ryu_d2s(static_cast<double>(last_output)))
        .append(")]");
}
template <typename FP_Type, typename NeuronType, typename index_type>
inline NeuralNetwork::Graph<FP_Type, NeuronType, index_type>::Graph(
    const std::vector<FP_Type>& vertices_data, index_type vertex_type_count[],
    const std::vector<EdgeCreate>& instructions, int graph_depth)
    : graph_depth(graph_depth) {
    add_vertices(vertices_data, vertex_type_count);
    connect_self_vertices(instructions, edges);
}
template <typename FP_Type, typename NeuronType, typename index_type>
void Graph<FP_Type, NeuronType, index_type>::calculate_output(const int& vert_idx,
                                                              FP_Type& output) {
    // It is important not to set output to 0.0 then use it as sum, as
    // it can cause conflicts with LSTM (Long short term mem) for self-loops.
    FP_Type sum = 0.0;
    const Vertex::ArrowList& in_arrows = _GET_FIELD_AT_INDEX_(inwards_arrows, vert_idx);
    //#define in_arrows _GET_FIELD_AT_INDEX_(inwards_arrows, vert_idx)
    for (int arrow_idx = 0; arrow_idx < in_arrows.size(); arrow_idx++) {
        // weighted output = output of vertex of the incoming arrow * weight of
        // incoming arrow
        // The output of the vertex of the incoming arrow.
        const FP_Type& last_out = _A_LIST_FIELD_(in_arrows[arrow_idx], in->last_output);
        // The weight of this inward arrow
        const FP_Type& arrow_weight = _A_LIST_FIELD_(in_arrows[arrow_idx], weight);
        // Add this to sum
        sum += last_out * arrow_weight;
#ifndef NEURAL_NDEBUG
        std::cout << "Accessing "
                  << "vert_idx: " << vert_idx << " arrow_idx: " << arrow_idx << " Vertex: "
                  << NeuralNetwork::to_string(*_A_LIST_FIELD_(in_arrows[arrow_idx], in))
                  << " last_out: " << Common::ryu_d2s(last_out)
                  << " arrow_weight: " << Common::ryu_d2s(arrow_weight)
                  << "\nSum is now: " << Common::ryu_d2s(sum) << std::endl;

#endif
    }
    // Apply the sigmoid function
    const FP_Type& response = _GET_FIELD_AT_INDEX_(activation_response, vert_idx);
    output = Common::ZMath::sigmoid(sum, response);
#ifndef NEURAL_NDEBUG
    std::cout << "Index " << vert_idx << ": Calculate_output outputs: " << Common::ryu_d2s(output)
              << std::endl;
#endif
    //#undef in_arrows
}
namespace {
template <typename index_type = int>
inline void transform_to_pos(std::vector<index_type>& count) {
    for (index_type i = 1; i < count.size(); i++) {
        count[i] += count[i - 1];
    }
}
template <typename index_type = int>
inline index_type sorted(index_type index) {
    index_type retval = index >> (sizeof(index_type) * 8) - 1;
    return retval;
}
template <typename index_type = int>
inline void mark_sorted(std::vector<index_type>& lookup, index_type index) {
    lookup[index] = ~lookup[index];
}
template <typename index_type = int>
inline void unmark(std::vector<index_type>& lookup, index_type index) {
    lookup[index] = ~lookup[index];
}
} // namespace
template <typename FP_Type, typename NeuronType, typename index_type>
template <bool type_count_normalized>
void NeuralNetwork::Graph<FP_Type, NeuronType, index_type>::rearrange_knew_counts(
    VertexList& vertices, std::vector<index_type>& type_count, std::vector<index_type>& lookup) {
    CAN_BE_OPTIMIZED_MSG("Instead of doing too many swappings, generate a swap plan"
                         "then swap arrows only for necessary vertices.");

    CAN_BE_OPTIMIZED_MSG("This sorting alg might swap position eventhough it's in a"
                         "desirable location, although not the latest.");

    CAN_BE_OPTIMIZED_MSG(
        "Or we can just replace VertexList to std::vector<Vertex*> "
        "where each of its element is pointer to the actual VertexList& "
        "passed into ctor, then reassign on that structure. We call that structure "
        "vertices_for_calculating.");
    if (!type_count_normalized) {
        transform_to_pos(type_count);
    }
    ASSERT(lookup.size() == vertices.size(), "lookup is not initialized to [0, vertices.size())");
    //    index_type num_sorted = 0;
    //    for (index_type i = 0; i < vertices.size(); i++) {
    //        // Iterate once through the given vector
    //        unsigned char type = static_cast<unsigned char>(_V_LIST_FIELD_(vertices[i],
    //        neuron_type)); while (type_count[type] != i) {
    //            // This location is not desirable.
    //            // std::swap(vertices[i], vertices[type_count[type]]);
    //            swap_vertex_index_position(vertices, i, type_count[type]);
    //            type_count[type] += 1;
    //            num_sorted += 1;
    //            if (num_sorted == vertices.size()) {
    //                // Done
    //                goto done_sorting;
    //            }
    //            type = static_cast<unsigned char>(_V_LIST_FIELD_(vertices[i], neuron_type));
    //        }
    //        type_count[type] += 1;
    //        num_sorted += 1;
    //        if (num_sorted == vertices.size()) {
    //            break;
    //        }
    //    }
    // done_sorting:
    //    return;
    for (index_type i = vertices.size(); i >= 1;) {
        i -= 1;
        if (sorted(lookup[i])) {
            continue;
        }
#ifndef NEURAL_NDEBUG
        std::cout << "rearrange_knew_count sorting at " << i << std::endl;
#endif
        mark_sorted(lookup, i);
        type_count[_V_LIST_FIELD_(vertices[i], neuron_type)] -= 1;
        while (i > type_count[_V_LIST_FIELD_(vertices[i], neuron_type)]) {
            index_type& swap_index = type_count[_V_LIST_FIELD_(vertices[i], neuron_type)];
            mark_sorted(lookup, swap_index);
            if (_V_LIST_FIELD_(vertices[swap_index], neuron_type) !=
                _V_LIST_FIELD_(vertices[i], neuron_type)) {
                std::swap(lookup[i], lookup[swap_index]);
                std::swap(vertices[swap_index], vertices[i]);
            }
            type_count[_V_LIST_FIELD_(vertices[i], neuron_type)] -= 1;
        }
    }
    for (index_type i = 0; i < lookup.size(); i++) {
        unmark(lookup, i);
    }
}
template <typename FP_Type, typename NeuronType, typename index_type>
inline void NeuralNetwork::Graph<FP_Type, NeuronType, index_type>::swap_arrows_vertex_index(
    VertexList& vertices, index_type getter, index_type swapped_index) {
    // It now does something if the vertices were swapped before hand.
    // ASSERT(getter != swapped_index,
    //       "Attempting to swap arrows on same indices, which does nothing, "
    //       "but costs so much computational power.");
    Graph<FP_Type, NeuronType>::Vertex::ArrowList& in_arrows =
        _V_LIST_FIELD_(vertices[getter], inwards_arrows);
    for (index_type i = 0; i < in_arrows.size(); i++) {
        _A_LIST_FIELD_(in_arrows[i], out) = &vertices[swapped_index];
    }
    Graph<FP_Type, NeuronType>::Vertex::ArrowList& out_arrows =
        _V_LIST_FIELD_(vertices[getter], outwards_arrows);
    for (index_type i = 0; i < out_arrows.size(); i++) {
        _A_LIST_FIELD_(out_arrows[i], in) = &vertices[swapped_index];
    }
}
template <typename FP_Type, typename NeuronType, typename index_type>
template <bool swap_arrows>
inline void NeuralNetwork::Graph<FP_Type, NeuronType, index_type>::swap_vertex_index_position(
    VertexList& vertices, index_type idx0, index_type idx1) {
    ASSERT(idx0 != idx1, "Attempting to swap index position on same index, which does nothing, "
                         "but costs so much computational power.");
    if (swap_arrows) {
        swap_arrows_vertex_index(vertices, idx0, idx1);
        swap_arrows_vertex_index(vertices, idx1, idx0);
    }
    std::swap(vertices[idx0], vertices[idx1]);
}
template <typename FP_Type, typename NeuronType, typename index_type>
template <bool extract_output, bool parse_from_input>
void Graph<FP_Type, NeuronType, index_type>::calculate_outputs(const std::vector<FP_Type>& inputs,
                                                               std::vector<FP_Type>& outputs) {
    // For input vertices, just copy @param inputs to these nodes sequentially.
    int vert_idx = 0; // vertex index in this->vertices.

    if (parse_from_input && inputs.size() > 0) {
        while (_GET_FIELD_AT_INDEX_(neuron_type, vert_idx) == NeuronType::INPUT) {
            // assign output to current vertex in this->vertices
            _GET_FIELD_AT_INDEX_(last_output, vert_idx) = inputs[vert_idx];
#ifndef NEURAL_NDEBUG
            std::cout << "Index " << vert_idx << "(INPUT_NODE) Outputs: "
                      << Common::ryu_d2s(_GET_FIELD_AT_INDEX_(last_output, vert_idx)) << std::endl;
#endif
            ++vert_idx;
        }
    }
    while (_GET_FIELD_AT_INDEX_(neuron_type, vert_idx) == NeuronType::BIAS) {
        // Assign output to const 1. The actual "bias" that we need to
        // tune is the Edge/Link's weight.
        CAN_BE_OPTIMIZED_MSG("We can just set this once and not do this again");
        _GET_FIELD_AT_INDEX_(last_output, vert_idx) = 1;
        ++vert_idx;
    }
    // In the case of HIDDEN and OUTPUT:
    for (; vert_idx < vertices.size(); vert_idx++) {
#ifndef NEURAL_NDEBUG
        std::cout << "Calculating HIDDEN/OUTPUT Node index " << vert_idx << std::endl;
#endif
        // calculate the output of this vertex and put it to last_output
        // of this vertex.
        calculate_output(vert_idx, _GET_FIELD_AT_INDEX_(last_output, vert_idx));
#ifndef NEURAL_NDEBUG
        std::cout << "Index " << vert_idx
                  << " Outputs: " << Common::ryu_d2s(_GET_FIELD_AT_INDEX_(last_output, vert_idx))
                  << std::endl;
#endif
        // if we want to extract output and the current vertex is the output node,
        // append this output to @param outputs.
        if (extract_output && _GET_FIELD_AT_INDEX_(neuron_type, vert_idx) == NeuronType::OUTPUT) {
            // append last_output of this output vertex to @param output.
            outputs.push_back(_GET_FIELD_AT_INDEX_(last_output, vert_idx));
        }
    }
}
template <typename FP_Type, typename NeuronType, typename index_type>
inline void NeuralNetwork::Graph<FP_Type, NeuronType, index_type>::create_link(
    VertexList& v_list, std::vector<Edge>& out_edge, index_type in_idx, index_type out_idx,
    FP_Type weight, bool recurrent) {
    CAN_BE_OPTIMIZED_MSG("This should be optimized to adding to the exact place in all_edges");
    CAN_BE_OPTIMIZED_MSG("So that it can be paralellized.");
    // The following code couldn't be used because edge destructs itself upon
    // being out of scope.
    // Graph<FP_Type, NeuronType>::Edge edge(&v_list[in_idx], &v_list[out_idx], weight,
    // is_recurrent); all_edges.push_back(edge);
    //_V_LIST_FIELD_(v_list[in_idx], outwards_arrows).push_back(&edge);
    //_V_LIST_FIELD_(v_list[out_idx], inwards_arrows).push_back(&edge);

    // The following code might not be able to be made paralellized
    // Might be made parallelized,
    // hint: http://www.cplusplus.com/reference/vector/vector/emplace/
    out_edge.emplace_back(&v_list[in_idx], &v_list[out_idx], weight, recurrent);
    Graph<FP_Type, NeuronType>::Edge* ptr = &out_edge.back();
    _V_LIST_FIELD_(v_list[in_idx], outwards_arrows).push_back(ptr);
    _V_LIST_FIELD_(v_list[out_idx], inwards_arrows).push_back(ptr);
}
template <typename FP_Type, typename NeuronType, typename index_type>
void NeuralNetwork::Graph<FP_Type, NeuronType, index_type>::init(
    const std::vector<FP_Type>& vertices_data, const std::vector<NeuronType>& types,
    std::vector<index_type>& vertex_type_count, const std::vector<EdgeCreate>& instructions,
    int graph_depth) {
    this->graph_depth = graph_depth;
    ASSERT(vertices_data.size() == types.size() * 2, "vertices_data and types size mismatch!");
    // Add all vertices
    for (index_type i = 0; i < types.size(); i++) {
        vertices.emplace_back(vertices_data[i * 2], vertices_data[(i * 2) + 1], types[i]);
    }
    std::vector<index_type> lookup;
    rearrange_knew_counts(this->vertices, vertex_type_count, lookup);
    connect_vertices(this->vertices, instructions, this->edges, lookup);
}
template <typename FP_Type, typename NeuronType, typename index_type>
inline void NeuralNetwork::Graph<FP_Type, NeuronType, index_type>::EdgeCreate::apply_vertex_list(
    VertexList& v_list, std::vector<Edge>& all_edges, const std::vector<index_type>& lookup) const {
    create_link(v_list, all_edges, lookup[in_idx], lookup[out_idx], weight, is_recurrent);
}
template <typename FP_Type, typename NeuronType, typename index_type>
inline void NeuralNetwork::Graph<FP_Type, NeuronType, index_type>::add_vertices(
    const std::vector<FP_Type>& data_response_initial_output, index_type inputs, index_type biases,
    index_type hiddens, index_type outputs) {
    index_type arr[] = {inputs, biases, hiddens, outputs};
    add_vertices<index_type, NeuronType>(data_response_initial_output, arr);
}
template <typename FP_Type, typename NeuronType, typename index_type>
inline void NeuralNetwork::Graph<FP_Type, NeuronType, index_type>::add_to_v_list(
    const VertexCreate& vc, VertexList& v_list) {
    v_list.emplace_back(vc.neuron_type, vc.activation_response, vc.initial_output);
    //Vertex v(vc.neuron_type, vc.activation_response, vc.initial_output);
    //v_list.push_back(std::move(v));
}
template <typename FP_Type, typename NeuronType, typename index_type>
inline void NeuralNetwork::Graph<FP_Type, NeuronType, index_type>::init(
    const std::vector<VertexCreate>& vertices, const std::vector<EdgeCreate>& instructions,
    std::vector<index_type> vertex_type_count, int graph_depth) {
    this->graph_depth = graph_depth;
    for (index_type i = 0; i < size(); i++) {
        add_to_v_list(vertices[i], this->vertices);
    }
    std::vector<index_type> lookup;
    rearrange_knew_counts(this->vertices, vertex_type_count, lookup);
    connect_vertices(this->vertices, instructions, this->edges, lookup);
}
template <typename FP_Type, typename NeuronType, typename index_type>
inline NeuralNetwork::Graph<FP_Type, NeuronType, index_type>::Graph(
    const std::vector<VertexCreate>& vertices, const std::vector<EdgeCreate>& instructions,
    int graph_depth)
    : graph_depth(graph_depth) {
    for (index_type i = 0; i < vertices.size(); i++) {
        add_to_v_list(vertices[i], this->vertices);
    }
    std::vector<index_type> lookup;
    rearrange_self(lookup);
    connect_vertices(this->vertices, instructions, this->edges, lookup);
}
template <typename FP_Type, typename NeuronType, typename index_type>
template <typename Neuron_Type>
inline void NeuralNetwork::Graph<FP_Type, NeuronType, index_type>::add_vertices(
    const std::vector<FP_Type>& data_response_initial_output, index_type type_count[]) {
    Common::check(vertices.capacity() >= (vertices.size() + data_response.size() / 2),
                  "Not enough capacity to hold the number of vertices being add. Unless "
                  "links are created after reassignment, consider reserving a larger value.");

    // for (index_type i = 0; i < data_resposne_initial_output.size(); i += 2) {
    //    vertices.emplace_back()
    //}
    index_type i = 0;
    for (Neuron_Type type = 0; type <= Neuron_Type::OUTPUT; type++) {
        for (index_type count = 0;
             count < type_count[type], i < data_response_initial_output.size(); count++, i += 2) {
            vertices.emplace_back(data_response_initial_output[i],
                                  data_response_initial_output[i + 1],
                                  static_cast<NeuronType>(type));
        }
    }
    for (; i < data_response_initial_output.size(); i += 2) {
        // type is now in its undefined state because of type++ at the end.
        vertices.emplace_back(data_response_initial_output[i], data_response_initial_output[i + 1],
                              static_cast<NeuronType>(type));
    }
}
template <typename FP_Type, typename NeuronType, typename index_type>
inline void NeuralNetwork::Graph<FP_Type, NeuronType, index_type>::EdgeCreate::apply_vertex_list(
    VertexList& v_list, std::vector<Edge>& all_edges) const {
    create_link(v_list, all_edges, in_idx, out_idx, weight, is_recurrent);
}

template <typename FP_Type, typename NeuronType, typename index_type>
void Graph<FP_Type, NeuronType, index_type>::connect_vertices(
    VertexList& vertices, const std::vector<EdgeCreate>& instructions,
    std::vector<Edge>& all_edges) {
    all_edges.reserve(instructions.size());
    CAN_BE_OPTIMIZED_MSG("Can possibly be made paralellized.");
    for (int i = 0; i < instructions.size(); i++) {
        instructions[i].apply_vertex_list(vertices, all_edges);
    }
}
template <typename FP_Type, typename NeuronType, typename index_type>
inline void NeuralNetwork::Graph<FP_Type, NeuronType, index_type>::connect_vertices(
    VertexList& vertices, const std::vector<EdgeCreate>& instructions, std::vector<Edge>& all_edges,
    std::vector<index_type> lookup) {
    all_edges.reserve(instructions.size());
    CAN_BE_OPTIMIZED_MSG("Can possibly be made paralellized.");
    for (int i = 0; i < instructions.size(); i++) {
        instructions[i].apply_vertex_list(vertices, all_edges,lookup);
    }
}
template <typename FP_Type, typename NeuronType, typename index_type>
void NeuralNetwork::Graph<FP_Type, NeuronType, index_type>::rearrange(
    VertexList& vertices, std::vector<index_type>& lookup) {
    // Count the number of occurances, then call rearrange_knew_count.
    std::vector<index_type> count;
    bool need_sort;
    count_type(vertices, count, need_sort);
    ASSERT(lookup.size() == 0, "lookup is not empty.");
    for (index_type i = 0; i < vertices.size(); i++) {
        lookup.emplace_back(i);
    }
    if (need_sort) {
#ifndef NEURAL_NDEBUG
        std::cout << "rearrange() calls rearrange_knew_counts()" << std::endl;
#endif
        rearrange_knew_counts(vertices, count, lookup);
    }
}

template <typename FP_Type, typename NeuronType, typename index_type>
void Graph<FP_Type, NeuronType, index_type>::append_str(std::string& str2append) const {
#ifdef RELEASE_DEFINED
    int vert_idx = 0; // vertex index in this->vertices.
    UNIMPLEMENTED_FEATURE_MSG("Might need to go from outputs and recurse back.");
    for (; vert_idx < vertices.size(); vert_idx++) {
        str2append.append("\n");
    }
#else
    str2append.append("Neurons:\n");
    for (int i = 0; i < vertices.size(); i++) {
        _V_LIST_FIELD_(vertices[i], append_str)(str2append);
        str2append.append("  ");
    }
    str2append.append("\nCalculation path:\n");
    // For input vertices, just copy @param inputs to these nodes sequentially.
    int vert_idx = 0; // vertex index in this->vertices.
    str2append.append("Parsing inputs:\n");

    while (_GET_FIELD_AT_INDEX_(neuron_type, vert_idx) == NeuronType::INPUT) {
        // assign output to current vertex in this->vertices
        _GET_FIELD_AT_INDEX_(append_str, vert_idx)(str2append);
        ++vert_idx;
    }
    str2append.append("\nBiases:\n");
    while (_GET_FIELD_AT_INDEX_(neuron_type, vert_idx) == NeuronType::BIAS) {
        // Assign output to const 1. The actual "bias" that we need to
        // tune is the Edge/Link's weight.
        CAN_BE_OPTIMIZED_MSG("We can just set this once and not do this again");
        _GET_FIELD_AT_INDEX_(append_str, vert_idx)(str2append);
        ++vert_idx;
    }
    str2append.append("Hidden & Output:\n");
    // In the case of HIDDEN and OUTPUT:
    for (; vert_idx < vertices.size(); vert_idx++) {
        // calculate the output of this vertex and put it to last_output
        // of this vertex.
        str2append.append("Neuron: ");
        _GET_FIELD_AT_INDEX_(append_str, vert_idx)(str2append);
        str2append.append("\nCalculation:\n ");
        const Vertex::ArrowList& in_arrows = _GET_FIELD_AT_INDEX_(inwards_arrows, vert_idx);
        for (int arrow_idx = 0; arrow_idx < in_arrows.size(); arrow_idx++) {
            // weighted output = output of vertex of the incoming arrow * weight of
            // incoming arrow
            // The output of the vertex of the incoming arrow.
            _A_LIST_FIELD_(in_arrows[arrow_idx], in->append_str)(str2append);
            str2append.append(" ");
            // The weight of this inward arrow
            _A_LIST_FIELD_(in_arrows[arrow_idx], append_str)(str2append);
            // Add this to sum
            str2append.append("\n");
        }
        // if we want to extract output and the current vertex is the output node,
        // append this output to @param outputs.
        if (_GET_FIELD_AT_INDEX_(neuron_type, vert_idx) == NeuronType::OUTPUT) {
            str2append.append("<OUTPUT>: ");
            str2append.append(Common::ryu_d2s(_GET_FIELD_AT_INDEX_(last_output, vert_idx)));
            str2append.append("\n");
        }
    }

#endif
}

template <typename FP_Type, typename NeuronType, typename index_type>
inline void NeuralNetwork::Graph<FP_Type, NeuronType, index_type>::count_type(
    const VertexList& vertices, std::vector<int>& out_count, bool& out_need_sort) {
    int max = -1;
    out_need_sort = false;
    for (int i = 0; i < vertices.size(); i++) {
        unsigned char type = static_cast<unsigned char>(_V_LIST_FIELD_(vertices[i], neuron_type));
        // Initializes count so that it's enough to store @var type.
        for (; max < type; max++) {
            out_count.push_back(0);
        }
        out_count[type] += 1;
        if (type < max) {
            // This would only happen if the value reading from left to right
            // suddenly fall out of order.
            out_need_sort = true;
        }
    }
}

template <typename T>
std::string NeuralNetwork::to_string(const T& has_append_string) {
    std::string s;
    has_append_string.append_str(s);
    return s;
}