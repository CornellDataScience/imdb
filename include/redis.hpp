#pragma once

#include <string>
#include <iostream>
#include <optional>  
#include "../include/store.hpp"   
#include "../include/parse.hpp" 

class RedisNode {
private:
    Store kvStore; 

    V get(K key);
    bool set(K key, V val);
    bool del(K key);
    bool rename(K key_from, K key_to);
    bool copy(K key_from, K key_to);

public:
    RedisNode();

    Message handle_client_req(const Message& req);
};
