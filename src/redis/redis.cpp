#include "../include/redis.hpp"

RedisNode::RedisNode()
{
    kvStore = Store();
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

// int main()
// {
//     std::cout << "Redis Test\n";
//     std::cout << "----------\n";

//     RedisNode redisNode = RedisNode();

//     std::cout << "\n\nSET TEST\n";
//     char resp_message_set[] = "*3\r\n$3\r\nSET\r\n$3\r\nkey\r\n$5\r\n12345\r\n";
//     Message set = deserialize(resp_message_set);
//     std::cout << "Type: " << (int)set.type << "\n";
//     std::cout << "Key: " << set.key << "\n";
//     std::cout << "Val: " << set.val << "\n";

//     Message set_resp = redisNode.handle_client_req(set);
//     std::cout << "Type: " << (int)set_resp.type << "\n";
//     std::cout << "Key: " << set_resp.key << "\n";
//     std::cout << "Val: " << set_resp.val << "\n";

//     std::cout << "\n\nGET TEST\n";
//     char resp_message_get[] = "*2\r\n$3\r\nGET\r\n$3\r\nkey\r\n";
//     Message get = deserialize(resp_message_get);
//     std::cout << "Type: " << (int)get.type << "\n";
//     std::cout << "Key: " << get.key << "\n";
//     std::cout << "Val: " << get.val << "\n";

//     Message get_resp = redisNode.handle_client_req(get);
//     std::cout << "Type: " << (int)get_resp.type << "\n";
//     std::cout << "Key: " << get_resp.key << "\n";
//     std::cout << "Val: " << get_resp.val << "\n";

//     return 0;
// }