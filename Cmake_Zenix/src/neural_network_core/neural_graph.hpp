#pragma once
#include "neuron_type.hpp"
#include <vector>
#include "../common/common.hpp"

// std::swap is in one of these or both.
#include <algorithm>
#include <utility>
//#define NEURAL_NDEBUG
namespace NeuralNetwork {
/*
 * The graph is a good neural network phenotype (phenotype is the one that interacts
 * with the environment). Genotype, on the other hand, refers to the traits or info
 * that determines the phenotype (genes, can be passed down from parents to children).
 * Modifications on the model (change in structure, weights,...) should be made on the
 * genotype and not this graph (the phenotype). The weights, however, can be changed on
 * the fly, but the structure should not be modified.
 *
 * Despite having reassign function, if hidden neurons are declared not in the order
 * of closest to input to closest to output, then the calculating algorithm fails.
 * TEMPLATE:
 * typename FP_Type: the floating point type that is used to store and compute
 */
template <typename FP_Type = double, typename Neuron_Type = NeuronType, typename index_type = int>
class Graph {
public:
    class Vertex;
    // The edge or the "link" between two neurons.
    // Each edge holds the input and output vertex, as well as the weight of the edge.
    class Edge {
    public:
        // The input vertex (the starting point)
        const Vertex* in; // Should have used index, so that it can still be referenced
        // correctly after reserve().
        // The output vertex (the point where the arrow points to)
        const Vertex* out; // Should have used index.
        // The weight of this edge
        FP_Type weight;
        // Whether this link is recurrent.
        // IS THIS NEEDED? We can get info from last_output from Vertex.
        bool is_recurrent;
        Edge(const Vertex* in, const Vertex* out, FP_Type weight, bool recurrent = false)
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
        Neuron_Type neuron_type;

        Vertex(Neuron_Type type, FP_Type activation_response = 1, FP_Type last_output = 0)
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
// Gets the literal field at @param index in vertices.
#define _V_LIST_FIELD_(list_element, field_name) list_element.##field_name
#define _GET_FIELD_AT_INDEX_(field_name, index) _V_LIST_FIELD_(this->vertices[index], ##field_name)

    /*
     * This creates a new Edge, which is then pushed back on @param out_edge and assigns
     * the vertices in @param v_list with this new Edge accordingly.

     * Pre-condition: @param out_edge must be reserved with enough size for this
     * push_back, otherwise, it would break. Consider using Graph's function
     * connect_vertices for a "safer" method.
     *
     * CURRENTLY NOT THREAD-SAFE.
    */
    // template <typename index_type = int>
    static void create_link(VertexList& v_list, std::vector<Edge>& out_edge, index_type in_Idx,
                            index_type out_idx, FP_Type weight, bool recurrent = false);
    /*
     * The structure that contains instruction for creating one edge on a created
     * VertexList that supports indexing.
     */
    class EdgeCreate {
        // The index of the input vertex in the given VertexList
        index_type in_idx;
        // The index of the output vertex in the given VertexList
        index_type out_idx;
        // The weight of the edge being created
        FP_Type weight;
        // Whether this link is recurrent.
        // Not sure if this is needed.
        bool is_recurrent;

    public:
        EdgeCreate(index_type in_idx, index_type out_idx, FP_Type weight, bool recurrent = false)
            : in_idx(in_idx), out_idx(out_idx), weight(weight), is_recurrent(recurrent){};

        /*
         * Apply this instruction on the given VertexList. This creates a new Edge, which
         * is then pushed back on @param all_edges and assigns the vertices with this
         * new Edge accordingly.

         * Pre-condition: @param all_edges must be reserved with enough size for this
         * push_back, otherwise, it would break. Consider using Graph's function
         * connect_vertices for a "safer" method.
        */
        void apply_vertex_list(VertexList& v_list, std::vector<Edge>& all_edges) const;
        /*
         * Apply this instruction on the given VertexList. This creates a new Edge, which
         * is then pushed back on @param all_edges and assigns the vertices with this
         * new Edge accordingly.

         * Pre-condition: @param all_edges must be reserved with enough size for this
         * push_back, otherwise, it would break. Consider using Graph's function
         * connect_vertices for a "safer" method.
        */
        void apply_vertex_list(VertexList& v_list, std::vector<Edge>& all_edges,
                               const std::vector<index_type>& lookup) const;
    };
    /*
     * The structure that contains instruction for creating one vertex on a created VertexList
     */
    struct VertexCreate {
        FP_Type activation_response;
        FP_Type initial_output;
        Neuron_Type neuron_type;
        VertexCreate(Neuron_Type neuron_type, FP_Type activation_response = 1,
                     FP_Type initial_output = 0)
            : activation_response(activation_response), initial_output(initial_output),
              neuron_type(neuron_type) {}
    };
    static void add_to_v_list(const VertexCreate& vc, VertexList& v_list);
    /*
    ======================= CLASS GRAPH BEGINS ======================
    */

protected:
    /*=======================================================================
    ============================== VARIABLES ================================
    */

