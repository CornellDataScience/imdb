#include "redis.hpp"
#include "parse.hpp"
#include "server.hpp"

#define PORT 8080
#define BUFFER_SIZE 1024

RedisNode::RedisNode(bool isBackup)
{
    this->isBackup = isBackup;
}

bool RedisNode::set(const K &key, const V &val)
{
    return kvStore.set(key, val);
}

V RedisNode::get(const K &key)
{
    if (auto val = kvStore.get(key); val)
    {
        return *val;
    }
    else
    {
        std::cout << "Key not found" << std::endl;
        return "";
    }
}

bool RedisNode::del(const K &key)
{
    return kvStore.del(key);
}

bool RedisNode::rename(const K &key_from, const K &key_to)
{
    return kvStore.rename(key_from, key_to);
}

bool RedisNode::copy(const K &key_from, const K &key_to)
{
    return kvStore.copy(key_from, key_to);
}

Message RedisNode::handle_client_req(const Message &req)
{
    switch (req.type)
    {
    case MessageType::GET:
    {
        V ret_val = get(req.key);
        return {MessageType::RETURN, "", std::get<std::string>(ret_val)};
    }
    case MessageType::SET:
    {
        set(req.key, req.val);
        return {MessageType::OK, req.key, req.val};
    }
    case MessageType::DEL:
        del(req.key);
        return {MessageType::OK, req.key, ""};
    case MessageType::RENAME:
        // TODO
        // Return a message type of OK
        return {};
    case MessageType::COPY:
        // TODO
        // Return a message type of OK
        return {};
    default:
        return {MessageType::FAIL, "", ""};
    }
}

int main(int argc, char **argv)
{
    bool isBackup;
    std::string primary_ip;

    if (argc == 1)
    {
        isBackup = false;
    }
    else if (argc == 3 && strcmp(argv[1], "--backup") == 0)
    {
        isBackup = true;
        primary_ip = argv[2];
    }
    else
    {
        std::cout << "Usage: ./redis OR ./redis --backup <primary_ip> " << std::endl;
        return 1;
    }

    RedisNode redisNode(isBackup);

    TcpServer server = TcpServer(PORT);
    std::cout << "Server listening on port " << PORT << "\n";

    char buffer[BUFFER_SIZE] = {0};

    server.connect();

    while (true)
    {
        bool received = server.receive(buffer);
        if (received)
        {
            Message req = deserialize(buffer);
            Message resp = redisNode.handle_client_req(req);
            server.respond(resp);
        }
        else
        {
            std::cerr << "[ERROR] recv failed: " << strerror(errno) << std::endl;
        }
    }

    return 0;
}
