#include "neural_network_core.hpp"
#pragma once
#include "neural_network_core.hpp"

using namespace NeuralNetwork;
template <typename FP_Type //, typename Neuron_Enum
          >
void Graph<FP_Type //,Neuron_Enum
           >::Edge::append_str(std::string& str2append) const {
    str2append.append(">==>==> ")
        .append(Common::ryu_d2s(static_cast<double>(weight)))
        .append(" >==>==>");
}
template <typename FP_Type //, typename Neuron_Enum
          >
void Graph<FP_Type //, Neuron_Enum
           >::Vertex::append_str(std::string& str2append) const {
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
template <typename FP_Type>
void Graph<FP_Type>::calculate_output(const int& vert_idx, FP_Type& output) {
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
template <typename FP_Type>
template <bool type_count_normalized>
void NeuralNetwork::Graph<FP_Type>::rearrange_knew_counts(VertexList& vertices,
                                                          std::vector<int>& type_count) {
    CAN_BE_OPTIMIZED_MSG("Instead of doing too many swappings, generate a swap plan" 
        "then swap arrows only for necessary vertices.");
    CAN_BE_OPTIMIZED_MSG("Or we can just replace VertexList to std::vector<Vertex*> "
    "where each of its element is pointer to the actual VertexList& "
    "passed into ctor, then reassign on that structure. We call that structure "
    "vertices_for_calculating.");
    if (!type_count_normalized) {
        int pos = 0;
        for (int i = 0; i < type_count.size(); i++) {
            //POTENTIAL_BUG_POINT_MSG("Might be bugged out because of optimization.");
            // assigns counts[i] as pos
            // adds pos by the previous value of counts[i]
            // Original (Python):
            // temp = counts[i]
            // counts[i] = pos
            // pos += temp
            pos += type_count[i];
            type_count[i] = pos - type_count[i];
        }
    }
    // Python code by myself (bucket swap sort is a name I came up with):
    /*    def bucket_swap_sort(unsorted, pos):
    //    num_elements_sorted = 0
    //    for i in range(len(unsorted)):
    //        print(">>i = {}".format(i))
    //        print(">>unsorted: {}".format(unsorted))
    //        print(">>pos: {}".format(pos))
    //        while pos[unsorted[i]] != i:
    //            if num_elements_sorted == len(unsorted):
    //                break
    //            swap_index = pos[unsorted[i]]
    //            pos[unsorted[i]] += 1
    //#swap
    //            print("swap({}): Swapping index {} to index {}".format(num_elements_sorted, i,
    swap_index))
    //            (unsorted[swap_index], unsorted[i]) = (unsorted[i], unsorted[swap_index])
    //            num_elements_sorted += 1
    //            print("<<unsorted: {}".format(unsorted))
    //            print("<<pos: {}".format(pos))
    //        if num_elements_sorted == len(unsorted):
    //            break
    //        pos[unsorted[i]] += 1
    //        num_elements_sorted += 1
    //    print("Done sorting: {}".format(unsorted))
    //    print("Total swap: {}".format(num_elements_sorted))
    */
    int num_sorted = 0;
    for (int i = 0; i < vertices.size(); i++) {
        // Iterate once through the given vector
        unsigned char type = static_cast<unsigned char>(_V_LIST_FIELD_(vertices[i], neuron_type));
        while (type_count[type] != i) {
            // This location is not desirable.
            // std::swap(vertices[i], vertices[type_count[type]]);
            swap_vertex_index_position(vertices, i, type_count[type]);
            type_count[type] += 1;
            num_sorted += 1;
            if (num_sorted == vertices.size()) {
                // Done
                goto done_sorting;
            }
            type = static_cast<unsigned char>(_V_LIST_FIELD_(vertices[i], neuron_type));
        }
        type_count[type] += 1;
        num_sorted += 1;
        if (num_sorted == vertices.size()) {
            break;
        }
    }
done_sorting:
    return;
}
template <typename FP_Type>
template <typename index_type>
inline void NeuralNetwork::Graph<FP_Type>::swap_arrows_vertex_index(VertexList& vertices, index_type getter,
                                                                    index_type swapped_index) {
    Graph<FP_Type>::Vertex::ArrowList& in_arrows = _V_LIST_FIELD_(vertices[getter], inwards_arrows);
    for (index_type i = 0; i < in_arrows.size(); i++) {
        _A_LIST_FIELD_(in_arrows[i], out) = &vertices[swapped_index];
    }
    Graph<FP_Type>::Vertex::ArrowList& out_arrows = _V_LIST_FIELD_(vertices[getter], outwards_arrows);
    for (index_type i = 0; i < out_arrows.size(); i++) {
        _A_LIST_FIELD_(out_arrows[i], in) = &vertices[swapped_index];
    }
}
template <typename FP_Type>
template <typename index_type, bool swap_arrows>
inline void NeuralNetwork::Graph<FP_Type>::swap_vertex_index_position(VertexList& vertices, 
    index_type idx0,index_type idx1) {
    if (swap_arrows) {
        swap_arrows_vertex_index(vertices, idx0, idx1);
        swap_arrows_vertex_index(vertices, idx1, idx0);
    }
    std::swap(vertices[idx0], vertices[idx1]);
}
template <typename FP_Type>
template <bool extract_output, bool parse_from_input>
void Graph<FP_Type>::calculate_outputs(const std::vector<FP_Type>& inputs,
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
template <typename FP_Type>
template <typename index_type>
inline void NeuralNetwork::Graph<FP_Type>::EdgeCreate<index_type>::apply_vertex_list(
    VertexList& v_list, std::vector<Edge>& all_edges) const {
    CAN_BE_OPTIMIZED_MSG("This should be optimized to adding to the exact place in all_edges");
    CAN_BE_OPTIMIZED_MSG("So that it can be paralellized.");
    // The following code couldn't be used because edge destructs itself upon
    // being out of scope.
    // Graph<FP_Type>::Edge edge(&v_list[in_idx], &v_list[out_idx], weight, is_recurrent);
    // all_edges.push_back(edge);
    //_V_LIST_FIELD_(v_list[in_idx], outwards_arrows).push_back(&edge);
    //_V_LIST_FIELD_(v_list[out_idx], inwards_arrows).push_back(&edge);

    // The following code might not be able to be made paralellized
    // Might be made parallelized,
    // hint: http://www.cplusplus.com/reference/vector/vector/emplace/
    all_edges.emplace_back(&v_list[in_idx], &v_list[out_idx], weight, is_recurrent);
    Graph<FP_Type>::Edge* ptr = &all_edges.back();
    _V_LIST_FIELD_(v_list[in_idx], outwards_arrows).push_back(ptr);
    _V_LIST_FIELD_(v_list[out_idx], inwards_arrows).push_back(ptr);
}
template <typename FP_Type>
template <typename index_type>
void Graph<FP_Type>::connect_vertices(VertexList& vertices,
                                      const std::vector<EdgeCreate<index_type>>& instructions,
                                      std::vector<Edge>& all_edges) {
    all_edges.reserve(instructions.size());
    CAN_BE_OPTIMIZED_MSG("Can possibly be made paralellized.");
    for (int i = 0; i < instructions.size(); i++) {
        instructions[i].apply_vertex_list(vertices, all_edges);
    }
}
template <typename FP_Type>
void NeuralNetwork::Graph<FP_Type>::rearrange(VertexList& vertices) {
    // Count the number of occurances, then call rearrange_knew_count.
    std::vector<int> count;
    int max = -1;
    bool need_sort = false;
    for (int i = 0; i < vertices.size(); i++) {
        unsigned char type = static_cast<unsigned char>(_V_LIST_FIELD_(vertices[i], neuron_type));
        // Initializes count so that it's enough to store @var type.
        for (; max < type; max++) {
            count.push_back(0);
        }
        count[type] += 1;
        if (type < max) {
            // This would only happen if the value reading from left to right
            // suddenly fall out of order.
            need_sort = true;
        }
    }
    if (need_sort) {
#ifndef NEURAL_NDEBUG
        std::cout << "rearrange calls rearrange_knew_counts()" << std::endl;
#endif
        rearrange_knew_counts(vertices, count);
    }
}

template <typename FP_Type>
void Graph<FP_Type>::append_str(std::string& str2append) const {
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

template <typename T>
std::string NeuralNetwork::to_string(const T& has_append_string) {
    std::string s;
    has_append_string.append_str(s);
    return s;
}