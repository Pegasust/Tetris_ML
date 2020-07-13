#pragma once
#include <limits>
#include <map>
#include <unordered_map>
#include <utility>
#include "assertion.hpp"
// Bijective map structures

namespace Common {
namespace Structs {
// ================================= STRUCTURES ===================================
template <typename L_Type = int, typename R_Type = typename L_Type>
// .first and .second must be implemented.
using Pair = std::pair<L_Type, R_Type>;

// Single map should implement access & insert by operator[], and and erase().
template <
    typename Num_Type = int, typename My_Pair = Pair<Num_Type>,
    typename Single_Map = std::unordered_map<Num_Type, Num_Type>>
class GenericBiMap {
public:
    using TNum = typename Num_Type;
    using TPair = typename My_Pair;
    using TMap = typename Single_Map;
    using Bi_Map = GenericBiMap<TNum, TPair, TMap>;

protected:
    Single_Map data;

public:
    bool contains(const TNum elem) const;
    TNum get(const TNum in);
    void add(const TNum x, const TNum y);
    // replace mapped of left (left, old_right) -> (left, new_right),
    // remove old_right to left,
    // create (new_right, left).
    // Returns 0 if not success, return any positive int (values removed) if success.
    TNum replace(const TNum left, const TNum new_right);
    // Number of elements
    std::size_t size() const { return data.size(); }
    // Number of pairs
    std::size_t pair_count() const { return size() / 2; }
    // Number of elements that can possibly be stored
    std::size_t max_size() const { return data.max_size(); }
    // Get syntactic sugar
    typename Bi_Map::TNum operator|(typename Bi_Map::TNum in) { return get(in); }
    // Get syntactic sugar
    typename Bi_Map::TNum operator[](typename Bi_Map::TNum in) { return get(in); }
    // Add or replace syntactic sugar
    typename Bi_Map& operator<<=(typename Bi_Map::TPair&& in_pair) {
        replace(
            std::forward<Bi_Map::TPair>(in_pair).first,
            std::forward<Bi_Map::TPair>(in_pair).second);
        return *this;
    }
    // Strictly add syntactic sugar
    typename Bi_Map& operator<<(typename Bi_Map::TPair&& in_pair) {
        add(std::forward<Bi_Map::TPair>(in_pair).first,
            std::forward<Bi_Map::TPair>(in_pair).second);
        return (*this);
    }
};

// ArrayBiMap (static size bimap) to store elements).
// O(N) space [a very big O(1)...], O(1) lookup.
// Great lookup time if BiMap is going to be as big as the domain/codomain.
// Note that Max_Size also applies on the maximum value being stored.
// For now, ArrayBiMap should only be used on unsigned values.
template <
    typename Num_Type = int, typename My_Pair = Pair<Num_Type>,
    std::size_t Max_Size = std::numeric_limits<short>::max()>
class ArrayBiMap {
public:
    using TNum = typename Num_Type;
    using TPair = typename My_Pair;
    using Bi_Map = ArrayBiMap<TNum, TPair, Max_Size>;

protected:
    Num_Type data[Max_Size];
    std::size_t count;

public:
    ArrayBiMap()
        : count(0){};
    bool contains(const TNum elem) const;
    TNum get(const TNum in);
    void add(const TNum x, const TNum y);
    // replace mapped of left (left, old_right) -> (left, new_right),
    // remove old_right to left,
    // create (new_right, left).
    // Returns 0 if not success, return any positive int (values removed) if success.
    TNum replace(const TNum left, const TNum new_right);
    // Number of elements
    std::size_t size() const { return count; }
    // Number of pairs
    std::size_t pair_count() const { return size() / 2; }
    // Number of elements that can possibly be stored
    static constexpr std::size_t max_size() { return Max_Size; }
    // Get syntactic sugar
    typename Bi_Map::TNum operator|(typename Bi_Map::TNum in) { return get(in); }
    // Get syntactic sugar
    typename Bi_Map::TNum operator[](typename Bi_Map::TNum in) { return get(in); }
    // Add or replace syntactic sugar
    typename Bi_Map& operator<<=(typename Bi_Map::TPair&& in_pair) {
        replace(
            std::forward<Bi_Map::TPair>(in_pair).first,
            std::forward<Bi_Map::TPair>(in_pair).second);
        return *this;
    }
    // Strictly add syntactic sugar
    typename Bi_Map& operator<<(typename Bi_Map::TPair&& in_pair) {
        add(std::forward<Bi_Map::TPair>(in_pair).first,
            std::forward<Bi_Map::TPair>(in_pair).second);
        return (*this);
    }
};

// TODO: VectorBiMap (ArrayBiMap backed by a vector data structure)

// TODO: TreeBiMap: Backed up my a type of tree, maps Num to Num.
// O(n) space, O(logN) time.
// Great "average" size case, also keeps data ordered.
// template <typename Num_Type = int, typename My_Pair = Pair<Num_Type>,
//          typename TreeStruct = std::map<Num_Type, Num_Type>>
// class TreeBiMap {
// public:
//    using TNum = typename Num_Type;
//    using TPair = typename My_Pair;
//    using TTree = typename TreeStruct;
//    using Bi_Map = TreeBiMap<TNum, TPair, TTree>;
//
//};
template <
    typename Num_Type = int, typename My_Pair = Pair<Num_Type>,
    typename Tree_Struct = std::map<Num_Type, Num_Type>>
using TreeBiMap = GenericBiMap<Num_Type, My_Pair, Tree_Struct>;

// TODO: BiasedBiMap: Specializes when either left is very big, or very small,
// as compared to right

// ============================== PROCEDURAL ACCESS ===============================

// ================================ CONVERSION ====================================

} // namespace Structs
} // namespace Common
#include "bi_map.ipp"