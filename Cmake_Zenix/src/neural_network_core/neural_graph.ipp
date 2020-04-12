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
    if (!type_count_normalized) {
        int pos = 0;
        for (int i = 0; i < type_count.size(); i++) {
            POTENTIAL_BUG_POINT_MSG("Might be bugged out because of optimization.");
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
        unsigned char type = static_cast<unsigned char>(_V_LIST_FIELD_(vertices[i], neuron_type));
        while (type_count[type] != i) {
            // int swap_index = type_count[type];
            // type_count[type] += 1;
            // std::swap(vertices[i], vertices[swap_index]);
            // num_sorted += 1;
            std::swap(vertices[i], vertices[type_count[type]]);
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
            std::cout << "Index " << vert_idx << " Outputs: "
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
        // calculate the output of this vertex and put it to last_output
        // of this vertex.
        calculate_output(vert_idx, _GET_FIELD_AT_INDEX_(last_output, vert_idx));
#ifndef NEURAL_NDEBUG
        std::cout << "Index " << vert_idx << " Outputs: "
            << Common::ryu_d2s(_GET_FIELD_AT_INDEX_(last_output, vert_idx))
            << std::endl;
#endif
            // if we want to extract output and the current vertex is the output node,
            // append this output to @param outputs.
            if (extract_output &&
                _GET_FIELD_AT_INDEX_(neuron_type, vert_idx) == NeuronType::OUTPUT) {
            // append last_output of this output vertex to @param output.
            outputs.push_back(_GET_FIELD_AT_INDEX_(last_output, vert_idx));
        }
    }
}

template <typename FP_Type>
void NeuralNetwork::Graph<FP_Type>::rearrange(VertexList& vertices) {
    // Count the number of occurances, then call rearrange_knew_count.
    std::vector<int> count;
    int max = -1;
    for (int i = 0; i < vertices.size(); i++) {
        unsigned char type = static_cast<unsigned char>(_V_LIST_FIELD_(vertices[i], neuron_type));
        // Initializes count so that it's enough to store @var type.
        for (; max < type; max++) {
            count.push_back(0);
        }
        count[type] += 1;
    }
    rearrange_knew_counts(vertices, count);
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