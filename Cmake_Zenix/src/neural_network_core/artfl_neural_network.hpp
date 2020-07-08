#pragma once
#include <vector>
#include "../common/common.hpp"
#include "neural_graph.hpp"
#include "neuron_type.hpp"
// std::swap is in one of these
#include <algorithm>
#include <utility>

#include <array>
#include <string>
#include <type_traits>
namespace NeuralNetwork {
template <typename FP_Type = double, typename Neuron_Type = NeuralNetwork::NeuronType,
          typename Index_Type = std::size_t>
class VolatileGraph {
public:
    static constexpr Neuron_Type TYPE_ARRAY_SIZE = Neuron_Type::UNDEFINED;
    // For vertex2edge or any other retrievals, return the edge going in.
    static constexpr bool ARROW_IN = true;
    // For vertex2edge or any other retrievals, return the edge going out.
    static constexpr bool ARROW_OUT = false;
    // For edge2vertex or any other retrievals, return the source vertex.
    static constexpr bool SOURCE_VERTEX = true;
    // For edge2vertex or any other retrievals, return the destination vertex.
    static constexpr bool DEST_VERTEX = false;
    using TypeEndArray = std::array<Index_Type, TYPE_ARRAY_SIZE>;
    // Cast to FP_Type
    template <typename T>
    inline static constexpr FP_Type FP_(const T num) {
        return static_cast<FP_Type>(num);
    }
    // Cast to Neuron_Type
    template <typename T>
    inline static constexpr Neuron_Type NT_(const T num) {
        return static_cast<Neuron_Type>(num);
    }
    // The edge is the link between two vertices. In ANNs, it is the neuron link that contains
    // weight. This edge also requires vertices to be stored in a contiguous blocks of memory (like
    // using arrays or vectors).
    class Edge {
    public:
        FP_Type weight;
        // The index in the graph's vertices vector of the incoming vertex (from)
        Index_Type in_vertex_indx;
        // The index in the graph's vertices vector of the outcoming vertex (to)
        Index_Type out_vertex_indx;
        // is_disabled
        bool is_disabled;
        inline bool is_recurrent() {
            return in_vertex_indx == out_vertex_indx;
        }
        inline bool disabled() {
            return is_disabled;
        }
        // Returns whether this edge is DISABLED AFTER the toggle.
        inline bool toggle_disabled() {
            is_disabled = !is_disabled;
            return is_disabled;
        }
        Edge(const Index_Type in, const Index_Type out, FP_Type weight, bool disabled = false)
            : in_vertex_indx(in), out_vertex_indx(out), weight(weight), is_disabled(disabled) {
#ifndef NEURAL_NDEBUG
            std::cout << "New Edge create" << std::endl;
#endif
        }
        // Adds to str2append the string representation of this edge.
        // NOT THREAD SAFE!
        void append_str(std::string& str2append) const;
    };
    // The vertex is the center of processing: it contains can retrieve data from the last layer
    // through an Edge, and exports its output within this structure.
    // This vertex contains an iterable collection of inwards and outwards edges, as well as
    // activation response.
    class Vertex {
    public:
        using ArrowList = std::vector<Index_Type>;
        // All arrows/edges pointing towards this vertex
        ArrowList inwards_edges;
        // All arrows/edges coming from this vertex
        ArrowList outwards_edges;
        // Absolute value approaches 0 -> more piecewise-like activation
        // function. If value > 0, then the output will be "in support" from
        // the input; otherwise, the output dislikes information from input.
        // Think of this as you as a player sees a villain (negative case).
        FP_Type response;
        // The last output from this vertex. Storing this value
        // allows implementation of long short-term memory (LSTM)
        FP_Type last_output;
        // Type of this neuron.
        Neuron_Type neuron_type;
        Vertex(Neuron_Type type, FP_Type activation_resposne = FP_(0), FP_Type last_output = FP_(0))
            : neuron_type(type), response(activation_resposne), last_output(last_output) {
#ifndef NEURAL_NDEBUG
            std::cout << "New Vertex created" << std::endl;
#endif
        } // Adds to str2append the string representation of this edge.
        // NOT THREAD-SAFE!
        void append_str(std::string& str2append) const;
    };

