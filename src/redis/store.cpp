#include "store.hpp"

Store::Store()
{
}

bool Store::set(K key, V val)
{
    const std::lock_guard<std::recursive_mutex> lock(map_mutex);
    bool was_present = map.find(key) != map.end();
    // map.insert({key, val});
    map[key] = val;
    return !was_present;
}

std::optional<V> Store::get(K key)
{
    const std::lock_guard<std::recursive_mutex> lock(map_mutex);
    auto result = map.find(key);
    if (result == map.end())
    {
        return std::nullopt;
    }
    return result->second;
}

bool Store::del(K key)
{
    const std::lock_guard<std::recursive_mutex> lock(map_mutex);
    return map.erase(key);
}

bool Store::copy(K original, K duplicate)
{
    const std::lock_guard<std::recursive_mutex> lock(map_mutex);
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
    const std::lock_guard<std::recursive_mutex> lock(map_mutex);
    copy(original, next);
    return map.erase(original);
}