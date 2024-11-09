#include <iostream>
#include "parse.hpp"

int main()
{
    std::cout << "Redis Test\n";
    std::cout << "-------------\n";
    char resp_message_get[] = "*2\r\n$3\r\nGET\r\n$3\r\nkey\r\n";
    char resp_message_set[] = "*3\r\n$3\r\nSET\r\n$3\r\nkey\r\n$5\r\nvalue\r\n";

    Message get = deserialize(resp_message_get);
    std::cout << "Type: " << (int)get.type << "\n";
    std::cout << "Key: " << get.key << "\n";
    std::cout << "Val: " << get.val << "\n";

    Message set = deserialize(resp_message_set);
    std::cout << "Type: " << (int)set.type << "\n";
    std::cout << "Key: " << set.key << "\n";
    std::cout << "Val: " << set.val << "\n";

    Message msg1 = {MessageType::GET, "1", ""};
    Message msg2 = {MessageType::SET, "2", "2"};
    Message msg3 = {MessageType::OK, "3", "3"};
    Message msg4 = {MessageType::RESP_VAL, "", "4"};

    char buff1[1024];
    char buff2[1024];
    char buff3[1024];
    char buff4[1024];

    serialize(msg1, buff1);
    serialize(msg2, buff2);
    serialize(msg3, buff3);
    serialize(msg4, buff4);

    std::cout << buff1 << "\n";
    std::cout << buff2 << "\n";
    std::cout << buff3 << "\n";
    std::cout << buff4 << "\n";

    return 0;
}