    /* ========================= FIELDS ============================ */
protected:
    // The vertices that will be ordered contiguously based on the order of Neuron_Type.
    // In NeuralNetwork::Neuron_Type, it will be ordered: INPUT, BIAS, HIDDEN, OUTPUT.
    // If intend to swap vertices, expect to use an additional vector for lookup.
    // The vertices should be ordered as per the expected calculation order (hidden shoud
    // of the next layer should be stored after the hidden of this layer).
    std::vector<Vertex> vertices;
    // The vector of all edges stored in this graph.
    std::vector<Edge> edges;

public:
    /* ========================= GETTERS =========================== */
    inline std::vector<Vertex>& get_vertices() {
        return vertices;
    }
    inline const std::vector<Vertex>& get_vertices() const {
        return vertices;
    }
    inline std::vector<Edge>& get_edges() {
        return edges;
    }
    inline const std::vector<Edge>& get_edges() const {
        return edges;
    }
    inline Index_Type vertices_count() {
        return vertices.size();
    }
    inline Index_Type edges_count() {
        return edges.size();
    }
    inline const Index_Type vertices_count() const {
        return vertices.size();
    }
    inline const Index_Type edges_count() const {
        return edges.size();
    }
    inline Edge& get_edge(const Index_Type edge_idx) {
        return edges[edge_idx];
    }
    inline Vertex& get_vertex(const Index_Type vertex_idx) {
        return vertices[vertex_idx];
    }
    inline const Edge& get_edge(const Index_Type edge_idx) const {
        return edges[edge_idx];
    }
    inline const Vertex& get_vertex(const Index_Type vertex_idx) const {
        return vertices[vertex_idx];
    }
    inline Vertex& edge2vertex(const Index_Type edge_idx, const bool source_vertex) {
        Edge edge = get_edge(edge_idx);
        return get_vertex(source_vertex ? edge.in_vertex_indx : edge.out_vertex_indx);
    }
    inline const Vertex& edge2vertex(const Index_Type edge_idx, const bool source_vertex) const {
        const Edge edge = get_edge(edge_idx);
        return get_vertex(source_vertex ? edge.in_vertex_indx : edge.out_vertex_indx);
    }
    inline Edge& vertex2edge(const Index_Type vertex_idx, const bool inwards,
                             const Index_Type arrow_idx) {
        Vertex vertex = get_vertex(vertex_idx);
        typename Vertex::ArrowList& arrows = inwards ? vertex.inwards_edges : vertex.outwards_edges;
        return get_edge(arrows[arrow_idx]);
    }
    inline const Edge& vertex2edge(const Index_Type vertex_idx, const bool inwards,
                                   const Index_Type arrow_idx) const {
        const Vertex vertex = get_vertex(vertex_idx);
        const typename Vertex::ArrowList& arrows =
            inwards ? vertex.inwards_edges : vertex.outwards_edges;
        return get_edge(arrows[arrow_idx]);
    }

private:
    // ================================ HELPER ====================================
    struct idx_of_type_functor {
        inline Index_Type operator()(const Index_Type hint, const Neuron_Type type,
                                     const std::vector<Vertex>& vertices) const noexcept {
            // Check from hint first
            for (Index_Type i = hint; i < vertices.size(); i++) {
                if (vertices[i].neuron_type == type) {
                    return i;
                }
            }
            // Check from start to hint.
            for (Index_Type i = 0; i < hint; i++) {
                if (vertices[i].neuron_type == type) {
                    return i;
                }
            }
            return -1;
        }
    };

public:
    static inline Index_Type get_vertex_id(Index_Type displacement, const Neuron_Type type,
                                           const TypeEndArray& arr) {
        if (type == NT_(0)) {
            return displacement;
        }
        return (displacement += arr[type - 1]);
    }
    static inline TypeEndArray make_end_array(Index_Type inputs = 3, Index_Type biases = 1,
                                              Index_Type hiddens = 0, Index_Type outputs = 1) {
        TypeEndArray arr;
        arr[Neuron_Type::INPUT] = inputs;
        arr[Neuron_Type::BIAS] = biases;
        arr[Neuron_Type::HIDDEN] = hiddens;
        arr[Neuron_Type::OUTPUT] = outputs;
        for (Index_Type i = 1; i < arr.size(); i++) {
            arr[i] += arr[i - 1];
        }
        return arr;
    }

public:
    /* ========================= MODIFIERS ========================= */
    // Create a new edge, returns the index of the edge that has just been added.
    // This method is safe because the order of edges are not important in calculation.
    inline Index_Type make_edge(const Index_Type from_vertex_idx, const Index_Type dest_vertex_idx,
                                const FP_Type weight = FP_(1)) {
        edges.emplace_back(from_vertex_idx, dest_vertex_idx, weight);
        Index_Type new_edge_idx = edges_count() - 1;
        get_vertex(from_vertex_idx).outwards_edges.push_back(new_edge_idx);
        get_vertex(dest_vertex_idx).inwards_edges.push_back(new_edge_idx);
        return new_edge_idx;
    }
    // Creates a new vertex, returns the index of the vertex that has just been added.
    // This method is unsafe due to it not taking account to the order.
    // Params:
    // * neuron_type: the type of this vertex
    // * activaiton_response: the activation response parameter of this vertex. [Default: 1.0]
    // * initial_output: the initial output of this vertex. Setting this value to non-zero
    // effectively
    //              emulates "memory from past life". [Default: 0.0 (no memory)]
    inline Index_Type make_vertex(const Neuron_Type neuron_type,
                                  const FP_Type activation_response = FP_(1),
                                  const FP_Type initial_output = FP_(0)) {
        vertices.emplace_back(neuron_type, activation_response, initial_output);
        return vertices_count() - 1;
    }
    // Creates a new vertex, returns the iterator that contains the newly added element.
    // Params:
    // * loc_iter: the location iterator to the point that we want to insert a vertex to. This
    // location should be in the right position so that it conforms to the declared order of
    // Neuron_Type and all neurons of same types are stored contiguously.
    // * neuron_type: the type of this vertex
    // * activaiton_response: the activation response parameter of this vertex. [Default: 1.0]
    // * initial_output: the initial output of this vertex. Setting this value to non-zero
    // effectively emulates "memory from past life". [Default: 0.0 (no memory)]
    inline typename std::vector<Vertex>::iterator make_vertex_s(
        typename std::vector<Vertex>::const_iterator loc_iter, const Neuron_Type neuron_type,
        const FP_Type actvn_resp = FP_(1), const FP_Type initial_output = FP_(0)) {
        return vertices.emplace(loc_iter, neuron_type, actvn_resp, initial_output);
    }
    
