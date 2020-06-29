#include "../../test_utils/test_utils.hpp"
#include "../neat.hpp"
bool phenotype_showcase() {
    PRINT_DEBUG_WARNINGS("Phenotype's NeuronType differs from NeuralNetwork's. Consider "
                         "rewriting this test case.");
    Phenotype<>::NewNeurons neurons{
        {NEAT::INPUT}, {NEAT::INPUT}, {NEAT::INPUT}, {NEAT::OUTPUT}, {NEAT::HIDDEN}};
    Phenotype<>::NewLinks links{{0, 3, 0.7}, {2, 3, 0.5}, {1, 4, 0.2},
                                {4, 3, 0.4}, {0, 4, 0.6}, {3, 4, 0.6}}; // 3, 4 and 4,3: recurrent
    // 3 inputs, 1 hidden, 1 output, where the hidden gets LSTM info from output node
    Phenotype<>::NGraph recurrent_pheno(neurons, links);
    const Phenotype<>::FPVector inputs{1.4, 5.8, 12.2};
    Phenotype<>::FPVector outputs;
    // "Ground-truth" calculations
    Phenotype<>::FP node5_out = 0;
    Phenotype<>::FP node4_out = 0;
    auto compute_groundtruth = [&]() {
        node5_out = CZMath::sigmoid(node4_out * 0.6 + inputs[0] * 0.6 + inputs[1] * 0.2);
        node4_out = CZMath::sigmoid(node5_out * 0.4 + inputs[0] * 0.7 + inputs[2] * 0.5);
        return node4_out;
    };
    constexpr int iterations = 5;
    for (int i = 0; i < iterations; i++) {
        outputs.clear();
        std::cout << "\nIteration: " << i << std::endl;
        recurrent_pheno.calculate_outputs(inputs, outputs);
        TEST_EQ(compute_groundtruth(), outputs[0]);
    }
    return true;
}

bool innovation_database_constructor_test() {
    Genotype<> genome_innovs;
    const int n_inp_outp[] = {3, 1, 50, 10};
    for (int test = 0; test < Common::array_size(n_inp_outp); test++) {
        genome_innovs.clear();
        const int& n_inputs = n_inp_outp[test];
        const int& n_outputs = n_inp_outp[test += 1];
        NEAT::InnovationDatabase<> db(n_inputs, n_outputs, genome_innovs);
        Genotype<>::InnovationLinksChain innov_chain = genome_innovs.link_chain();
        TEST_EQ(innov_chain.size(), n_inputs * n_outputs);
        // Test innovation for genomes
        int i = 0;
        for (InnovationChain<>::iterator g_innov = innov_chain.begin();
             g_innov != innov_chain.end(); ++g_innov, i++) {
            InnovationLink<>& innov_link = genome_innovs[g_innov];
            // Test that the input nodes are inputs, and outputs are outputs
            InnovationInfo<>& db_innov_info = db.innov_at(innov_link);
            TEST_EQ(db.neurtype_at(db_innov_info.in_node), NEAT::NeuronType::INPUT);
            TEST_EQ(db.neurtype_at(db_innov_info.out_node), NEAT::NeuronType::OUTPUT);
            TEST_EQ(innov_link.is_enabled(), true);
            TEST_EQ(innov_link.innovation_number(), i);
            // Check if toggle_enabled works
            innov_link.toggle_enabled();
            TEST_EQ(innov_link.is_enabled(), false);
            TEST_EQ(innov_link.innovation_number(), i);
            // Check if toggle_enabled can toggle false back to true.
            innov_link.toggle_enabled();
            TEST_EQ(innov_link.is_enabled(), true);
            TEST_EQ(innov_link.innovation_number(), i);
            // Check if set_enabled NOP if true.
            innov_link.set_enabled(true);
            TEST_EQ(innov_link.is_enabled(), true);
            TEST_EQ(innov_link.innovation_number(), i);
            // Check if set_enabled sets to false if false
            innov_link.set_enabled(false);
            TEST_EQ(innov_link.is_enabled(), false);
            TEST_EQ(innov_link.innovation_number(), i);
            // Check if set_enabled NOP if false
            innov_link.set_enabled(false);
            TEST_EQ(innov_link.is_enabled(), false);
            TEST_EQ(innov_link.innovation_number(), i);
            // Check if set_enabled sets from false to true
            innov_link.set_enabled(true);
            TEST_EQ(innov_link.is_enabled(), true);
            TEST_EQ(innov_link.innovation_number(), i);
        }
    }

    return true;
}
bool innov_info_op_test() {
    InnovationInfo<> info_temp = {0, 4};
    InnovationInfo<> info_temp_eq = {0, 4};
    InnovationInfo<> info_temp_ne = {1, 4};
    InnovationInfo<> info_temp_ne1 = {0, 3};
    TEST_EQ(info_temp, info_temp_eq);
    TEST_NE(info_temp, info_temp_ne);
    TEST_NE(info_temp, info_temp_ne1);
    TEST_EQ(NEAT::innov_info_hash_fn()(info_temp), NEAT::innov_info_hash_fn()(info_temp_eq));
    return true;
}
bool innov_db_mutation_test() {
    Genotype<> genotype_seed;
    const int n_ins = 3;
    const int n_outs = 1;
    // inputs: [0, 3); bias: [3, 4), output: [4, 5)
    AkashicRecords<> innov_records(n_ins, n_outs, genotype_seed);
    Genotype<> peer_genotype = genotype_seed;
    // Add a new connection
    // Huh, what happens if all nodes had a connection already? it should not add
    // another connection because we used emplace_s.
    innov_records.add_connection_innovation(0, 4, genotype_seed);
    // should not add new connection.
    TEST_EQ(genotype_seed.c_link_chain().size(), n_ins * n_outs);
    TEST_EQ(innov_records.innov_db.size(), n_ins * n_outs);
    // however... if we add a recurrent node, it should add a new innovation to
    // the record, and to the genotype.
    innov_records.add_connection_innovation(4, 0, genotype_seed);
    TEST_EQ(innov_records.innov_db.size(), (n_ins * n_outs) + 1);
    TEST_EQ(genotype_seed.c_link_chain().size(), (n_ins * n_outs) + 1);
    // Nothing should be added from to peer.
    TEST_EQ(peer_genotype.innov_size(), n_ins * n_outs);
    // Now, the peer should have another node
    int peer_node_innov = peer_genotype.innov_size() - 1;
    innov_records.add_node_between(peer_node_innov, peer_genotype);
    TEST_EQ(peer_genotype.innov_size(), n_ins * n_outs + 2);
    InnovationLink<> peer_innovlink = peer_genotype[peer_node_innov];
    // adding the node should disable this innovation.
    TEST_EQ(peer_innovlink.is_enabled(), false);
    return true;
}
int main() {
    TEST_INITIALIZE;
    DO_TEST(phenotype_showcase);
    DO_TEST(innovation_database_constructor_test);
    DO_TEST(innov_info_op_test);
    DO_TEST(innov_db_mutation_test);
    TEST_RESULTS;
    return 0;
}