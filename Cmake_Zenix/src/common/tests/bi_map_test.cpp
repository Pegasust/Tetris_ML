#include "../../test_utils/test_utils.hpp"
#include "../bi_map.hpp"
using namespace Common::Structs;

template <typename GBimap = typename GenericBiMap<>>
bool tgeneric_bimap() {
    GBimap bimap;
    bimap << GBimap::TPair(1, 2) << GBimap::TPair(3, 10);
    // 1 <-> 2
    // 3 <-> 10
    TEST_EQ((bimap | 1), 2);
    TEST_EQ((bimap | 2), 1);
    TEST_EQ((bimap | 3), 10);
    TEST_EQ((bimap | 10), 3);
    TEST_EQ(bimap.size(), 4);
    TEST_EQ(bimap.pair_count(), 2);

    bimap <<= GBimap::TPair(1, 69);
    // 1 <-> 69
    // 3 <-> 10
    TEST_EQ((bimap | 1), 69);
    TEST_EQ((bimap | 69), 1);
    TEST_EQ((bimap | 3), 10);
    TEST_EQ((bimap | 10), 3);
    TEST_EQ(bimap.pair_count(), 2);

    // technically illegal: bimap << GBimap::TPair(69,120)
    // because << states that we expect to add non-pre-existed pairs.
    bimap <<= GBimap::TPair(69, 120); // technically illegal
    // 69 <-> 120
    // 3 <-> 10
    TEST_EQ((bimap | 69), 120);
    TEST_EQ((bimap | 120), 69);
    TEST_EQ((bimap | 3), 10);
    TEST_EQ((bimap | 10), 3);
    TEST_EQ(bimap.pair_count(), 2);

    bimap << GBimap::TPair(25, 420);
    // 69 <-> 120
    // 3 <-> 10
    // 25 <-> 420
    TEST_EQ((bimap | 69), 120);
    TEST_EQ((bimap | 120), 69);
    TEST_EQ((bimap | 3), 10);
    TEST_EQ((bimap | 10), 3);
    TEST_EQ((bimap | 25), 420);
    TEST_EQ((bimap | 420), 25);
    TEST_EQ(bimap.pair_count(), 3);
    return true;
}

int main() {
    TEST_INITIALIZE;
    DO_TEST(tgeneric_bimap<>);
    DO_TEST(tgeneric_bimap<ArrayBiMap<>>);
    TEST_RESULTS;
    return 0;
}