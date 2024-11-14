#include <variant>
#include <string>
#include <optional>
#include <unordered_map>
#include <mutex>

using K = std::string;
using V = std::variant<std::string>;

class Store
{
private:
    std::unordered_map<K, V> map;
    std::recursive_mutex map_mutex;

public:
    Store();
    std::optional<V> get(K key);
    bool set(K key, V val);
    bool del(K key);
    bool rename(K original, K next);
    bool copy(K original, K duplicate);
};