    // Creates a new vertex, returns the iterator that contains the newly added element.
    // Params:
    // * type_end: the TypeEndArray of for this graph
    // * neuron_type: the type of this vertex
    // * activaiton_response: the activation response parameter of this vertex. [Default: 1.0]
    // * initial_output: the initial output of this vertex. Setting this value to non-zero
    // effectively
    //              emulates "memory from past life". [Default: 0.0 (no memory)]
    inline typename std::vector<Vertex>::iterator make_vertex_s(const TypeEndArray& type_end,
        const Neuron_Type neuron_type, const FP_Type actvn_resp = FP_(1),
        const FP_Type initial_output = FP_(0)) {
        Index_Type type_end_idx = type_end[neuron_type];
        typename std::vector<Vertex>::const_iterator loc_iter = std::next(get_vertices().begin(),
           type_end_idx);
        return make_vertex_s(loc_iter, neuron_type, actvn_resp, initial_output);
    }

    // Creates a new vertex, returns the displacement from the first vertex (idx).
    // Params:
    // * type_end: the TypeEndArray of for this graph
    // * neuron_type: the type of this vertex
    // * activaiton_response: the activation response parameter of this vertex. [Default: 1.0]
    // * initial_output: the initial output of this vertex. Setting this value to non-zero
    // effectively
    //              emulates "memory from past life". [Default: 0.0 (no memory)]
    inline Index_Type make_vertex_s_idx(
        const TypeEndArray& type_end, const Neuron_Type neuron_type,
        const FP_Type actvn_resp = FP_(1), const FP_Type initial_output = FP_(0)) {
        Index_Type type_end_idx = type_end[neuron_type];
        typename std::vector<Vertex>::const_iterator loc_iter =
            std::next(get_vertices().begin(), type_end_idx);
        make_vertex_s(loc_iter, neuron_type, actvn_resp, initial_output);
        return type_end_idx;
    }

