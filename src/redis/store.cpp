#include "store.hpp"

Store::Store()
{
}

bool Store::set(K key, V val)
{
    bool was_present = map.find(key) != map.end();
    // map.insert({key, val});
    map[key] = val;
    return !was_present;
}

std::optional<V> Store::get(K key)
{
    auto result = map.find(key);
    if (result == map.end())
    {
        return std::nullopt;
    }
    return result->second;
}

bool Store::del(K key)
{
    return map.erase(key);
}

bool Store::copy(K original, K duplicate)
{
    auto result = map.find(original);
    if (result == map.end())
    {
        return false;
    }
    map.insert({duplicate, result->second});
    return true;
}

bool Store::rename(K original, K next)
{
    copy(original, next);
    return map.erase(original);
}