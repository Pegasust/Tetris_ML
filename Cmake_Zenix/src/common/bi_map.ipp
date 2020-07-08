template <typename Num_Type, typename My_Pair, typename Single_Map>
bool Common::Structs::GenericBiMap<Num_Type, My_Pair, Single_Map>::contains(
    const typename Common::Structs::GenericBiMap<Num_Type, My_Pair, Single_Map>::TNum in) const{
    return this->data.find(in) != this->data.cend();
}

template <typename Num_Type, typename My_Pair, typename Single_Map>
typename Common::Structs::GenericBiMap<Num_Type, My_Pair, Single_Map>::TNum
Common::Structs::GenericBiMap<Num_Type, My_Pair, Single_Map>::get(
    const typename Common::Structs::GenericBiMap<Num_Type, My_Pair, Single_Map>::TNum in) {
    if (!contains(in)) {
        return -1;
    }
    return this->data[in];
}

template <typename Num_Type, typename My_Pair, typename Single_Map>
void Common::Structs::GenericBiMap<Num_Type, My_Pair, Single_Map>::add(
    const typename Common::Structs::GenericBiMap<Num_Type, My_Pair, Single_Map>::TNum x,
    const typename Common::Structs::GenericBiMap<Num_Type, My_Pair, Single_Map>::TNum y) {
    using _TNum_ = typename Common::Structs::GenericBiMap<Num_Type, My_Pair, Single_Map>::TNum;
    _TNum_ v = get(x);
    ASSERT(v != y && get(v) != x, "Trying to add a pre-existed pair.");
    this->data[x] = y;
    this->data[y] = x;
}

template <typename Num_Type, typename My_Pair, typename Single_Map>
typename Common::Structs::GenericBiMap<Num_Type, My_Pair, Single_Map>::TNum
Common::Structs::GenericBiMap<Num_Type, My_Pair, Single_Map>::replace(
    const typename Common::Structs::GenericBiMap<Num_Type, My_Pair, Single_Map>::TNum left,
    const typename Common::Structs::GenericBiMap<Num_Type, My_Pair, Single_Map>::TNum new_right) {
    typename Common::Structs::GenericBiMap<Num_Type, My_Pair, Single_Map>::TNum old_right =
        get(left);
    typename Common::Structs::GenericBiMap<Num_Type, My_Pair, Single_Map>::TNum retval =
        data.erase(old_right);
    add(left, new_right);
    return retval;
}

template <typename Num_Type, typename My_Pair, std::size_t Max_Size>
typename Common::Structs::ArrayBiMap<Num_Type, My_Pair, Max_Size>::TNum
Common::Structs::ArrayBiMap<Num_Type, My_Pair, Max_Size>::get(
    const typename Common::Structs::ArrayBiMap<Num_Type, My_Pair, Max_Size>::TNum in) {
    return this->data[in];
}

template <typename Num_Type, typename My_Pair, std::size_t Max_Size>
bool Common::Structs::ArrayBiMap<Num_Type, My_Pair, Max_Size>::contains(
    const typename Common::Structs::ArrayBiMap<Num_Type, My_Pair, Max_Size>::TNum in) const {
    using _TNum_ = typename Common::Structs::ArrayBiMap<Num_Type, My_Pair, Max_Size>::TNum;
    if (this->data[in] < 0) {
        POTENTIAL_BUG_POINT_MSG("Should change the implementation to make all values biased.");
        return false;
    }
    _TNum_ v = data[in];
    return data[v] == in;
}


template <typename Num_Type, typename My_Pair, std::size_t Max_Size>
void Common::Structs::ArrayBiMap<Num_Type, My_Pair, Max_Size>::add(
    const typename Common::Structs::ArrayBiMap<Num_Type, My_Pair, Max_Size>::TNum x,
    const typename Common::Structs::ArrayBiMap<Num_Type, My_Pair, Max_Size>::TNum y) {
    using _TNum_ = typename Common::Structs::ArrayBiMap<Num_Type, My_Pair, Max_Size>::TNum;
    ASSERT(!contains(x) && !contains(y), "Trying to add a pre-existed pair.");
    this->data[x] = y;
    this->data[y] = x;
    count += 2;
}

template <typename Num_Type, typename My_Pair, std::size_t Max_Size>
typename Common::Structs::ArrayBiMap<Num_Type, My_Pair, Max_Size>::TNum
Common::Structs::ArrayBiMap<Num_Type, My_Pair, Max_Size>::replace(
    const typename Common::Structs::ArrayBiMap<Num_Type, My_Pair, Max_Size>::TNum left,
    const typename Common::Structs::ArrayBiMap<Num_Type, My_Pair, Max_Size>::TNum new_right) {
    using _TNum_ = typename Common::Structs::ArrayBiMap<Num_Type, My_Pair, Max_Size>::TNum;
    if (contains(left)) {
        count -= 2;
        _TNum_ mapped = get(left);
        this->data[mapped] = -1;
    }
    add(left, new_right);
    return 1;
}
