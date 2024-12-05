#pragma once

#include <string>
#include <iostream>
#include <optional>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../include/store.hpp"
#include "../include/parse.hpp"

class RedisNode
{
private:
    Store kvStore;
    bool isBackup;

    V get(const K &key);
    bool set(const K &key, const V &val);
    bool del(const K &key);
    bool rename(const K &key_from, const K &key_to);
    bool copy(const K &key_from, const K &key_to);

public:
    RedisNode();
    RedisNode(bool isBackup);
    Message handle_client_req(const Message &req);
};
