#pragma once
#include "../../common/common.hpp"
#include "../../test_utils/test_utils.hpp"
#include "../neural_network_core.hpp"

bool vgraph_constructable_test() {
    // enum NeuronType : unsigned char { INPUT = 0, BIAS = 1, HIDDEN = 2, OUTPUT = 3, UNDEFINED = 4
    // }; 3 inputs, 1 output, 1 hidden.
    auto end_arr = VGraphD::make_end_array(3, 1, 1, 1);
    VGraphD graph;
    graph.make_vertices(end_arr);
    TEST_EQ(graph.get_vertex(0).inwards_edges.size(), 0);
    TEST_EQ(graph.get_vertex(0).outwards_edges.size(), 0);
    TEST_EQ(graph.get_vertex(0).neuron_type, INPUT);
    TEST_EQ(graph.get_vertex(0).last_output, 0);
    TEST_EQ(graph.get_vertex(0).response, 1);

    // in0 -> out0 (0.7)
    // in2 -> out0 (0.5)
    // in1 -> hidden0 (0.2)
    // hidden0 -> out0 (0.4)
    // in0 -> hidden0 (0.6)
    // out0 -> hidden0 (0.6)
    int i0o0 = graph.make_edge(VGraphD::get_vertex_id(0, INPUT, end_arr),
                               VGraphD::get_vertex_id(0, OUTPUT, end_arr), 0.7);
    int i2o0 = graph.make_edge(VGraphD::get_vertex_id(2, INPUT, end_arr),
                               VGraphD::get_vertex_id(0, OUTPUT, end_arr), 0.5);
    int i1h0 = graph.make_edge(VGraphD::get_vertex_id(1, INPUT, end_arr),
                               VGraphD::get_vertex_id(0, HIDDEN, end_arr), 0.2);
    int h0o0 = graph.make_edge(VGraphD::get_vertex_id(0, HIDDEN, end_arr),
                               VGraphD::get_vertex_id(0, OUTPUT, end_arr), 0.4);
    int i0h0 = graph.make_edge(VGraphD::get_vertex_id(0, INPUT, end_arr),
                               VGraphD::get_vertex_id(0, HIDDEN, end_arr), 0.6);
    int o0h0 = graph.make_edge(VGraphD::get_vertex_id(0, OUTPUT, end_arr),
                               VGraphD::get_vertex_id(0, HIDDEN, end_arr), 0.6);
    TEST_EQ(graph.get_edge(i0o0).weight, 0.7);
    TEST_EQ(graph.get_edge(i2o0).weight, 0.5);
    TEST_EQ(graph.get_edge(i1h0).weight, 0.2);
    TEST_EQ(graph.get_edge(h0o0).weight, 0.4);
    TEST_EQ(graph.get_edge(i0h0).weight, 0.6);
    TEST_EQ(graph.get_edge(o0h0).weight, 0.6);
    TEST_EQ(graph.edge2vertex(i0o0, VGraphD::SOURCE_VERTEX).neuron_type, INPUT);
    TEST_EQ(graph.edge2vertex(i0o0, VGraphD::DEST_VERTEX).neuron_type, OUTPUT);
    TEST_EQ(graph.edge2vertex(i2o0, VGraphD::SOURCE_VERTEX).neuron_type, INPUT);
    TEST_EQ(graph.edge2vertex(i2o0, VGraphD::DEST_VERTEX).neuron_type, OUTPUT);
    TEST_EQ(graph.edge2vertex(i1h0, VGraphD::SOURCE_VERTEX).neuron_type, INPUT);
    TEST_EQ(graph.edge2vertex(i1h0, VGraphD::DEST_VERTEX).neuron_type, HIDDEN);
    TEST_EQ(graph.edge2vertex(h0o0, VGraphD::SOURCE_VERTEX).neuron_type, HIDDEN);
    TEST_EQ(graph.edge2vertex(h0o0, VGraphD::DEST_VERTEX).neuron_type, OUTPUT);
    TEST_EQ(graph.edge2vertex(i0h0, VGraphD::SOURCE_VERTEX).neuron_type, INPUT);
    TEST_EQ(graph.edge2vertex(i0h0, VGraphD::DEST_VERTEX).neuron_type, HIDDEN);
    TEST_EQ(graph.edge2vertex(o0h0, VGraphD::SOURCE_VERTEX).neuron_type, OUTPUT);
    TEST_EQ(graph.edge2vertex(o0h0, VGraphD::DEST_VERTEX).neuron_type, HIDDEN);
    return true;
}

bool vgraph_simple_output_test() {
    auto end_arr = VGraphD::make_end_array(3, 1, 1, 1);
    VGraphD graph;
    graph.make_vertices(end_arr);
    graph.make_edge(graph.get_vertex_id(0, INPUT, end_arr), graph.get_vertex_id(0, OUTPUT, end_arr),
                    0.7);
    graph.make_edge(graph.get_vertex_id(2, INPUT, end_arr), graph.get_vertex_id(0, OUTPUT, end_arr),
                    0.5);
    graph.make_edge(graph.get_vertex_id(1, INPUT, end_arr), graph.get_vertex_id(0, HIDDEN, end_arr),
                    0.2);
    graph.make_edge(graph.get_vertex_id(0, HIDDEN, end_arr),
                    graph.get_vertex_id(0, OUTPUT, end_arr), 0.4);
    graph.make_edge(graph.get_vertex_id(0, INPUT, end_arr), graph.get_vertex_id(0, HIDDEN, end_arr),
                    0.6);
    std::cout << "Test for unsync" << std::endl;
    std::vector<double> inputs{1.4, 5.8, 12.2};
    std::vector<double> outputs;
    graph.calculate_outputs(inputs, outputs);
    double hidden_out = CZMath::sigmoid(inputs[0] * 0.6 + inputs[1] * 0.2);
    double expected_out = CZMath::sigmoid(hidden_out * 0.4 + inputs[0] * 0.7 + inputs[2] * 0.5);
    TEST_EQ(expected_out, outputs[0]);
    graph.clear_vertex_outputs();
    std::cout << "Test for synchronized" << std::endl;
    for (int i = 0; i < inputs.size(); i++) {
        graph.set_input(i, inputs[i], end_arr);
    }
    graph.update();
    TEST_EQ(expected_out, graph.get_output(0, end_arr));
    return true;
}
int volatile_neural_graph_test() {
    TEST_INITIALIZE;
    DO_TEST(vgraph_constructable_test);
    DO_TEST(vgraph_simple_output_test);
    TEST_RESULTS;
    return 1;
}