#include "../../test_utils/test_utils.hpp"
#include <type_traits>
#include <limits>
#include "common/string_manip.h"

class Mod {
public:
    using Lvl = unsigned long long;
    using ID = unsigned long long;
    Lvl my_lvl;
    const ID my_id;
    Mod(Lvl lvl = 0, ID id = 101) : my_lvl(lvl), my_id(id){};
};
template <typename Num_Type>
constexpr int is_int(const Num_Type num)
{
    return std::numeric_limits<Num_Type>::is_integer ? 1 : 0;
}

template <typename Num_Type, typename Decayed = std::decay<Num_Type>::type>
constexpr int decayed_int(const Num_Type num = static_cast<Num_Type>(0))
{
    return std::numeric_limits<Decayed>::is_integer ? 1 : 0;
}
template <typename Int_Type = int, typename Decayed = typename std::decay<Int_Type>::type>
constexpr int digits(const Int_Type num = static_cast<Int_Type>(0))
{
    return std::numeric_limits<Decayed>::digits10 + 1 + std::numeric_limits<Decayed>::is_signed;
}

template <typename Num_Type, typename Decayed = std::decay<Num_Type>::type>
inline int foo(const Num_Type num = static_cast<Num_Type>(0))
{
    if (std::numeric_limits<Decayed>::is_integer) {
        std::cout << "Is integer: " << std::numeric_limits<Decayed>::is_integer << std::endl;
        return int_num<Decayed>();
    }
    return float_num<Decayed>();
}
template <typename Num_Type, typename Decayed = std::decay<Num_Type>::type>
constexpr int int_num(const Num_Type num = static_cast<Num_Type>(0))
{
    static_assert(std::numeric_limits<Num_Type>::is_integer, "int_num must have num of integer");
    return 100;
}
// 0 if double, -10 if float
template <typename Num_Type, typename Decayed = std::decay<Num_Type>::type>
constexpr int float_num(const Num_Type = static_cast<Num_Type>(0))
{
    return std::is_same<Num_Type, float>::value ? -10 : 0;
}
bool ref_test()
{
    using Level = unsigned long long;
    TEST_EQ(std::numeric_limits<Level>::is_integer, true);
    using Decayed = typename std::decay<Level>::type;
    TEST_EQ(std::numeric_limits<Decayed>::is_integer, true);
    Mod m(100);
    Mod def;
    const Mod cmod(100, 420);
    TEST_EQ(std::numeric_limits<decltype(m.my_lvl)>::is_integer, true);
    TEST_EQ(std::numeric_limits<decltype(def.my_lvl)>::is_integer, true);
    TEST_EQ(std::numeric_limits<decltype(Mod::my_lvl)>::is_integer, true);
    TEST_EQ(std::numeric_limits<std::decay_t<decltype(m.my_lvl)>>::is_integer, true);
    TEST_EQ(std::numeric_limits<std::decay_t<decltype(def.my_lvl)>>::is_integer, true);
    TEST_EQ(std::numeric_limits<std::decay_t<decltype(Mod::my_lvl)>>::is_integer, true);

    TEST_EQ(is_int(m.my_id), 1);
    TEST_EQ(is_int(def.my_lvl), 1);
    TEST_EQ(decayed_int(cmod.my_id), 1);
    TEST_EQ(is_int(cmod.my_id), 1);
    const Mod& ref = cmod;
    TEST_EQ(decayed_int(ref.my_id), 1);
    TEST_EQ(is_int(ref.my_id), 1);
    std::cout << "size: " << Common::num_cstr_size<int>() << std::endl;
    std::cout << "double decayed_int: " << decayed_int<double>() << std::endl;
    // TEST_EQ(Common::num_cstr_size<int>(), digits<double>());
    TEST_EQ(foo<double>(), 0);
    TEST_EQ(foo<float>(), -10);
    TEST_EQ(foo<int>(), 100);
    TEST_EQ(foo<long long>(), 100);
    return true;
}

int main()
{
    TEST_INITIALIZE;
    DO_TEST(ref_test);
    TEST_RESULTS;
    return 0;
}