#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include "parse.hpp"

static const std::unordered_map<std::string, MessageType> msgTypeMap = {
    {"SET", MessageType::SET},
    {"GET", MessageType::GET},
    {"DEL", MessageType::DEL},
    {"OK", MessageType::OK},
    {"RETURN", MessageType::RETURN}};

static const std::unordered_map<char, RESPType> respTypeMap = {
    {'*', RESPType::ARRAY},
    {'$', RESPType::STRING}};

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
    msg.type = static_cast<MessageType>(msgTypeMap.at(messageType));

    std::string key = deserializeBulkString(data, pos);
    msg.key = key;

    // funny logic may need to think aobut this for other message types
    if (msg.type != MessageType::GET && msg.type != MessageType::DEL)
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

    char RESPType = data[pos++];

    switch (respTypeMap.at(RESPType))
    {
    case RESPType::ARRAY:
    {
        deserializeRESP<RESPType::ARRAY>(data, pos, msg);
        break;
    }
    default:
        std::cout << "Invalid RESPType!!\n";
        break;
    }
    return msg;
}

std::string serializeBulkString(const std::string &data)
{
    return "$" + std::to_string(data.size()) + "\r\n" + data + "\r\n";
}

std::string serializeArray(const std::vector<std::string> &data)
{
    std::string serialized = "*" + std::to_string(data.size()) + "\r\n";
    for (const auto &str : data)
    {
        serialized += serializeBulkString(str);
    }
    return serialized;
}

bool serialize(const Message &msg, char *buff)
{
    std::string serialized;
    switch (msg.type)
    {
    case MessageType::SET:
    {
        serialized += serializeArray({"SET", msg.key, msg.val});
        break;
    }
    case MessageType::GET:
    {
        serialized += serializeArray({"GET", msg.key});
        break;
    }
    case MessageType::DEL:
    {
        serialized += serializeArray({"DEL", msg.key});
        break;
    }
    case MessageType::RENAME:
    {
        // TODO
        break;
    }
    case MessageType::COPY:
    {
        // TODO
        break;
    }
    case MessageType::OK:
        serialized += serializeArray({"OK", msg.key, msg.val});
        break;
    case MessageType::RETURN:
        serialized += serializeArray({"RETURN", msg.key, msg.val});
        break;
    }
    std::memcpy(buff, serialized.c_str(), serialized.size() + 1);
    return true;
}