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

//EXTRA SPACE REMOVE LATER
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

bool RedisNode::rename(const K &key_from, const V &key_to)
{
    return kvStore.rename(key_from, key_to);
}

bool RedisNode::copy(const K &key_from, const V &key_to)
{
    return kvStore.copy(key_from, key_to);
}

bool RedisNode::decrby(const K &key, const V &val)
{
    return kvStore.decrby(key, val);
}

bool RedisNode::append(const K &key, const V &value)
{
    return kvStore.append(key, value);
}

V RedisNode::getdel(const K &key)
{
     if (auto val = kvStore.getdel(key); val)
    {
        return *val;
    }
    else
    {
        std::cout << "Key not found" << std::endl;
        return "";
    }
}


Message RedisNode::handle_client_req(const Message &req)
{
    switch (req.type)
    {
    case MessageType::GET:
    {
        V resp_val = get(req.key);
        return {MessageType::RETURN, std::get<std::string>(resp_val), ""}; // put the value in the key field to avoid an extra space in print
    }
    case MessageType::SET:
    {
        set(req.key, req.val);
        return {MessageType::OK, req.key, req.val};
    }
    case MessageType::DEL:
    {
        del(req.key);
        return {MessageType::OK, req.key, ""};
    }
    case MessageType::RENAME:
    {
        rename(req.key, req.val);
        V val = get(req.val);
        return {MessageType::OK, req.val, std::get<std::string>(val)};
    }
    case MessageType::COPY:
    {
        V resp_val = req.val;
        copy(req.key, req.val);
        return {MessageType::OK, req.val, std::get<std::string>(resp_val)};
    }
    case MessageType::APPEND:
    {

        append(req.key, req.val);
        return {MessageType::OK, req.key, std::get<std::string>(get(req.key))};
    }
    // Current stuff cant handle 3 inputse
    // case MessageType::GETRANGE:
    //     V resp_val = get(req.key);
    //     resp_val = resp_val.substr(stoi(req.from), stoi(req.to));
    //     return {MessageType::RETURN, "", std::get<std::string>(resp_val)};
    case MessageType::DECRBY:
    {
        decrby(req.key, req.val);
        return {MessageType::OK, req.key, std::get<std::string>(get(req.key))};
    }
    case MessageType::GETDEL:
    {
        V resp_val = getdel(req.key);
        return {MessageType::RETURN, "", std::get<std::string>(resp_val)};
    }
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
