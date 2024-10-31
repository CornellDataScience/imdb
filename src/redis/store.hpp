#include <unordered_map>
#include <mutex>
#include <iostream>
#include <optional>
#include <string>

// thread-safe wrapper layer of std::unordered_map
template <typename Key, typename Value> 
class KVStore
{
public:
    KVStore();
    // get the value of the key in value store, returns value if key exists, returns nullopt if key does not exists
    std::optional<Value> get(Key key) const;

    // put an entry (key, value) in key value store
    bool put(Key key, Value value);

    // erase an entry (key, value) in key value store
    bool erase(Key key);
    
    // rename the entry associated with source_key to target_key, deleting the original source_key entry from the key value store
    bool rename(Key source_key, Key target_key);

    // append value to the current entry associated with key
    bool append(Key key, Value value);

    // copy value at the entry associated with source_key to the entry associated with target_key
    bool copy(Key source_key, Key target_key);
    // operator only for get for now (doing put/append does break thread-safe if not careful)
    std::optional<Value>  operator[](const Key& key);
       
private:
    std::unordered_map<Key, Value> store;
};