    // Initializes vertices based on type_end.
    // Params:
    // * type_end: array that conforms to (type_end[i] = type_end[i-1] + num_neurons_type_i),
    // (type_end[-1] implies 0)
    // * data_response[NO_ARGS-compatible]: the vector of activation response (neuron[i].act_rsp ==
    // data_response[i]).
    // * data_ini_out[NO_ARGS-compatible]: the vector of initial_output (neuron[i].last_output ==
    // data_ini_out[i]).
    // * default_activation_response: the default activition response to use if data_response is
    // NO_ARGS or out of size.
    // * default_initial_output: the default initial output to use if data_ini_out is NO_ARGS or out
    // of size. Template Params: NONE, do not attempt to pass in any template argument, just let the
    // compiler deduct types.
    template <typename Vect_Resp, typename Vect_Out, bool reserve = true,
              bool has_resp = std::is_same<Vect_Resp, std::vector<FP_Type>>::value,
              bool has_ini_out = std::is_same<Vect_Out, std::vector<FP_Type>>::value>
    void make_vertices(const TypeEndArray& type_end, const Vect_Resp& data_response,
                       const Vect_Out& data_ini_out,
                       const FP_Type default_activation_response = FP_(1),
                       const FP_Type default_initial_output = FP_(0));

    // Initializes vertices based on type_end. This is a wrapper of the same method name, only
    // passing NO_ARGS to both data. Params:
    // * type_end: array that conforms to (type_end[i] = type_end[i-1] + num_neurons_type_i),
    // (type_end[-1] implies 0)
    // * default_activation_response: the default activition response to use
    // * default_initial_output: the default initial output to use
    inline void make_vertices(const TypeEndArray& type_end,
                              const FP_Type default_activation_response = FP_(1),
                              const FP_Type default_initial_output = FP_(0)) {
        make_vertices(type_end, Common::TemplateArgs::no_args<>(),
                      Common::TemplateArgs::no_args<>(), default_activation_response,
                      default_initial_output);
    }
    // Marks the edge disabled, the calculation will not take this edge into account.
    // The edge indices should not be changed as a result of disabled edge, so removing
    // the edge entirely is not recommended.
    // Returns 0 if operation ran unsuccessfully, returns non-zero if ran successfully.
    inline Index_Type disable_edge(Edge& edge);

    // Marks the edge enabled, the calculation will take this edge into account.
    // The edge indices should not be changed as a result of toggling edge, so re-adding
    // the edge is not recommended.
    // Returns 0 if operation ran unsuccessfully, returns non-zero if ran successfully.
    inline Index_Type enable_edge(Edge& edge);

    // Returns whether the edge is DISABLED AFTER the toggle.
    inline bool toggle_edge(Edge& edge);