    /*The neural nodes of this graph. It should be oredered from input, bias, hidden, output.
    It is vital that this field does not get modified directly, as it will break the program
    due to this holding the memory block! Swapping two vertices locations require the edges
    to swap its ins and outs too.
    */
    VertexList vertices;
    // This value can be determined at the moment of generating from phenotype.
    // This value is not necessary in output calculation. The ordering of vertices
    // matters.
    int graph_depth;
    /* This variable is only available for use if using ctor(const std::vector<FP_Type>&,
        index_type[], const std::vector<EdgeCreate>&). This field ensures the
        vector of edges will be deleted as this graph is out of scope.
     */
    std::vector<Edge> edges;
    // Calculates the output of Vertex at @param vert_idx and outputs it to @param output
    // This function finds the sum of all weighted input vertices (last_output * weight),
    // then returns the sigmoid with dependent on the link's activation_response.
    void calculate_output(const int& vert_idx, FP_Type& output);

public:
    inline const VertexList& const_vertices() const {
        return vertices;
    }
    inline const std::vector<Edge>& const_edges() const {
        return edges;
    }
    inline VertexList& get_vertices() {
        return vertices;
    }
    inline std::vector<Edge>& get_edges() {
        return edges;
    }
    inline index_type vertices_count() const {
        return vertices.size();
    }
    /*
     * Assigns vector of pointer to floating point values of input nodes so that
     * we can direrctly access & synchronize inputs. This allows usage of calculate_outputs
     * without the flag of parse_from_inputs.
     * UNTESTED.
     */
    template <bool arranged = true>
    inline void input_addresses(std::vector<FP_Type*>& inputs) {
        ASSERT(inputs.size() == 0, "inputs does not have 0 size");
        int vert_idx = 0;
        if (arranged) {
            while (_GET_FIELD_AT_INDEX_(neuron_type, vert_idx) == Neuron_Type::INPUT) {
                inputs.push_back(&_GET_FIELD_AT_INDEX_(last_output, vert_idx));
                vert_idx += 1;
            }
        } else {
            PRINT_DEBUG_WARNINGS("getting input_addresses that is unrarranged is unsafe.");
            for (; vert_idx < _V_LIST_FIELD_(this->vertices, size()); vert_idx++) {
                if (_GET_FIELD_AT_INDEX_(neuron_type, vert_idx) == Neuron_Type::INPUT) {
                    inputs.push_back(&_GET_FIELD_AT_INDEX_(last_output, vert_idx));
                }
            }
        }
    }
    /*
     * Assigns vector of pointer to floating point values of output nodes so that
     * we can direrctly access & synchronize outputs. This allows usage of calculate_outputs
     * without the flag of extract_output.
     * UNTESTED.
     */
    template <bool arranged = true>
    inline void output_addresses(std::vector<FP_Type*>& outputs) {
        ASSERT(outputs.size() == 0, "outputs does not have 0 size");
        int vert_idx = _V_LIST_FIELD_(this->vertices, size()) - 1;
        if (arranged) {
            while (_GET_FIELD_AT_INDEX_(neuron_type, vert_idx) == Neuron_Type::OUTPUT) {
                outputs.push_back((&_GET_FIELD_AT_INDEX_(last_output, vert_idx)));
                vert_idx -= 1;
            }
            // Revert outputs because we added it in the opposite direction
            std::reverse(outputs.begin(), outputs.end());
        } else {
            PRINT_DEBUG_WARNINGS("getting output_addresses that is unrarranged is unsafe.");
            for (; vert_idx < _V_LIST_FIELD_(this->vertices, size()); vert_idx++) {
                if (_GET_FIELD_AT_INDEX_(neuron_type, vert_idx) == Neuron_Type::INPUT) {
                    outputs.push_back((&_GET_FIELD_AT_INDEX_(last_output, vert_idx)));
                }
            }
        }
    }
    template <bool set_bias_to_1 = true>
    void zero_vertices_output() {
        for (index_type i = 0; i < _V_LIST_FIELD_(this->vertices, size); i++) {
            if (set_bias_to_1) {
                if (_V_LIST_FIELD_(this->vertices[i], neuron_type) == Neuron_Type::BIAS) {
                    _V_LIST_FIELD_(this->vertices[i], last_output) = static_cast<FP_Type>(1);
                }
            }
            _V_LIST_FIELD_(this->vertices[i], last_output) = static_cast<FP_Type>(0);
        }
    }
    /*
    * Params:
    *  phenotyped_vertices: the collection of vertices that is well-initialized
        with inwards and outwards arrows that are also well-linked. This should
        be ordered from INPUT, BIAS, HIDDEN, OUTPUT.
    */
    Graph(VertexList& phenotyped_vertices, int depth, bool rearrange_vertices = true)
        : vertices(phenotyped_vertices), graph_depth(depth) {
        if (rearrange_vertices) {
            rearrange(vertices);
        }
    };
    /*
    Empty default constructor. The end-user is expected to add vertices to the list and
    edges together and rearrange if it needs rearrangement.
    For the best performance and accuracy, add elements to vertices first
    */
    Graph() : graph_depth(-1){};

