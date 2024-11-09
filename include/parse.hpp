#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

enum class RESPType : uint8_t
{
    ARRAY = 0,
    STRING = 1
};

enum class MessageType : uint8_t
{
    SET = 0,
    GET = 1,
    OK = 2,
    RESP_VAL = 3
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

bool serialize(const Message &msg, char *buff);
