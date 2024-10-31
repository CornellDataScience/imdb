#include "store.hpp"

template <typename Key, typename Value>
KVStore<Key, Value>::KVStore()
{
}

template <typename Key, typename Value>
std::optional<Value> KVStore<Key, Value>::get(Key key) const
{
    return std::optional<Value>();
}

template <typename Key, typename Value>
bool KVStore<Key, Value>::put(Key key, Value value)
{
    return false;
}

template <typename Key, typename Value>
bool KVStore<Key, Value>::erase(Key key)
{
    return false;
}

template <typename Key, typename Value>
bool KVStore<Key, Value>::rename(Key source_key, Key target_key)
{
    return false;
}

template <typename Key, typename Value>
bool KVStore<Key, Value>::append(Key key, Value value)
{
    return false;
}

template <typename Key, typename Value>
bool KVStore<Key, Value>::copy(Key source_key, Key target_key)
{
    return false;
}

template <typename Key, typename Value>
std::optional<Value> KVStore<Key, Value>::operator[](const Key &key)
{
    return std::optional<Value>();
}