    /*
    A safe constructor that constructs from given data. This will create VertexList
    that stores in this class so that this->vertices and this->edges are freed as
    this obj is destructed.
    * PRE-CONDITION:
    * This constructor requires user to input vertices_data in the RIGHT calculation
    order.
    * PARAMS:
    *   vertices_data: the floating point values that stores activation response and
            initial last_output of i//2 neuron.
    *   vertex_type_count: the array that represents the count of each type of neuron.
    *   instructions: the data to create edges made from vertices_data (index is as
            per declared, meaning i-th entry (vertices_data[i*2] and [i*2+1]) of
            vertices data is i-th neuron declared in instructions.
    */
    Graph(const std::vector<FP_Type>& vertices_data, index_type vertex_type_count[],
          const std::vector<EdgeCreate>& instructions, int graph_depth = -1);

private:
    void init(const std::vector<FP_Type>& vertices_data, const std::vector<Neuron_Type>& types,
              std::vector<index_type>& vertex_type_count,
              const std::vector<EdgeCreate>& instructions, int graph_depth);

public:
    /*
     * A memory-safe and type-order-safe constructor that constructs from given data.
     * First, vertices are created from vertices_data. Knowing this might be unarranged,
     * the function uses a specialized counting sort that sorts all unarraged vertices
     * into the calculation-correct order. The sorting function also generates a lookup
     * map, which this constructor will use to get all edges declared in @param instructions
     * assigned to the correct vertices. However, the user still needs to define the correct
     * order for each type.
     */
    Graph(const std::vector<FP_Type>& vertices_data, const std::vector<Neuron_Type>& types,
          std::vector<index_type>& vertex_type_count, const std::vector<EdgeCreate>& instructions,
          int graph_depth = -1) {
        init(vertices_data, types, vertex_type_count, instructions, graph_depth);
    }
    /*
        Memory-safe and type-order safe. Does the same process as
        ctor(vector<FP_Type>, vector<Neuron_Type>, index_type[], vector<EdgeCreate>)
    */
private:
    void init(const std::vector<VertexCreate>& vertices,
              const std::vector<EdgeCreate>& instructions,
              std::vector<index_type>& vertex_type_count, int graph_depth);

public:
    Graph(const std::vector<VertexCreate>& vertices, const std::vector<EdgeCreate>& instructions,
          std::vector<index_type>& vertex_type_count, int graph_depth = -1) {
        init(vertices, instructions, vertex_type_count, graph_depth);
    }
    /* ===================================================================
        Memory-safe, laziest, type-order-safe ctor
       ===================================================================
    */
    Graph(const std::vector<VertexCreate>& vertices, const std::vector<EdgeCreate>& instructions,
          int graph_depth = -1);
    /*
     * Add (@param data).size()/2 vertices to this->vertices. Ensure that capacity of
     * this->vertices can hold (@param data).size()/2 more vertices.
     *
     * Params:
     *  Input:
     *    data: vector of floating point values where each vertex is defined by 2 FP_Types
                sequentially. Ex: [0.0, 1.0] will add 1 vertex with activation_response 0.0
                and last_output 1.0. [1.0, 0.0, 2.0, 10.0] -> 2 vertices: [(1.0, 0.0), (2.0, 10.0)]
    */
    // template <typename index_type = int>
    void add_vertices(const std::vector<FP_Type>& data_response_initial_output, index_type inputs,
                      index_type biases, index_type hiddens, index_type outputs);
    /*
     * Add (@param data).size()/2 vertices to this->vertices. Ensure that capacity of
     * this->vertices can hold (@param data).size()/2 more vertices.
     =================================== UNTESTED AND UNSAFE ==================================
     *
     * Params:
     *  Input:
     *    data: vector of floating point values where each vertex is defined by 2 FP_Types
                sequentially. Ex: [0.0, 1.0] will add 1 vertex with activation_response 0.0
                and last_output 1.0. [1.0, 0.0, 2.0, 10.0] -> 2 vertices: [(1.0, 0.0), (2.0, 10.0)]
     *    type_count: the array that stores the number of each type such that type_count[Neuron_Type
     n] contains the number of vertices of type n. Ex: [1, 0, 108, 1] will create 1 input node, 0
     bias node, 108 hidden nodes, 1 output node. If, however, type_count contains not all data from
     @param data_response_initial_output, nodes of type UNDEFINED will be added with the declared
     activation_response and initial_output.
    */
    template <                              // typename index_type = int,
        typename Neuron_Type = Neuron_Type> // redundant?
    void add_vertices(const std::vector<FP_Type>& data_response_initial_output,
                      index_type type_count[]);
    /* ===================================== FAVORABLE FUNCTION
     =====================================
     * Add (@param data).size()/2 vertices to this->vertices. Ensure that capacity of
     * this->vertices can hold (@param data).size()/2 more vertices.
     *
     * Params:
     *  Input:
     *    data: vector of floating point values where each vertex is defined by 2 FP_Types
                sequentially. Ex: [0.0, 1.0] will add 1 vertex with activation_response 0.0
                and last_output 1.0. [1.0, 0.0, 2.0, 10.0] -> 2 vertices: [(1.0, 0.0), (2.0, 10.0)]
     *    type_count: the array that stores the number of each type such that type_count[Neuron_Type
     n] contains the number of vertices of type n. Ex: [1, 0, 108, 1] will create 1 input node, 0
     bias node, 108 hidden nodes, 1 output node. If, however, type_count contains not all data from
     @param data_response_initial_output, nodes of type UNDEFINED will be added with the declared
     activation_response and initial_output.
     *    default_initial_output: the default initial_output to use if data_response_initial_output
     runs out of value, or use_given_data is set to false.
     *    default_activation_response: the default activation response to use if
     data_response_initial_output runs out of value or use_given_data is set to false.
    */
    template <index_type array_size = Neuron_Type::UNDEFINED, bool use_given_data = true,
              typename vect = std::vector<FP_Type>>
    void add_vertices(const vect data_response_initial_output,
                      const std::array<index_type, array_size>& type_count,
                      const FP_Type default_activation_response = static_cast<FP_Type>(1),
                      const FP_Type default_initial_output = static_cast<FP_Type>(0));