    //    =============== SYNC ============= //
    /*
     * If Neuron_Type::INPUT == 0, vert_idx is simply the index from 0 to type_end[0]. If it is
     * not, this method requires the user to call get_vertex_id.
     */
    inline void set_input(const Index_Type vert_idx, const FP_Type new_output) {
        get_vertex(vert_idx).last_output = new_output;
    }
    inline FP_Type get_output(const Index_Type vert_idx) {
        return get_vertex(vert_idx).last_output;
    }
    inline const FP_Type get_output(const Index_Type vert_idx) const {
        return get_vertex(vert_idx).last_output;
    }
    inline void set_input(const Index_Type input_disp, const FP_Type new_output,
                          const TypeEndArray& type_end) {
        get_vertex(get_vertex_id(input_disp, Neuron_Type::INPUT, type_end)).last_output =
            new_output;
    }
    inline FP_Type get_output(const Index_Type output_disp, const TypeEndArray& type_end) {
        return get_vertex(get_vertex_id(output_disp, Neuron_Type::OUTPUT, type_end)).last_output;
    }
    inline const FP_Type get_output(const Index_Type vert_idx, const TypeEndArray& type_end) const {
        return get_vertex(get_vertex_id(output_disp, Neuron_Type::OUTPUT, type_end)).last_output;
    }
    void clear_vertex_outputs(const bool set_bias_1 = true);

private:
    void calculate_this_output(const Index_Type& vert_idx, FP_Type& output);

public:
    // Calculates the outputs given @param inputs. In normal case, @param outputs will be appended
    // with last_outputs from output nodes.
    // Params:
    // * inputs [NO_ARGS-compatible]: the inputs to be forwarded to input nodes for calculation.
    // * outputs[NO_ARGS-compatible]: the outputs to be appended from the calculation outputs from
    // output nodes.
    // * start_idx_type_func: the function to determine the starting index of given
    //          type. This should accept args in the following order, if passing functor, include
    //          trailing ():
    //             [*] Index_Type hint, [*] Neuron_Type search_type, [*] std::Vertex& vertices.
    template <typename Vect_In, typename Vect_Out, typename Start_Idx_Func,
              bool parse_from_inputs = std::is_same<std::vector<FP_Type>, Vect_In>::value,
              bool extract_outputs = std::is_same<std::vector<FP_Type>, Vect_Out>::value>
    void calculate_outputs(const Vect_In& inputs, Vect_Out& outputs,
                           Start_Idx_Func start_idx_type_func);
    // Calculates the outputs given @param inputs. In normal case, @param outputs will be appended
    // with last_outputs from output nodes.
    // Params:
    // * inputs [NO_ARGS-compatible]: the inputs to be forwarded to input nodes for calculation.
    // * outputs[NO_ARGS-compatible]: the outputs to be appended from the calculation outputs from
    // output nodes.
    template <typename Vect_In, typename Vect_Out,
              bool parse_from_inputs = std::is_same<std::vector<FP_Type>, Vect_In>::value,
              bool extract_outputs = std::is_same<std::vector<FP_Type>, Vect_Out>::value>
    inline void calculate_outputs(const Vect_In& inputs, Vect_Out& outputs) {
        calculate_outputs(inputs, outputs, idx_of_type_functor());
    }
    // A wrapper for calculate_outputs(NO_ARGS, NO_ARGS). Calling this wrapper function implies
    // synchronized inputs and outputs design.
    inline void update() {
        calculate_outputs(Common::TemplateArgs::no_args<>(), Common::TemplateArgs::no_args<>());
    }
    // NOT THREAD-SAFE!
    void append_str(std::string& str2append) const;
};

using VGraphD = VolatileGraph<double, NeuralNetwork::NeuronType, int>;
using VGraphF = VolatileGraph<float, NeuralNetwork::NeuronType, int>;
} // namespace NeuralNetwork

#include "artfl_neural_network.ipp"