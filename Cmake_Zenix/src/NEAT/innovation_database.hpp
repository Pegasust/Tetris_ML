#pragma once
#include <array>
#include "../common/assertion.hpp"
#include "genotype.hpp"
namespace NEAT {
// ===================================== INNOVATION_DATABASE =====================================
template <typename Index_Type = int, typename FP_Type = double, typename Neuron_Type = NeuronType>
class InnovationDatabase {
    using Database = std::vector<InnovationInfo<Index_Type>>;
    using AddLinkLookup =
        std::unordered_map<InnovationInfo<Index_Type>, Index_Type, innov_info_hash_fn>;
    using AddNodeLookup = std::unordered_map<Index_Type, std::array<Index_Type, 2>>;

public:
    Database innov_db;
    std::vector<GenomeNeuron<Neuron_Type>> genotype_neurons;
    // All of the add-node-mutations
    AddNodeLookup add_node_innov_lookup;
    AddLinkLookup add_link_innov_lookup;
    Index_Type type_start[Neuron_Type::UNDEFINED];
    inline Index_Type next_innov_idx() {
        return innov_db.size();
    }

protected:
    void add_node(const Neuron_Type type) {
        genotype_neurons.emplace_back(type);
    }
    void add_hidden_node() {
        add_node(Neuron_Type::HIDDEN);
    }
    /*
     * This function assumes genotype_neurons stores neurons of the same type
     * contiguously and the first element is not of type NeuronType::UNDEFINED.
     */
    void assign_type_start() {
        Neuron_Type cur_type = Neuron_Type::UNDEFINED;
        for (Index_Type i = 0; genotype_neurons.size(); i++) {
            if (genotype_neurons[i].type != cur_type) {
                cur_type = genotype_neurons[i].type;
                if (cur_type == Neuron_Type::UNDEFINED) {
                    // Hits undefined neuron type, just break to prevent accessing
                    // NeuronType::UNDEFINED.
                    break;
                }
                type_start[cur_type] = i;
                if (cur_type == Neuron_Type::HIDDEN) {
                    // We're done since HIDDEN would store contiguously until end.
                    break;
                }
            }
        }
    }
    // Unsafely add new connection innovation to database and lookup
    void add_new_innovation(InnovationInfo<Index_Type>& innov) {
        add_link_innov_lookup[innov] = next_innov_idx();
        innov_db.push_back(std::move(innov));
    }

public:
    bool genotype_has_link(const Index_Type neuron_from, const Index_Type neuron_to,
                           const Genotype<Index_Type, FP_Type>& genotype) {
        InnovationInfo<Index_Type> innov = {from, to};
        typename AddLinkLookup::const_iterator prev_innov = add_link_innov_lookup.find(innov);
        if (prev_innov == add_link_innov_lookup.end()) {
            // There is no innovation of this in the record, genotype cannot have this.
            return false;
        }
        // check if genotype has this innovation
        Index_Type innov_num = prev_innov->second;
        return !genotype.not_exist(innov_num);
    }
    /*
     * Declares that a genome attempts to "innovate" a new link from @param from
     * to @param to. If this link has already been innovated by any species in this
     * database, or add_to_genome_innovs is set to false, the innovation is not added
     * (nor is it overwritten by the status of enabled). Either the link has already
     * been innovated, @param genome_innovs will have the innovation added, unless the
     * innovation has already existed in genome_innovs.
     */
    template <bool add_to_genome_innovs = true>
    void add_connection_innovation(const Index_Type from, const Index_Type to,
                                   Genotype<Index_Type, FP_Type>& genome_innovs) {
        PRINT_DEBUG_WARNINGS("Untested func");
        InnovationInfo<Index_Type> innov = {from, to};
        typename AddLinkLookup::const_iterator previous_innov = add_link_innov_lookup.find(innov);
        Index_Type genome_innov;
        if (previous_innov == add_link_innov_lookup.end()) {
            // Not existed
            // The index after this innovation is added will be size before it is added.
            if (add_to_genome_innovs) {
                genome_innov = innov_db.size();
            }
            add_new_innovation(innov);
        } else if (add_to_genome_innovs) {
            genome_innov = previous_innov->second;
        }
        if (add_to_genome_innovs) {
            // emplace iff !exist
            genome_innovs.emplace_s(genome_innov);
        }
    }
    /* Add 2 innovations regarding adding a new hidden node between the link
     * specified by @param innovation_num. This is the add node mutation. The 2
     * innovations. The two newly added innovations will be added to @param genome
     * (enabled by default). This method will also disable its innovation history that
     * contains @param innovation_num.
     * I acknowledge the weirdness of a recurrent innovation, and decided the
     * recurrent link will be treated as a distinct link from this innovation link.
     * Imagine a structure of graph that transcends beyond 2D, where current links
     * flow in the given 2D, and the recurrent links flow in another cross-section
     * of this structure, but is parallel with the current links dimensions. This is
     * to say, the structure can decide to whether add a new node on recurrent links
     * separately.
     */
    void add_node_between(const Index_Type innovation_num,
                          Genotype<Index_Type, FP_Type>& genome_innovs) {
        PRINT_DEBUG_WARNINGS("Untested func");
        using AddNodeLookupIter = typename AddNodeLookup::iterator;
        AddNodeLookupIter previous_innov = add_node_innov_lookup.find(innovation_num);
        if (previous_innov == add_node_innov_lookup.end()) { // Not exist yet.
            // The newly added node will at the index of genotype_neurons.size().
            Index_Type new_node_index = genotype_neurons.size();
            // Add a new node to the records
            add_hidden_node();
            // incoming -> new node, new node -> outcoming
            // Aliasing is kind of broken with MSVC. incoming and outcoming gets broken
            // after calling to add_connection_innovation (maybe has something to do
            // with stack frame?
            const Index_Type incoming = innov_db[innovation_num].in_node;
            const Index_Type outcoming = innov_db[innovation_num].out_node;
            add_connection_innovation(incoming, new_node_index, genome_innovs);
            add_connection_innovation(new_node_index, outcoming, genome_innovs);
            // add_node_innov_lookup[innovation_num] ={static_cast<Index_Type>(innov_db.size() - 2),
            //                    static_cast<Index_Type>(innov_db.size() - 1)};
            std::array<Index_Type, 2> arr = {static_cast<Index_Type>(innov_db.size() - 2),
                                             static_cast<Index_Type>(innov_db.size() - 1)};
            CAN_BE_OPTIMIZED_MSG(
                "We can possibly optimize this by having a structure that "
                "can emplace instead of creating this array and copying into lookup");
            add_node_innov_lookup[innovation_num] = (arr);
        } else { // already existed.
            // const std::array<Index_Type,2> & innovs = previous_innov->second;
            genome_innovs.emplace_s(previous_innov->second[0]);
            genome_innovs.emplace_s(previous_innov->second[1]);
        }
        // Disable the old connection because there is a new node added between it.
        typename Genotype<Index_Type, FP_Type>::iterator old_connection_loc =
            genome_innovs.locate(innovation_num);
        ASSERT(!genome_innovs.qnot_exist(old_connection_loc, innovation_num),
               "Attempting to add node between not-yet-existed connection innov.");
        ASSERT(genome_innovs[old_connection_loc].is_enabled(),
               "Attempting to add node between disabled connection innov.");
        genome_innovs.enable_unsafe(old_connection_loc;)
    }

public:
    // ================================== NODE_NUM_CTOR ======================================
    // How many inputs, outputs, and biases are we adding to this database. The numbers are
    // const throughout a problem. @param connect_all determines whether all inputs should
    // be connected to outputs (which is on by classic NEAT, but off on Feature-Select (FS)
    // NEAT. This also adds the innovations to @param genome_innovs (FS-NEAT will not add).
    InnovationDatabase(const Index_Type n_inputs, const Index_Type n_outputs,
                       Genotype<Index_Type, FP_Type>& genome_innovs, const bool connect_all = true,
                       const Index_Type n_biases = 1)
        : innov_db(), genotype_neurons(), add_node_innov_lookup(),
          add_link_innov_lookup(), type_start{0, n_inputs, n_inputs + n_biases,
                                              n_inputs + n_biases + n_outputs} {
        // input, bias, output
        ASSERT(n_inputs > 0 && n_outputs > 0, "Attempting to make an innovation database with "
                                              "non-positive integer.");
        // for (int i = 0; i < n_inputs; i++) {
        //    add_node(Neuron_Type::INPUT);
        //}
        // for (int i = 0; i < n_biases; i++) {
        //    add_node(Neuron_Type::BIAS);
        //}
        // for (int i = 0; i < n_outputs; i++) {
        //    add_node(Neuron_Type::OUTPUT);
        //}
        for (Index_Type n = 0; n < Neuron_Type::HIDDEN; n++) {
            Index_Type& start = type_start[n];
            Index_Type& end = type_start[n + 1];
            for (Index_Type i = start; i < end; i++) {
                add_node(static_cast<Neuron_Type>(n));
            }
        }
        if (!connect_all) {
            return;
        }
        // Connects all the nodes
        for (Index_Type output_idx = type_start[Neuron_Type::OUTPUT];
             output_idx < type_start[Neuron_Type::OUTPUT + 1]; output_idx++) {
            for (Index_Type input_idx = type_start[Neuron_Type::INPUT];
                 input_idx < type_start[Neuron_Type::INPUT + 1]; input_idx++) {
                add_connection_innovation(input_idx, output_idx, genome_innovs);
            }
        }
    }
    // ========================= DEFAULT CTOR ============================
    InnovationDatabase()
        : innov_db(), genotype_neurons(), add_node_innov_lookup(), add_link_innov_lookup(),
          type_start() {}
    /*
     * Returns the genotypic neuron at @param neuron_idx stored in the contiguous
     * neuron database.
     */
    GenomeNeuron& neuron_at(const Index_Type neuron_idx) {
        // Alternative: do ifs with type_start.
        return this->genotype_neurons[neuron_idx];
    }
    /*
     * Returns the neuron type at @param neuron_idx stored in the contiguous
     * neuron database.
     */
    const Neuron_Type& neurtype_at(const Index_Type neuron_idx) {
        // Alternative: do ifs with type_start.
        return this->genotype_neurons[neuron_idx].type;
    }
    /*
     * Returns the innovation info at @param innov_idx stored in the contiguous
     * innovation database.
     */
    InnovationInfo<Index_Type>& innov_at(const Index_Type innov_idx) {
        return this->innov_db[neuron_idx];
    }
    /*
     * Returns the innovation info at @param innov_idx stored in the contiguous
     * innovation database.
     */
    InnovationInfo<Index_Type>& innov_at(const InnovationLink<Index_Type, FP_Type> innov_link) {
        return this->innov_db[innov_link.innovation_number()];
    }
    // Helper functions later for implementation.
};
// ==========================================================================

template <typename Index_Type = int>
using AkashicRecords = InnovationDatabase<Index_Type>; // :)
} // namespace NEAT