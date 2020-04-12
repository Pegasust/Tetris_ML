#pragma once
#include <vector>
#include "../common/common.hpp"
// Swap is in one of these.
#include <algorithm>
#include <utility>

//#define NEURAL_NDEBUG

/* ============================= NEURAL_GRAPH ========================= */
namespace NeuralNetwork {
// The type of Neuron: Input, which reads from list of inputs, Bias, which is const
// during simulation, Hidden, which is a "hidden" node representing the intelligence
// or a complexity to resolve from inputs, Output, which is the node to be outputted
// to the program. The order of declaring NeuronType is vital; otherwise, Graph::rearrange
// will not work correctly.
enum NeuronType : unsigned char { INPUT = 0, BIAS = 1, HIDDEN = 2, OUTPUT = 3, UNDEFINED = 4 };

// Assigns neuron type to cstring. Requires at least length of 8.
void neuron_type_2_cstr(const NeuronType& type, char cstr[]);

/*
 * The graph is a good neural network phenotype (phenotype is the one that interacts
 * with the environment). Genotype, on the other hand, refers to the traits or info
 * that determines the phenotype (genes, can be passed down from parents to children).
 * Modifications on the model (change in structure, weights,...) should be made on the
 * genotype and not this graph (the phenotype).
 * TEMPLATE:
 * typename FP_Type: the floating point type that is used to store and compute
 */
template <typename FP_Type = double>
class Graph {
public:
    class Vertex;
    // The edge or the "link" between two neurons.
    // Each edge holds the input and output vertex, as well as the weight of the edge.
    class Edge {
    public:
        // The input vertex
        Vertex* in;
        // The output vertex
        Vertex* out;
        // The weight of this edge
        FP_Type weight;
        // Whether this link is recurrent.
        // IS THIS NEEDED? We can get info from last_output from Vertex.
        bool is_recurrent;
        Edge(Vertex* in, Vertex* out, FP_Type weight, bool recurrent = false)
            : in(in), out(out), weight(weight), is_recurrent(recurrent) {
#ifndef NEURAL_NDEBUG
            std::cout << "New Edge created" << std::endl;
#endif
        }
        // Adds to str2append the string representation of this edge.
        // NOT THREAD SAFE!
        void append_str(std::string& str2append) const;
    };

    // The vertex or the "neuron".
    // Each vertex contains all inward edges and outward edges
    // and holds activation_response and its last output.
    // Calculations to get output out of a graph should be made on a Vertex,
    // and auxilary data structure should store Vertices so that the calculation
    // will not run into self-loop and recurrent.
    class Vertex {
    public:
        using ArrowList = std::vector<Edge*>;
#define _A_LIST_FIELD_(list_element, field_name) list_element->##field_name
        // All arrows/edges pointing towards this vertex
        ArrowList inwards_arrows;
        // All arrows/edges coming out from this vertex
        ArrowList outwards_arrows;
        // Absolute value approaches 0 -> more piecewise-like activation
        // function. If value > 0, then the output will be "in support" from
        // the input; otherwise, the output dislikes information from input.
        // Think of this as you as a player sees a villain (negative case).
        FP_Type activation_response;
        // The last output from this vertex. Storing this value
        // allows implementation of long short-term memory (LSTM)
        FP_Type last_output;
        // Type of this neuron.
        NeuronType neuron_type;

        Vertex(NeuronType type, FP_Type activation_response = 1, FP_Type last_output = 0)
            : neuron_type(type), activation_response(activation_response), last_output(last_output),
              inwards_arrows(), outwards_arrows() {
#ifndef NEURAL_NDEBUG
            std::cout << "New Vertex created" << std::endl;
#endif 
        }
        // Adds to str2append the string representation of this edge.
        // NOT THREAD-SAFE!
        void append_str(std::string& str2append) const;
    };

public:
    using VertexList = std::vector<Vertex>;
    using Neuron = Vertex;
    using Link = Edge;

private:
    /*The neural nodes of this graph. It should be oredered from input, bias, hidden, output*/
    VertexList vertices;
    // This value can be determined at the moment of generating from phenotype.
    int graph_depth;
// Gets the literal field at @param index in vertices.
#define _V_LIST_FIELD_(list_element, field_name) list_element.##field_name
#define _GET_FIELD_AT_INDEX_(field_name, index) _V_LIST_FIELD_(this->vertices[index], ##field_name)
    // Calculates the output of Vertex at @param vert_idx and outputs it to @param output
    // This function finds the sum of all weighted input vertices (last_output * weight),
    // then returns the sigmoid with dependent on the link's activation_response.
    void calculate_output(const int& vert_idx, FP_Type& output);

public:
    /*
     * Params:
     *  phenotyped_vertices: the collection of vertices that is well-initialized
            with inwards and outwards arrows that are also well-linked. This should
            be ordered from INPUT, BIAS, HIDDEN, OUTPUT.
    */
    // template<bool rearrange_vertices = true> // Unreachable template.
    Graph(VertexList& phenotyped_vertices, int depth, bool rearrange_vertices = true)
        : vertices(phenotyped_vertices), graph_depth(depth) {
        if (rearrange_vertices) {
            rearrange(vertices);
        }
    };
    /*
     * Rearranges @param vertices to follow the order: INPUT, BIAS, HIDDEN, OUTPUT.
     */
    static void rearrange(VertexList& vertices);
    /*
     * Rearranges @param vertices to follow the order: INPUT, BIAS, HIDDEN, OUTPUT
     * given the count for each type in @param type_count. type_count should be
     * ordered from INPUT, BIAS, HIDDEN, to OUTPUT.
     *
     * @param type_count will be modified after this call.
     *
     * template:
     *  type_count_normalized: if this is set, skip the normalize step and start
            rearranging. type_count is normalized if it's in this form:
            (INPUT = 0, BIAS = 1, HIDDEN = 2, OUTPUT = 3)
            [0, count[0], count[0]+count[1], count[0]+count[1]+count[2]].
            Unnormalized is in this form:
            [count[0], count[1], count[2], count[3]] (notice how count[3] is not needed)
    */
    template <bool type_count_normalized = false>
    static void rearrange_knew_counts(VertexList& vertices, std::vector<int>& type_count);
    // Calculates inputs through this graph and assign to output nodes.
    // If extract_output is set, output is appeneded with values from output nodes.
    // If parse_from_inputs is set, @param inputs will be forwarded to input nodes'
    // last_output values.
    template <bool extract_output = true, bool parse_from_inputs = true>
    void calculate_outputs(const std::vector<FP_Type>& inputs, std::vector<FP_Type>& outputs);
    // NOT THREAD-SAFE!
    void append_str(std::string& str2append) const;
};
using GraphF = Graph<float>;
using GraphD = Graph<double>;
// Simple wrapper around anytype that contains public method in signature:
// void append_str(std::string&) const
template <typename T>
std::string to_string(const T&);
} // namespace NeuralNetwork

#include "neural_graph.ipp"
#undef _GET_FIELD_AT_INDEX_
#undef _V_LIST_FIELD_
#undef _A_LIST_FIELD_
/* ==========================END NEURAL_GRAPH ========================= */

#undef NEURAL_NDEBUG