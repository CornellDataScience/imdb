#pragma once

#include <cstdint>
#include <string>

enum class RespType : uint8_t
{
    ARRAY = 0,
    STRING = 1
};

struct Message
{
    uint8_t type;
    std::string key;
    std::string val;
};

Message deserialize(char *buff);
bool serialize(const Message &msg, char *buff);
