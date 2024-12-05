#include "redis.hpp"
#include "parse.hpp"
#include "server.hpp"

#define PORT 8080
#define BUFFER_SIZE 1024

RedisNode::RedisNode()
{
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

bool RedisNode::set(const K &key, const V &val)
{
    return kvStore.set(key, val);
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
        V resp_val = get(req.key);
        return {MessageType::RESP_VAL, "", std::get<std::string>(resp_val)};
    }
    case MessageType::SET:
    {
        set(req.key, req.val);
        return {MessageType::OK, req.key, req.val};
    }
    // case DEL:
    //     del(req.key);
    // case RENAME:
    //     rename(req.key_from, req.key_to);
    // case COPY:
    //     copy(req.key_from, req.key_to);
    default:
        return {MessageType::FAIL, "", ""};
    }
}

int main()
{
    char buffer[BUFFER_SIZE] = {0};
    TcpServer server = TcpServer(PORT);

    std::cout << "Server listening on port " << PORT << "\n";

    RedisNode redisNode = RedisNode();

    Message message = {.type = MessageType(), .key = "h", .val = "hi"};

    server.receive(message, buffer);
    std::cout << message.key << message.val;
    Message message2 = {.type = MessageType(), .key = "hi back", .val = "hi back"};

    server.respond(message2);
    std::cout << message2.key << message2.val;

    close(server.server_fd);
    return 0;
}
