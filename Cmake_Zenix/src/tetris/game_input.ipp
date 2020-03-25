#pragma once


//template <int capacity>
//Tetris::InputCollection<capacity>::InputCollection() : count(0), collection() {}
//
//template <int capacity>
//void Tetris::InputCollection<capacity>::add_entry(const InputEntry& entry) {
//    if (count == capacity) {
//        collection.pop();
//    }
//    collection.push(entry);
//}
//
//
//template <>
//class Tetris::InputCollection<0> {
//public:
//    InputCollection();
//    InputCollection(const std::string& serialized_collection);
//    void add_entry(const InputEntry& entry);
//    void serialize_self(const std::string& string);
//};
//
//template <>
//class Tetris::InputCollection<-1> {
//
//private:
//    using Collection = std::queue<InputEntry>;
//    Collection collection;
//    InputEntry recently_added;
//
//public:
//    InputCollection();
//    InputCollection(const std::string& serialized_collection);
//    void add_entry(const InputEntry& entry);
//    /*
//    Make string that can be deserialized.
//    */
//    void serialize_self(const std::string& string);
//};
//
//
//Tetris::InputCollection<0>::InputCollection() {
//
//}
//
//void Tetris::InputCollection<0>::add_entry(const InputEntry& entry) {
//
//}
//
//Tetris::InputCollection<-1>::InputCollection() : collection() {
//    recently_added = NULL;
//}
//
//void Tetris::InputCollection<-1>::add_entry(const InputEntry& entry) {
//    collection.push(entry);
//}