    template <index_type array_size = Neuron_Type::UNDEFINED>
    void add_vertices(const std::array<index_type, array_size>& type_count,
                      const FP_Type default_activation_response = static_cast<FP_Type>(1),
                      const FP_Type default_initial_output = static_cast<FP_Type>(0)) {
        add_vertices<array_size, false, std::size_t>(0, type_count, default_activation_response,
                                                     default_initial_output);
    }
    /*
     * Make connections and links, then apply those to @param vertices based on
     * instructions from @param instructions
     */
    // template <typename index_type = int>
    static void connect_vertices(VertexList& vertices, const std::vector<EdgeCreate>& instructions,
                                 std::vector<Edge>& all_edges, const index_type instrs_offset = 0);

    /*
     * Make connections and links, then apply those to @param
     * vertices based on instructions from @param instructions
     */
    // template <typename index_type = int>
    static void connect_vertices(VertexList& vertices, const std::vector<EdgeCreate>& instructions,
                                 std::vector<Edge>& all_edges, std::vector<index_type> lookup);

    // template <typename index_type = int>
    inline void connect_self_vertices(const std::vector<EdgeCreate>& instrs,
                                      std::vector<Edge>& all_edges,
                                      const index_type instrs_offset = 0) {
        connect_vertices(this->vertices, instrs, all_edges, instrs_offset);
    }
    inline void connect_self_vertices(const std::vector<EdgeCreate>& instrs,
                                      const index_type instrs_offset = 0) {
        connect_self_vertices(instrs, this->edges, instrs_offset);
    }
    /*
     * Rearranges @param vertices to follow the order: INPUT, BIAS, HIDDEN, OUTPUT.
     * If @param vertices is already "sorted," this function only checks with O(n) traversal
     * and do not perform sort.
     * Since this might require swapping edges of numerous vertices, this function can
     * take a long time for large graphs.
     */
    // template <typename index_type = int>
    static void rearrange(VertexList& vertices, std::vector<index_type>& lookup);
    /*
     * Rearranges @param vertices to follow the order: INPUT, BIAS, HIDDEN, OUTPUT.
     * This function does use lookup so that vertices can swap quickly and easilly.
     */
    // template <typename index_type = int>
    inline static std::vector<index_type> rearrange(VertexList& vertices) {
        std::vector<index_type> retval;
        rearrange(vertices, retval);
        // swaps all existing arrows to their arranged locations.
        for (int i = 0; i < retval.size(); i++) {
            if (i == retval[i]) {
                continue;
            }
            // else if (retval[i] >> ((sizeof(index_type) * 8) - 1)) {
            //    // Yes, it's sorted, I know, now unmark it.
            //    retval[i] = ~retval[i];
            //    continue;
            //}
            swap_arrows_vertex_index(vertices, i, i);
            // Mark sorted
            // retval[retval[i]] = ~retval[retval[i]];
        }
        return retval;
    }
    // template <typename index_type = int>
    inline void rearrange_self(std::vector<index_type>& lookup) {
        rearrange(vertices, lookup);
    }
    // template <typename index_type = int>
    inline std::vector<index_type> rearrange_self() {
        return rearrange(this->vertices);
    }
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
    template <bool type_count_normalized = false //, typename index_type = int
              >
    static void rearrange_knew_counts(VertexList& vertices, std::vector<index_type>& type_count,
                                      std::vector<index_type>& lookup);
    // Calculates inputs through this graph and assign to output nodes.
    // If extract_output is set, output is appeneded with values from output nodes.
    // If parse_from_inputs is set, @param inputs will be forwarded to input nodes'
    // last_output values.
    // @param start_idx_type_func takes in (const) Neuron_Type and (const) VertexList&
    // and output Index_Type that is the starting index of iteration of given Neuron_Type
    template <bool extract_output = true, bool parse_from_inputs = true, typename Start_Idx_Func>
    void calculate_outputs(const std::vector<FP_Type>& inputs, std::vector<FP_Type>& outputs,
                           Start_Idx_Func start_idx_type_func);
    template <bool extract_output = true, bool parse_from_inputs = true>
    inline void calculate_outputs(const std::vector<FP_Type>& inputs,
                                  std::vector<FP_Type>& outputs) {
        calculate_outputs<extract_output, parse_from_inputs>(inputs, outputs,
                                                             (start_idx_of_functor()));
    }
    // The same as calculate_outputs, but this is intended to be used with the
    // io_sync pattern.
    inline void update() {
#define __EMPTY_VECTOR__ static_cast<std::vector<FP_Type>>(NULL)
        calculate_outputs<false, false>(__EMPTY_VECTOR__, __EMPTY_VECTOR__);
#undef __EMPTY_VECTOR__
    }
    // NOT THREAD-SAFE!
    void append_str(std::string& str2append) const;
    /* Swaps the location of vertex of index @param idx0 with @param idx1. This function
    also changes the inwards_arrows and outwards_arrows of each vertex.
    */
    template < // typename index_type = int,
        bool swap_arrows = true>
    static void swap_vertex_index_position(VertexList& vertices, index_type idx0, index_type idx1);

