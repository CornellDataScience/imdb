#include <../include/redis.hpp>

class RedisNode {
private:
    Store kvStore; 

    V get(const K& key) {
        if (auto val = kvStore.get(key); val) {
            return *val;
        } else {
            std::cout << "Key not found" << std::endl;
        }
    }

    bool set(const K& key, const K& val) {
        return kvStore.set(key, val);
    }

    bool del(const K& key) {
        return kvStore.del(key);
    }

    bool rename(const K& key_from, const K& key_to) {
        return kvStore.rename(key_from, key_to);
    }

    bool copy(const K& key_from, const K& key_to) {
        return kvStore.copy(key_from, key_to);
    }
    
public:
    RedisNode() {}

    Message handle_client_req(const Message& req) {
        switch(req.type){
            case 0:
                get(req.key);
            case 1:
                set(req.key, req.val);
            // case DEL:
            //     del(req.key);
            // case RENAME:
            //     rename(req.key_from, req.key_to);
            // case COPY:
            //     copy(req.key_from, req.key_to);
        }
    }

};

int main() {
    KVStoreWrapper kvWrapper;

    // Testing the wrapper
    kvWrapper.set("username", "Alice");
    std::cout << "Username: " << kvWrapper.get("username") << std::endl;

    if (kvWrapper.exists("username")) {
        std::cout << "Username exists." << std::endl;
    }

    kvWrapper.remove("username");

    return 0;
}