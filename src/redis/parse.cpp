#include <cstring>
#include <string>
#include <iostream>
#include "parse.hpp"

static const std::unordered_map<std::string, MessageType> msgTypeMap = {
    {"SET", MessageType::SET},
    {"GET", MessageType::GET},
    {"OK", MessageType::OK},
    {"RESP_VAL", MessageType::RESP_VAL}};

static const std::unordered_map<std::string, RESPType> respTypeMap = {
    {"*", RESPType::ARRAY},
    {"$", RESPType::STRING}};

size_t parseSize(const std::string &data, size_t &pos)
{
    size_t end_pos = data.find("\r\n", pos);
    size_t size = std::stoul(data.substr(pos, end_pos - pos));
    pos = end_pos + 2;
    return size;
}

std::string parseString(const std::string &data, size_t &pos, size_t size)
{
    std::string result = data.substr(pos, size);
    pos += size + 2;
    return result;
}

std::string deserializeBulkString(std::string &data, size_t &pos) // template this later
{
    pos++;
    size_t bulk_size = parseSize(data, pos);
    return parseString(data, pos, bulk_size);
}

template <>
void deserializeRESP<RESPType::ARRAY>(std::string &data, size_t &pos, Message &msg)
{
    size_t array_size = parseSize(data, pos);

    std::string messageType = deserializeBulkString(data, pos);
    msg.type = static_cast<uint8_t>(msgTypeMap.at(messageType));

    std::string key = deserializeBulkString(data, pos);
    msg.key = key;

    if (msg.type == 0 || msg.type == 2 || msg.type == 3)
    {
        std::string val = deserializeBulkString(data, pos);
        msg.val = val;
    }
}

Message deserialize(char *buff)
{
    Message msg;
    std::string data(buff);
    size_t pos = 0;

    char RESPType = data[pos];
    pos++;
    switch (respTypeMap.at(std::string(1, RESPType))) // maybe fix later ?
    {
    case RESPType::ARRAY:
    {
        deserializeRESP<RESPType::ARRAY>(data, pos, msg);
        break;
    }
    default:
        std::cout << "Invalid RESPType\n";
        break;
    }

    return msg;
}

bool serialize(const Message &msg, char *buff)
{
    std::string serialized;
    if (msg.type == 0) // set
    {
        serialized += "*3\r\n";
        serialized += "$3\r\nSET\r\n";
        serialized += "$" + std::to_string(msg.key.size()) + "\r\n";
        serialized += msg.key + "\r\n";
        serialized += "$" + std::to_string(msg.val.size()) + "\r\n";
        serialized += msg.val + "\r\n";
    }
    else if (msg.type == 1) // get
    {
        serialized += "*2\r\n";
        serialized += "$3\r\nGET\r\n";
        serialized += "$" + std::to_string(msg.key.size()) + "\r\n";
        serialized += msg.key + "\r\n";
    }
    else if (msg.type == 2) // ok
    {
        serialized += "*3\r\n";
        serialized += "$3\r\nSET\r\n";
        serialized += "$" + std::to_string(msg.key.size()) + "\r\n";
        serialized += msg.key + "\r\n";
        serialized += "$" + std::to_string(msg.val.size()) + "\r\n";
        serialized += msg.val + "\r\n";
    }
    else if (msg.type == 3) // resp_val
    {
        serialized += "*3\r\n";
        serialized += "$3\r\nSET\r\n";
        serialized += "$" + std::to_string(msg.key.size()) + "\r\n";
        serialized += msg.key + "\r\n";
        serialized += "$" + std::to_string(msg.val.size()) + "\r\n";
        serialized += msg.val + "\r\n";
    }
    std::memcpy(buff, serialized.c_str(), serialized.size() + 1);
    return true;
}