    static void count_type(const VertexList& vertices, std::vector<int>& out_count,
                           bool& out_need_sort);

private:
    struct start_idx_of_functor {
        index_type operator()(const Neuron_Type type, const VertexList& vertices) {
            for (index_type i = 0; i < _V_LIST_FIELD_(vertices, size()); i++) {
                if (_V_LIST_FIELD_(vertices[i], neuron_type) == type) {
                    return i;
                }
            }
            return -1;
        }
    };
    // static index_type start_idx_of(const Neuron_Type type, const VertexList& vertices);
    // Swaps all <out> of inwards_arrows and <in> of outwards_arrow of vertices at @param getter
    // with vertex of vertices at @param swapped_index
    // template <typename index_type = int>
    static void swap_arrows_vertex_index(VertexList& vertices, index_type getter,
                                         index_type swapped_index);
    /*
     * Given an unordered vertex list, create such data structure called rearrange plan in which
     each swap entry is in (idx0, idx1), which unordered_vertices[idx0] should be swapped with
     unordered_vertices[idx1] in order to make calculations with unordered_vertices accurate.

     * Params:
     *  Input:
     *      unordered_vertices: The list of vertices in unordered arrangement. The edges binding
                does not matter. The completion of this function will sort unordered_vertices.
     *  Output:
     *      lookup: the vector of index such that before-sort-unordered_vertices[lookup[i]] yields
                the vertex at the "sorted"/rearranged
     */
    // template <typename index_type = int>
    // static void rearrange_plan(VertexList& unordered_vertices,
    //    std::vector<index_type>& lookup);
};
using GraphF = Graph<float, NeuronType>;
using GraphD = Graph<double, NeuronType>;

}

#include "neural_graph.ipp"
#undef _GET_FIELD_AT_INDEX_
#undef _V_LIST_FIELD_
#undef _A_LIST_FIELD_
// ============================= EXAMPLE USAGE ==============================
/*
bool graph_safe_ctor_recurrent_self_loop_test() {
    std::vector<GraphD::VertexCreate> vertices{
        GraphD::VertexCreate(NeuronType::INPUT), GraphD::VertexCreate(NeuronType::INPUT),
        GraphD::VertexCreate(NeuronType::INPUT), GraphD::VertexCreate(NeuronType::OUTPUT),
        GraphD::VertexCreate(NeuronType::HIDDEN)};
    std::vector<double> inputs{1.4, 5.8, 12.2};
    std::vector<double> output;
    const int iterations = 5;
    double node5_out = 0;
    double node4_out = 0;

    // recurrent:
    std::vector<GraphD::EdgeCreate> recurrent_edges{{0, 3, 0.7}, {2, 3, 0.5}, {1, 4, 0.2},
                                                    {4, 3, 0.4}, {0, 4, 0.6}, {3, 4, 0.6}};
    GraphD recurrent_graph(vertices, recurrent_edges);
    for (int i = 0; i < iterations; i++) {
        output.clear();
        std::cout << "\nIteration: " << i << std::endl;
        recurrent_graph.calculate_outputs(inputs, output);
        node5_out = CZMath::sigmoid(node4_out * 0.6 + inputs[0] * 0.6 + inputs[1] * 0.2);
        node4_out = CZMath::sigmoid((node5_out * 0.4) + (inputs[0] * 0.7) + (inputs[2] * 0.5));
        TEST_EQ(node4_out, output[0]);
    }

    // self-loop
    node5_out = 0;
    node4_out = 0;
    std::vector<GraphD::EdgeCreate> self_loop_edges {
        {0, 3, 0.7}, {2, 3, 0.5}, {1, 4, 0.2}, {4, 3, 0.4}, {0, 4, 0.6},
    {3, 4, 0.6}, {4, 4, 0.3}, {3, 3, 0.9}
    };
    GraphD self_loop_graph(vertices, self_loop_edges);
    for (int i = 0; i < iterations; i++) {
        output.clear();
        std::cout << "\nIteration: " << i << std::endl;
        self_loop_graph.calculate_outputs(inputs, output);
        node5_out =
            CZMath::sigmoid(node5_out * 0.3 + node4_out * 0.6 + inputs[0] * 0.6 + inputs[1] * 0.2);
        node4_out = CZMath::sigmoid((node4_out * 0.9) + (node5_out * 0.4) + (inputs[0] * 0.7) +
                                    (inputs[2] * 0.5));
        TEST_EQ(node4_out, output[0]);
    }
    return true;
}
*/
#undef NEURAL_NDEBUG