#pragma once
#include "artfl_neural_network.hpp"
namespace NeuralNetwork {
template <typename FP_Type, typename NeuronType, typename Index_Type>
void VolatileGraph<FP_Type, NeuronType, Index_Type>::Edge::append_str(
    std::string& str2append) const {
    // {
    str2append.append(">==>==> ")
        .append(Common::ryu_d2s(static_cast<double>(weight)))
        .append(" >==>==>");
}
template <typename FP_Type, typename NeuronType, typename index_type>
void VolatileGraph<FP_Type, NeuronType //, Neuron_Enum
                   ,
                   index_type>::Vertex::append_str(std::string& str2append) const {
    static char buffer[8];
    neuron_type_2_cstr(neuron_type, buffer);
    str2append.append("[a(")
        .append(Common::ryu_d2s(static_cast<double>(response)))
        .append(") ")
        .append(buffer)
        .append(" o(")
        .append(Common::ryu_d2s(static_cast<double>(last_output)))
        .append(")]");
}
template <typename FP_Type, typename Neuron_Type, typename Index_Type>
template <typename Vect_Resp, typename Vect_Out, bool reserve, bool has_resp, bool has_ini_out>
void VolatileGraph<FP_Type, Neuron_Type, Index_Type>::make_vertices(
    const TypeEndArray& type_end, const Vect_Resp& data_response, const Vect_Out& data_ini_out,
    const FP_Type default_activation_response, const FP_Type default_initial_output) {
    // {
    FP_Type act_resp, ini_outp;
    if (!has_resp) {
        act_resp = default_activation_response;
    }
    if (!has_ini_out) {
        ini_outp = default_initial_output;
    }
    if (reserve) {
        // Reserve to store enough neurons in this add
        this->vertices.reserve(this->vertices_count() + type_end[type_end.size() - 1]);
    }
    Index_Type i = 0;
    for (Index_Type type = 0; type < type_end.size(); type++) {
        // By iterating through from 0 to undefined, we are guaranteed to push all
        // types contiguously and ordered.
        for (; i < type_end[type]; i++) {
            if (has_resp) {
                act_resp =
                    (i < data_response.size() ? data_response[i] : default_activation_response);
            }
            if (has_ini_out) {
                ini_outp = (i < data_ini_out.size() ? data_ini_out[i] : default_initial_output);
            }
            // const FP_Type act_resp = (has_resp && i < data_response.size())
            //                             ? data_response[i]
            //                             : default_activation_response;
            // const FP_Type ini_outp =
            //    (has_ini_out && i < data_ini_out.size()) ? data_ini_out[i] :
            //    default_initial_output;
            make_vertex(NT_(type), act_resp, ini_outp);
        }
    }
}

template <typename FP_Type, typename Neuron_Type, typename Index_Type>
Index_Type VolatileGraph<FP_Type, Neuron_Type, Index_Type>::disable_edge(Edge& edge) {
    Index_Type enabled = !edge.disabled();
    if (enabled) {
        edge.toggle_disabled();
    }
    return 1;
}

template <typename FP_Type, typename Neuron_Type, typename Index_Type>
Index_Type VolatileGraph<FP_Type, Neuron_Type, Index_Type>::enable_edge(Edge& edge) {
    Index_Type disabled = edge.disabled();
    if (disabled) {
        edge.toggle_disabled();
    }
    return 1;
}

template <typename FP_Type, typename Neuron_Type, typename Index_Type>
bool VolatileGraph<FP_Type, Neuron_Type, Index_Type>::toggle_edge(Edge& edge) {
    return edge.toggle_disabled();
}

template <typename FP_Type, typename Neuron_Type, typename Index_Type>
void VolatileGraph<FP_Type, Neuron_Type, Index_Type>::clear_vertex_outputs(const bool set_bias_1) {
    for (Index_Type i = 0; i < vertices_count(); i++) {
        if (set_bias_1) {
            if (get_vertex(i).neuron_type == Neuron_Type::BIAS) {
                get_vertex(i).last_output = FP_(1);
                continue;
            }
        }
        get_vertex(i).last_output = FP_(0);
    }
}
template <typename FP_Type, typename Neuron_Type, typename Index_Type>
inline void VolatileGraph<FP_Type, Neuron_Type, Index_Type>::calculate_this_output(
    const Index_Type& vert_idx, FP_Type& output) {
    // {
    // It is important not to set output to 0.0 then use it as sum, as
    // it can cause conflicts with LSTM (Long short term mem) for self-loops.
    FP_Type sum = 0.0;
    const typename Vertex::ArrowList& in_arrows = get_vertex(vert_idx).inwards_edges;
    for (Index_Type arrow_idx = 0; arrow_idx < in_arrows.size(); arrow_idx++) {
        // weighted output = output of vertex of the incoming arrow * weight of
        // incoming arrow
        const Edge this_edge = get_edge(in_arrows[arrow_idx]);
        if (this_edge.disabled()) {
            continue;
        }
        const Vertex last_vert = get_vertex(this_edge.in_vertex_indx);
        // The output of the vertex of the incoming arrow.
        const FP_Type& last_out = last_vert.last_output;
        // The weight of this inward arrow
        const FP_Type& arrow_weight = this_edge.weight;
        // Add this to sum
        sum += last_out * arrow_weight;
#ifndef NEURAL_NDEBUG
        std::cout << "Accessing "
                  << "vert_idx: " << vert_idx << " arrow_idx: " << arrow_idx
                  << " Vertex: " << NeuralNetwork::to_string(last_vert)
                  << " last_out: " << Common::ryu_d2s(last_out)
                  << " arrow_weight: " << Common::ryu_d2s(arrow_weight)
                  << "\nSum is now: " << Common::ryu_d2s(sum) << std::endl;

#endif
    }
    // Apply the sigmoid function
    const FP_Type& response = get_vertex(vert_idx).response;
    output = Common::ZMath::sigmoid(sum, response);
#ifndef NEURAL_NDEBUG
    std::cout << "Index " << vert_idx << ": Calculate_output outputs: " << Common::ryu_d2s(output)
              << std::endl;
#endif
    //#undef in_arrows
}
template <typename FP_Type, typename Neuron_Type, typename Index_Type>
template <typename Vect_In, typename Vect_Out, typename Start_Idx_Func, bool parse_from_inputs,
          bool extract_outputs>
void VolatileGraph<FP_Type, Neuron_Type, Index_Type>::calculate_outputs(
    const Vect_In& inputs, Vect_Out& outputs, Start_Idx_Func start_idx_type_func) {
    // {
    Index_Type vert_idx = 0;
    if (parse_from_inputs && inputs.size() > 0) {
        vert_idx = start_idx_type_func(vert_idx, Neuron_Type::INPUT, vertices);
        // Assumes vert_idx == 0 means it's the input.
        ASSERT(vert_idx != -1,
               "The given start_idx_of function could not find INPUT. Either the start_idx_of func "
               "is broken or structure does not have INPUT vertex.");
        ASSERT(get_vertex(vert_idx).neuron_type == Neuron_Type::INPUT,
               "start_idx_of function returns a non -1 index that is not INPUT?");
        do {
            ASSERT(vert_idx < inputs.size(), "inputs do not provide enough inputs for all nodes");
            get_vertex(vert_idx).last_output = inputs[vert_idx];
#ifndef NEURAL_NDEBUG
            std::cout << "Index " << vert_idx << "(INPUT_NODE) Outputs: "
                      << Common::ryu_d2s(get_vertex(vert_idx).last_output) << std::endl;
#endif
            ++vert_idx;
        } while (get_vertex(vert_idx).neuron_type == Neuron_Type::INPUT);
    } // parse from inputs
    // Handle bias index
    vert_idx = start_idx_type_func(vert_idx, Neuron_Type::BIAS, vertices);
    // Set all bias nodes to 1
    if (vert_idx > 0) {
        for (; get_vertex(vert_idx).neuron_type == Neuron_Type::BIAS; vert_idx++) {
            // Assign output to const 1. The actual "bias" that we need to
            // tune is the Edge/Link's weight.
            CAN_BE_OPTIMIZED_MSG("We can just set this once and not do this again in the ctor");
            get_vertex(vert_idx).last_output = FP_(1);
        }
    }
    // Calculate HIDDEN and OUTPUT
    const Neuron_Type calculating_type[] = {Neuron_Type::HIDDEN, Neuron_Type::OUTPUT};
    for (Index_Type type = 0; type < Common::array_size(calculating_type); type++) {
        for (vert_idx = start_idx_type_func(vert_idx, calculating_type[type], this->vertices);
             vert_idx < vertices_count() &&
             get_vertex(vert_idx).neuron_type == calculating_type[type];
             vert_idx++) {
#ifndef NEURAL_NDEBUG
            std::cout << "Calculating "
                      << (get_vertex(vert_idx).neuron_type == Neuron_Type::HIDDEN ? "HIDDEN"
                                                                                  : "OUTPUT")
                      << " Node index " << vert_idx << std::endl;
#endif
            // calculate the output of this vertex and put it to last_output
            // of this vertex.
            CAN_BE_OPTIMIZED_MSG("If FP_Type is float, can safely use OpenCL.");
            calculate_this_output(vert_idx, get_vertex(vert_idx).last_output);
#ifndef NEURAL_NDEBUG
            std::cout << "Index " << vert_idx
                      << " Outputs: " << Common::ryu_d2s(get_vertex(vert_idx).last_output)
                      << std::endl;
#endif
            // if we want to extract output and the current vertex is the output node,
            // append this output to @param outputs.
            if (extract_outputs && get_vertex(vert_idx).neuron_type == Neuron_Type::OUTPUT) {
                // append last_output of this output vertex to @param output.
                outputs.push_back(get_vertex(vert_idx).last_output);
            }
        }
    }
}
template <typename FP_Type, typename Neuron_Type, typename Index_Type>

void VolatileGraph<FP_Type, Neuron_Type, Index_Type>::append_str(std::string& str2append) const {
    //{
    str2append.append("Neurons:\n");
    for (Index_Type i = 0; i < vertices.size(); i++) {
        //_V_LIST_FIELD_(vertices[i], append_str)(str2append);
        get_vertex(i).append_str(str2append);
        str2append.append("  ");
    }
    str2append.append("\nCalculation path:\n");
    // For input vertices, just copy @param inputs to these nodes sequentially.
    Index_Type vert_idx = 0; // vertex index in this->vertices.
    str2append.append("Parsing inputs:\n");

    while (get_vertex(vert_idx).neuron_type == NeuronType::INPUT) {
        // assign output to current vertex in this->vertices
        get_vertex(i).append_str(str2append);
        ++vert_idx;
    }
    str2append.append("\nBiases:\n");
    while (_GET_FIELD_AT_INDEX_(neuron_type, vert_idx) == NeuronType::BIAS) {
        // Assign output to const 1. The actual "bias" that we need to
        // tune is the Edge/Link's weight.
        CAN_BE_OPTIMIZED_MSG("We can just set this once and not do this again");
        get_vertex(i).append_str(str2append);
        ++vert_idx;
    }
    str2append.append("Hidden & Output:\n");
    // In the case of HIDDEN and OUTPUT:
    for (; vert_idx < vertices.size(); vert_idx++) {
        // calculate the output of this vertex and put it to last_output
        // of this vertex.
        str2append.append("Neuron: ");
        get_vertex(i).append_str(str2append);
        str2append.append("\nCalculation:\n ");
        const typename Vertex::ArrowList& in_arrows = get_vertex(vert_idx).inwards_edges;
        for (Index_Type arrow_idx = 0; arrow_idx < in_arrows.size(); arrow_idx++) {
            // weighted output = output of vertex of the incoming arrow * weight of
            // incoming arrow
            // The output of the vertex of the incoming arrow.
            // get_vertex(get_edge(in_arrows[arrow_idx]).in_vertex_indx).append_str(str2append);
            edge2vertex(in_arrows[arrow_idx], SOURCE_VERTEX).append_str(str2append);
            str2append.append(" ");
            // The weight of this inward arrow
            get_edge(in_arrows[arrow_idx]).append_str(str2append);
            // Add this to sum
            str2append.append("\n");
        }
        // if we want to extract output and the current vertex is the output node,
        // append this output to @param outputs.
        if (get_vertex(vert_idx).neuron_type == NeuronType::OUTPUT) {
            str2append.append("<OUTPUT>: ");
            str2append.append(Common::ryu_d2s(get_vertex(vert_idx).last_output));
            str2append.append("\n");
        }
    }
}

} // namespace NeuralNetwork