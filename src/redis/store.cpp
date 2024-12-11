#include "store.hpp"
#include <string> 
Store::Store()
{
}

bool Store::set(K key, V val)
{
    const std::lock_guard<std::recursive_mutex> lock(map_mutex);
    bool was_present = map.find(key) != map.end();
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

bool Store::copy(K original, V duplicate)
{
    const std::lock_guard<std::recursive_mutex> lock(map_mutex);
    auto result = map.find(original);
    if (result == map.end())
    {
        return false;
    }

    if (std::holds_alternative<std::string>(duplicate))
    {
        std::string duplicate_string = std::get<std::string>(duplicate);
        set((K)(duplicate_string), result->second);
        return true;
    }

    return false;
}


bool Store::rename(K original, V next)
{
    const std::lock_guard<std::recursive_mutex> lock(map_mutex);
    copy(original, next);
    return map.erase(original);
}

bool Store::append(K key, V next)
{
    const std::lock_guard<std::recursive_mutex> lock(map_mutex);

    auto opt_value = get(key);
    if (!opt_value)
    {
        return false;
    }

    V current_value = *opt_value;

    if (std::holds_alternative<std::string>(current_value) && std::holds_alternative<std::string>(next))
    {
        std::string new_value = std::get<std::string>(current_value) + std::get<std::string>(next);
        set(key, V(new_value));
        return true;
    }

    return false;
}


// bool Store::getrange(K key, V from, V to)
// {
//     const std::lock_guard<std::recursive_mutex> lock(map_mutex);
//     auto result = map.find(key);
//     if (result == map.end())
//     {
//         return std::nullopt;
//     }
//     result = substr(from, to)
//     return result->second;
// }

bool Store::decrby(K key, V val)
{
    const std::lock_guard<std::recursive_mutex> lock(map_mutex);
    auto opt_value = get(key);
    if (!opt_value)
    {
        return false;
    }
    std::string value_str = std::get<std::string>(*opt_value);
    std::string val_str = std::get<std::string>(val);

    int value = std::stoi(value_str);
    int decrement = std::stoi(val_str);

    int new_value = value - decrement;
    set(key, std::to_string(new_value));

    return true;
    
}

std::optional<V> Store::getdel(K key) {
    const std::lock_guard<std::recursive_mutex> lock(map_mutex);

    auto opt_val = get(key);
    if (opt_val) {
        del(key);
        return opt_val;
    }

    return std::nullopt;
}
