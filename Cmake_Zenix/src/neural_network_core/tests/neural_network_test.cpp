#include <iostream>
#include "../../common/assertion.hpp"
#include "../../test_utils/test_utils.hpp"
#include "../neural_network_core.hpp"
#include "open_cl_compute_test.hpp"
#include "volatile_neural_graph.hpp"

bool graph_initial_test() {
    // http:// nn.cs.utexas.edu/downloads/papers/stanley.ec02.pdf pg9:
    // Nodes: 1 (in), 2(in), 3(in), 4(out), 5(hidden)
    // Links: 1->4 (0.7), 3->4(0.5), 2->5(0.2), 5->4(0.4), 1->5(0.6),
    // 4->5(0.6)
    // Create the vertices first
    GraphD::Vertex node1(NeuronType::INPUT);
    TEST_MSG(node1.activation_response == 1.0, "node1 act_rsp is not 1 by default");
    TEST_(node1.neuron_type == NeuronType::INPUT);
    TEST_(node1.last_output == 0.0);
    GraphD::Vertex node2(NeuronType::INPUT);
    TEST_(node2.activation_response == 1.0);
    TEST_(node2.neuron_type == NeuronType::INPUT);
    TEST_(node2.last_output == 0.0);
    GraphD::Vertex node3(NeuronType::INPUT, 0.7);
    TEST_(node3.activation_response == 0.7);
    TEST_(node3.neuron_type == NeuronType::INPUT);
    TEST_(node3.last_output == 0.0);
    GraphD::Vertex node4(NeuronType::OUTPUT);
    TEST_(node4.activation_response == 1.0);
    TEST_(node4.neuron_type == NeuronType::OUTPUT);
    TEST_(node4.last_output == 0.0);
    GraphD::Vertex node5(NeuronType::HIDDEN);
    TEST_(node5.activation_response == 1.0);
    TEST_(node5.neuron_type == NeuronType::HIDDEN);
    TEST_(node5.last_output == 0.0);
    GraphD::VertexList vertices;
    vertices.push_back(node1);
    vertices.push_back(node2);
    vertices.push_back(node3);
    vertices.push_back(node4);
    vertices.push_back(node5);
    GraphD::VertexList unsorted_copy = vertices;
    GraphD::rearrange(vertices);
    GraphD rearranged(vertices, 3, false);
    GraphD unsorted(unsorted_copy, 3, false);
    std::string unsorted_str = "Unsorted: ";
    unsorted.append_str(unsorted_str);
    std::string rearranged_str = "Rearranged: ";
    rearranged.append_str(rearranged_str);
    std::cout << unsorted_str << std::endl;
    std::cout << rearranged_str << std::endl;
    // TEST_(vertices != unsorted_copy);
    // Graph<> graph;
    return true;
}
bool vertex_list_refs_test() {
    GraphD::VertexList vertices;
#define MAKE_NODE(shorthand_type) vertices.push_back(GraphD::Vertex(NeuronType::##shorthand_type))
    MAKE_NODE(INPUT);
    MAKE_NODE(INPUT);
    MAKE_NODE(INPUT);
    MAKE_NODE(OUTPUT);
    MAKE_NODE(HIDDEN);
#undef MAKE_NODE
    // Reference to the first node:
    GraphD::Vertex& node1 = vertices[0];
    node1.last_output = 0.4420;
    TEST_(node1.last_output == vertices[0].last_output);
    // A list that supposedly contains pointer to all elements from vertices
    std::vector<GraphD::Vertex*> vertices_ptrs;
    for (int i = 0; i < vertices.size(); i++) {
        vertices_ptrs.push_back(&vertices[i]);
    }
    double new_output = 10.67;
    for (int i = 0; i < vertices_ptrs.size(); i++) {
        vertices_ptrs[i]->last_output = new_output;
    }
    int i = 0;
    TEST_(vertices_ptrs[i]->last_output == new_output &&
          vertices[i].last_output == vertices_ptrs[i]->last_output);
    i = 1;
    TEST_(vertices_ptrs[i]->last_output == new_output &&
          vertices[i].last_output == vertices_ptrs[i]->last_output);
    i = 2;
    TEST_(vertices_ptrs[i]->last_output == new_output &&
          vertices[i].last_output == vertices_ptrs[i]->last_output);
    i = 3;
    TEST_(vertices_ptrs[i]->last_output == new_output &&
          vertices[i].last_output == vertices_ptrs[i]->last_output);
    i = 4;
    TEST_(vertices_ptrs[i]->last_output == new_output &&
          vertices[i].last_output == vertices_ptrs[i]->last_output);
    return true;
}
bool vertex_list_move_test() {
    GraphD::VertexList vertices;
#define MAKE_NODE(shorthand_type) vertices.push_back(GraphD::Vertex(NeuronType::##shorthand_type))

#undef MAKE_NODE
    return true;
}
bool graph_output_test() {
    // http:// nn.cs.utexas.edu/downloads/papers/stanley.ec02.pdf pg9:
    // Nodes: 1 (in), 2(in), 3(in), 4(out), 5(hidden)
    // Links: 1->4 (0.7), 3->4(0.5), 2->5(0.2), 5->4(0.4), 1->5(0.6),
    // 4->5(0.6)

    GraphD::VertexList vertices;
#define MAKE_NODE(shorthand_type) vertices.push_back(GraphD::Vertex(NeuronType::##shorthand_type))
    MAKE_NODE(INPUT);
    MAKE_NODE(INPUT);
    MAKE_NODE(INPUT);
    MAKE_NODE(OUTPUT);
    MAKE_NODE(HIDDEN);
#undef MAKE_NODE
    GraphD graph(std::move(vertices), 3, false);
    // GraphD::VertexList& graph_vert_ref = graph.
    GraphD::Vertex::ArrowList edges;
    edges.reserve(16);
    int edge_idx = 0;
    //#define MAKE_LINK(in_num, out_num, weight)                                                         \
//    edges.push_back(new GraphD::Edge(&vertices[in_num - 1], &vertices[out_num - 1], weight));      \
//    vertices[in_num - 1].outwards_arrows.push_back((edges[edge_idx]));                             \
//    vertices[out_num - 1].inwards_arrows.push_back((edges[edge_idx]));                             \
//    edge_idx += 1;
    std::vector<GraphD::Edge> all_edges;
    std::vector<GraphD::EdgeCreate> instructions;
#define MAKE_LINK(in_num, out_num, weight)                                                         \
    instructions.emplace_back(in_num - 1, out_num - 1, weight, false)
    MAKE_LINK(1, 4, 0.7);
    MAKE_LINK(3, 4, 0.5);
    MAKE_LINK(2, 5, 0.2);
    MAKE_LINK(5, 4, 0.4);
    MAKE_LINK(1, 5, 0.6);
    graph.connect_self_vertices(instructions, all_edges);
    graph.rearrange_self();
    // POTENTIAL_BUG_POINT_MSG("Rearrange messes up the original ordering of vertices!");
#undef MAKE_LINK
    // POTENTIAL_BUG_POINT_MSG("Vertices is not being moved, it's copied.");
    //// POTENTIAL_BUG_POINT_MSG("edges do not connect vertices together?");
    // POTENTIAL_BUG_POINT_MSG("The location of vertices is moved to graph, while edges are still "
    //                        "pointing to old vertices");

    std::vector<double> inputs{1.4, 5.8, 12.2};
    std::vector<double> outputs;
    graph.calculate_outputs(inputs, outputs);
    POTENTIAL_BUG_POINT_MSG("calculate_outputs failed to count node5's output.");
    double node5_out = CZMath::sigmoid(inputs[0] * 0.6 + inputs[1] * 0.2);
    double expected_out =
        CZMath::sigmoid((node5_out * 0.4) + (inputs[0] * 0.7) + (inputs[2] * 0.5));
    std::cout << NeuralNetwork::to_string(graph) << std::endl;
    std::cout << "\nExpected out: " << Common::ryu_d2s(expected_out) << std::endl;
    std::cout << "Node5 output: " << Common::ryu_d2s(node5_out) << std::endl;

    std::cout << " Output: " << Common::ryu_d2s(outputs[0]) << std::endl;
    std::cout << std::endl;
    for (int i = 0; i < edges.size(); i++) {
        std::cout << NeuralNetwork::to_string(*edges[i]->in) << NeuralNetwork::to_string(*edges[i])
                  << NeuralNetwork::to_string(*edges[i]->out) << std::endl;
    }
    std::cout << "Vertices:" << std::endl;
    for (int i = 0; i < vertices.size(); i++) {
        std::cout << NeuralNetwork::to_string(vertices[i]) << std::endl;
    }

    TEST_(expected_out == outputs[0]);
    return true;
}
bool graph_output_no_reassign_test() {
    // http:// nn.cs.utexas.edu/downloads/papers/stanley.ec02.pdf pg9:
    // Nodes: 1 (in), 2(in), 3(in), 4(out), 5(hidden)
    // Links: 1->4 (0.7), 3->4(0.5), 2->5(0.2), 5->4(0.4), 1->5(0.6),
    // 4->5(0.6)
    GraphD::VertexList vertices;
#define MAKE_NODE(shorthand_type) vertices.push_back(GraphD::Vertex(NeuronType::##shorthand_type))
    MAKE_NODE(INPUT);
    MAKE_NODE(INPUT);
    MAKE_NODE(INPUT);
    MAKE_NODE(HIDDEN);
    MAKE_NODE(OUTPUT);
#undef MAKE_NODE
    GraphD graph(std::move(vertices), 3, false);
    // GraphD::VertexList& graph_vert_ref = graph.
    GraphD::Vertex::ArrowList edges;
    edges.reserve(16);
    int edge_idx = 0;
    //#define MAKE_LINK(in_num, out_num, weight)                                                         \
//    edges.push_back(new GraphD::Edge(&vertices[in_num - 1], &vertices[out_num - 1], weight));      \
//    vertices[in_num - 1].outwards_arrows.push_back((edges[edge_idx]));                             \
//    vertices[out_num - 1].inwards_arrows.push_back((edges[edge_idx]));                             \
//    edge_idx += 1;
    std::vector<GraphD::Edge> all_edges;
    std::vector<GraphD::EdgeCreate> instructions;
#define MAKE_LINK(in_num, out_num, weight)                                                         \
    instructions.emplace_back(in_num - 1, out_num - 1, weight, false)
    MAKE_LINK(1, 5, 0.7);
    MAKE_LINK(3, 5, 0.5);
    MAKE_LINK(2, 4, 0.2);
    MAKE_LINK(4, 5, 0.4);
    MAKE_LINK(1, 4, 0.6);
    graph.connect_self_vertices(instructions, all_edges);
    graph.rearrange_self();
#undef MAKE_LINK
    std::vector<double> inputs{1.4, 5.8, 12.2};
    std::vector<double> outputs;
    graph.calculate_outputs(inputs, outputs);
    POTENTIAL_BUG_POINT_MSG("calculate_outputs failed to count node5's output.");
    double node5_out = CZMath::sigmoid(inputs[0] * 0.6 + inputs[1] * 0.2);
    double expected_out =
        CZMath::sigmoid((node5_out * 0.4) + (inputs[0] * 0.7) + (inputs[2] * 0.5));
    std::cout << NeuralNetwork::to_string(graph) << std::endl;
    std::cout << "\nExpected out: " << Common::ryu_d2s(expected_out) << std::endl;
    std::cout << "Node5 output: " << Common::ryu_d2s(node5_out) << std::endl;

    std::cout << " Output: " << Common::ryu_d2s(outputs[0]) << std::endl;
    std::cout << std::endl;
    for (int i = 0; i < edges.size(); i++) {
        std::cout << NeuralNetwork::to_string(*edges[i]->in) << NeuralNetwork::to_string(*edges[i])
                  << NeuralNetwork::to_string(*edges[i]->out) << std::endl;
    }
    std::cout << "Vertices:" << std::endl;
    for (int i = 0; i < vertices.size(); i++) {
        std::cout << NeuralNetwork::to_string(vertices[i]) << std::endl;
    }

    TEST_(expected_out == outputs[0]);
    return true;
}
bool graph_recurrent_test() {
    // http:// nn.cs.utexas.edu/downloads/papers/stanley.ec02.pdf pg9:
    // Nodes: 1 (in), 2(in), 3(in), 4(out), 5(hidden)
    // Links: 1->4 (0.7), 3->4(0.5), 2->5(0.2), 5->4(0.4), 1->5(0.6),
    // 4->5(0.6)
    GraphD::VertexList vertices;
#define MAKE_NODE(shorthand_type) vertices.push_back(GraphD::Vertex(NeuronType::##shorthand_type))
    MAKE_NODE(INPUT);
    MAKE_NODE(INPUT);
    MAKE_NODE(INPUT);
    MAKE_NODE(HIDDEN);
    MAKE_NODE(OUTPUT);
#undef MAKE_NODE
    GraphD graph(std::move(vertices), 3, false);
    // GraphD::VertexList& graph_vert_ref = graph.
    GraphD::Vertex::ArrowList edges;
    edges.reserve(16);
    int edge_idx = 0;
    //#define MAKE_LINK(in_num, out_num, weight)                                                         \
//    edges.push_back(new GraphD::Edge(&vertices[in_num - 1], &vertices[out_num - 1], weight));      \
//    vertices[in_num - 1].outwards_arrows.push_back((edges[edge_idx]));                             \
//    vertices[out_num - 1].inwards_arrows.push_back((edges[edge_idx]));                             \
//    edge_idx += 1;
    std::vector<GraphD::Edge> all_edges;
    std::vector<GraphD::EdgeCreate> instructions;
#define MAKE_LINK(in_num, out_num, weight)                                                         \
    instructions.emplace_back(in_num - 1, out_num - 1, weight, false)
    MAKE_LINK(1, 5, 0.7);
    MAKE_LINK(3, 5, 0.5);
    MAKE_LINK(2, 4, 0.2);
    MAKE_LINK(4, 5, 0.4);
    MAKE_LINK(1, 4, 0.6);
    MAKE_LINK(5, 4, 0.6);
    graph.connect_self_vertices(instructions, all_edges);
    graph.rearrange_self();
#undef MAKE_LINK
    double node5_out = 0;
    double node4_out = 0;
    std::vector<double> inputs{1.4, 5.8, 12.2};
    std::vector<double> outputs;
    const int iterations = 5; // test passed with 100, however, std out got flooded.
    for (int i = 0; i < iterations; i++) {
        outputs.clear();
        std::cout << "\nIteration: " << i << std::endl;
        graph.calculate_outputs(inputs, outputs);
        POTENTIAL_BUG_POINT_MSG("calculate_outputs failed to count node5's output.");
        node5_out = CZMath::sigmoid(node4_out * 0.6 + inputs[0] * 0.6 + inputs[1] * 0.2);
        node4_out = CZMath::sigmoid((node5_out * 0.4) + (inputs[0] * 0.7) + (inputs[2] * 0.5));
        std::cout << NeuralNetwork::to_string(graph) << std::endl;
        std::cout << "\nExpected out: " << Common::ryu_d2s(node4_out) << std::endl;
        std::cout << "Node5 output: " << Common::ryu_d2s(node5_out) << std::endl;

        std::cout << " Output: " << Common::ryu_d2s(outputs[0]) << std::endl;
        std::cout << std::endl;
        for (int i = 0; i < edges.size(); i++) {
            std::cout << NeuralNetwork::to_string(*edges[i]->in)
                      << NeuralNetwork::to_string(*edges[i])
                      << NeuralNetwork::to_string(*edges[i]->out) << std::endl;
        }
        std::cout << "Vertices:" << std::endl;
        for (int i = 0; i < vertices.size(); i++) {
            std::cout << NeuralNetwork::to_string(vertices[i]) << std::endl;
        }
        TEST_(node4_out == outputs[0]);
    }
    return true;
}
bool graph_self_loop_test() {
    GraphD::VertexList vertices;
#define MAKE_NODE(shorthand_type) vertices.push_back(GraphD::Vertex(NeuronType::##shorthand_type))
    MAKE_NODE(INPUT);
    MAKE_NODE(INPUT);
    MAKE_NODE(INPUT);
    MAKE_NODE(HIDDEN);
    MAKE_NODE(OUTPUT);
#undef MAKE_NODE
    GraphD graph(std::move(vertices), 3, false);
    // GraphD::VertexList& graph_vert_ref = graph.
    GraphD::Vertex::ArrowList edges;
    edges.reserve(16);
    int edge_idx = 0;
    //#define MAKE_LINK(in_num, out_num, weight)                                                         \
//    edges.push_back(new GraphD::Edge(&vertices[in_num - 1], &vertices[out_num - 1], weight));      \
//    vertices[in_num - 1].outwards_arrows.push_back((edges[edge_idx]));                             \
//    vertices[out_num - 1].inwards_arrows.push_back((edges[edge_idx]));                             \
//    edge_idx += 1;
    std::vector<GraphD::Edge> all_edges;
    std::vector<GraphD::EdgeCreate> instructions;
#define MAKE_LINK(in_num, out_num, weight)                                                         \
    instructions.emplace_back(in_num - 1, out_num - 1, weight, false)
    MAKE_LINK(1, 5, 0.7);
    MAKE_LINK(3, 5, 0.5);
    MAKE_LINK(2, 4, 0.2);
    MAKE_LINK(4, 5, 0.4);
    MAKE_LINK(1, 4, 0.6);
    MAKE_LINK(5, 4, 0.6);
    // Self-loop on hidden
    MAKE_LINK(4, 4, 0.3);
    // Self-loop on output
    MAKE_LINK(5, 5, 0.9);
    graph.connect_self_vertices(instructions, all_edges);
    graph.rearrange_self();
#undef MAKE_LINK
    double node5_out = 0;
    double node4_out = 0;
    std::vector<double> inputs{1.4, 5.8, 12.2};
    std::vector<double> outputs;
    const int iterations = 5; // test passed with 100, however, std out got flooded.
    for (int i = 0; i < iterations; i++) {
        outputs.clear();
        std::cout << "\nIteration: " << i << std::endl;
        graph.calculate_outputs(inputs, outputs);
        POTENTIAL_BUG_POINT_MSG("calculate_outputs failed to count node5's output.");
        node5_out =
            CZMath::sigmoid(node5_out * 0.3 + node4_out * 0.6 + inputs[0] * 0.6 + inputs[1] * 0.2);
        node4_out = CZMath::sigmoid((node4_out * 0.9) + (node5_out * 0.4) + (inputs[0] * 0.7) +
                                    (inputs[2] * 0.5));
        std::cout << NeuralNetwork::to_string(graph) << std::endl;
        std::cout << "\nExpected out: " << Common::ryu_d2s(node4_out) << std::endl;
        std::cout << "Node5 output: " << Common::ryu_d2s(node5_out) << std::endl;

        std::cout << " Output: " << Common::ryu_d2s(outputs[0]) << std::endl;
        std::cout << std::endl;
        for (int i = 0; i < edges.size(); i++) {
            std::cout << NeuralNetwork::to_string(*edges[i]->in)
                      << NeuralNetwork::to_string(*edges[i])
                      << NeuralNetwork::to_string(*edges[i]->out) << std::endl;
        }
        std::cout << "Vertices:" << std::endl;
        for (int i = 0; i < vertices.size(); i++) {
            std::cout << NeuralNetwork::to_string(vertices[i]) << std::endl;
        }
        TEST_(node4_out == outputs[0]);
    }
    return true;
}
bool graph_safe_ctor_test() {
    // http:// nn.cs.utexas.edu/downloads/papers/stanley.ec02.pdf pg9:
    // Nodes: 1 (in), 2(in), 3(in), 4(out), 5(hidden)
    // Links: 1->4 (0.7), 3->4(0.5), 2->5(0.2), 5->4(0.4), 1->5(0.6),
    // 4->5(0.6)
    std::vector<GraphD::VertexCreate> vertices{
        GraphD::VertexCreate(NeuronType::INPUT), GraphD::VertexCreate(NeuronType::INPUT),
        GraphD::VertexCreate(NeuronType::INPUT), GraphD::VertexCreate(NeuronType::OUTPUT),
        GraphD::VertexCreate(NeuronType::HIDDEN)};
    std::vector<double> inputs{1.4, 5.8, 12.2};
    std::vector<double> output;
    // One edge test:
    std::vector<GraphD::EdgeCreate> one_edge{{0, 3, 0.7}};
    GraphD graph_one_edge(vertices, one_edge, 2);
    graph_one_edge.calculate_outputs(inputs, output);
    TEST_EQ(output[0], CZMath::sigmoid(1.4 * 0.7));

    // Quite complete test:
    output.clear();
    std::vector<GraphD::EdgeCreate> some_edges{
        {0, 3, 0.7}, {2, 3, 0.5}, {1, 4, 0.2}, {4, 3, 0.4}, {0, 4, 0.6}};
    GraphD graph_some_edges(vertices, some_edges, 3);
    graph_some_edges.calculate_outputs(inputs, output);
    TEST_EQ(output.size(), 1);
    double node5_out = CZMath::sigmoid(inputs[0] * 0.6 + inputs[1] * 0.2);
    double expected_out =
        CZMath::sigmoid((node5_out * 0.4) + (inputs[0] * 0.7) + (inputs[2] * 0.5));
    TEST_EQ(output[0], expected_out);
    return true;
}
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
    std::vector<GraphD::EdgeCreate> self_loop_edges{{0, 3, 0.7}, {2, 3, 0.5}, {1, 4, 0.2},
                                                    {4, 3, 0.4}, {0, 4, 0.6}, {3, 4, 0.6},
                                                    {4, 4, 0.3}, {3, 3, 0.9}};
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
bool graph_synchronize_test() {
    std::vector<GraphD::VertexCreate> vertices{
        GraphD::VertexCreate(NeuronType::INPUT), GraphD::VertexCreate(NeuronType::INPUT),
        GraphD::VertexCreate(NeuronType::INPUT), GraphD::VertexCreate(NeuronType::OUTPUT),
        GraphD::VertexCreate(NeuronType::HIDDEN)};
    const std::vector<double> inputs{1.4, 5.8, 12.2};
    const int iterations = 5;
    double node5_out = 0;
    double node4_out = 0;

    // recurrent:
    std::vector<GraphD::EdgeCreate> recurrent_edges{{0, 3, 0.7}, {2, 3, 0.5}, {1, 4, 0.2},
                                                    {4, 3, 0.4}, {0, 4, 0.6}, {3, 4, 0.6}};
    GraphD recurrent_graph(vertices, recurrent_edges);
    std::vector<double*> graph_inputs;
    recurrent_graph.input_addresses(graph_inputs);
    std::vector<double*> output;
    recurrent_graph.output_addresses(output);
    *graph_inputs[0] = inputs[0];
    *graph_inputs[1] = inputs[1];
    *graph_inputs[2] = inputs[2];
    for (int i = 0; i < iterations; i++) {
        std::cout << "\nIteration: " << i << std::endl;
        recurrent_graph.update();
        node5_out = CZMath::sigmoid(node4_out * 0.6 + inputs[0] * 0.6 + inputs[1] * 0.2);
        node4_out = CZMath::sigmoid((node5_out * 0.4) + (inputs[0] * 0.7) + (inputs[2] * 0.5));
        TEST_EQ(node4_out, *output[0]);
    }
    
    return true;
}

int neural_network_test() {
    TEST_INITIALIZE;
    DO_TEST(graph_initial_test);
    DO_TEST(vertex_list_refs_test);
    DO_TEST(vertex_list_move_test);
    DO_TEST(graph_output_test);
    DO_TEST(graph_output_no_reassign_test);
    DO_TEST(graph_recurrent_test);
    DO_TEST(graph_self_loop_test);
    DO_TEST(graph_safe_ctor_test);
    DO_TEST(graph_safe_ctor_recurrent_self_loop_test);
    DO_TEST(graph_synchronize_test);
    TEST_RESULTS;
    return 1;
}
int main() {
    neural_network_test();
    std::cout << "\n\n\n\n\n\nOpenCL test" << std::endl;
    open_cl_compute_test();
    std::cout << "\n\n\n\n\n\nVolatile_Neural_Graph test" << std::endl;
    volatile_neural_graph_test();
    return 1;
}