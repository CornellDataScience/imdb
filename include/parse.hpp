#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

enum class RESPType : uint8_t
{
    ARRAY = 0,
    STRING = 1
};

enum class MessageType : uint8_t
{
    RETURN = 0,
    OK = 1,
    FAIL = 2,
    SET = 3,
    GET = 4,
    DEL = 5,
    RENAME = 6,
    COPY = 7,
    APPEND = 8,
    // GET_RANGE = 6,
    DECRBY = 9,
    GETDEL = 10,
};

struct Message
{
    MessageType type;
    std::string key;
    std::string val;
};

Message deserialize(char *buff);

template <RESPType T>
void deserializeRESP(std::string &data, size_t &pos, Message &msg);

std::string serializeArray(const std::vector<std::string> &data);
bool serialize(const Message &msg, char *buff);
