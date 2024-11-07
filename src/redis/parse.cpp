#include "parse.hpp"
#include <cstring>
#include <string>

Message deserialize(char *buff)
{
    Message msg;
    std::string data(buff);
    size_t pos = 0;
    pos++;
    size_t array_size = std::stoi(data.substr(pos, data.find("\r\n", pos) - pos));
    pos = data.find("\r\n", pos) + 2;
    pos++;
    size_t type_size = std::stoi(data.substr(pos, data.find("\r\n", pos) - pos));
    pos = data.find("\r\n", pos) + 2;
    std::string type = data.substr(pos, type_size);
    pos += type_size + 2;
    if (type == "SET")
    {
        msg.type = 0;
    }
    else if (type == "GET")
    {
        msg.type = 1;
    }
    else if (type == "OK")
    {
        msg.type = 2;
    }
    else if (type == "RESP_VAL")
    {
        msg.type = 3;
    }
    pos++;
    size_t key_size = std::stoi(data.substr(pos, data.find("\r\n", pos) - pos));
    pos = data.find("\r\n", pos) + 2;
    msg.key = data.substr(pos, key_size);
    pos += key_size + 2;
    if (msg.type == 0 || msg.type == 2 || msg.type == 3)
    {
        pos++;
        size_t val_size = std::stoi(data.substr(pos, data.find("\r\n", pos) - pos));
        pos = data.find("\r\n", pos) + 2;
        msg.val = data.substr(pos, val_size);
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