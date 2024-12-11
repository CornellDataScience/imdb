#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <sstream>

#include <vector>
#include "parse.hpp"

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

// awful awful awful
static const std::unordered_map<MessageType, std::string> enumToStr = {
{MessageType::SET, "SET"},
{MessageType::DEL, "DEL"},
{MessageType::GET, "GET"},
{MessageType::OK, "OK"},
// {MessageType::GETRANGE, "GET_RANGE"},
{MessageType::RETURN, "RETURN"},
{MessageType::APPEND, "APPEND"},
{MessageType::DECRBY, "DECRBY"},
{MessageType::GETDEL, "GETDEL"}

    };

std::vector<std::string> pp_msg(const Message &msg)
{
    std::vector<std::string> result;
    result.push_back(enumToStr.at(msg.type));
    result.push_back(msg.key);
    result.push_back(msg.val);
    return result;
}

std::vector<std::string> split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}

int main(int argc, char *argv[])
{
    // Define the server IP address (your friend's IP address)
    if (argc != 2)
    {
        std::cerr << "[ERROR] Usage: " << argv[0] << " <server_ip>" << std::endl;
        return -1;
    }
    const char *server_ip = argv[1];
    std::cout << "[DEBUG] Starting TCP client..." << std::endl;

    // Create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        std::cerr << "[ERROR] Socket creation failed: " << strerror(errno) << std::endl;
        return -1;
    }
    std::cout << "[DEBUG] Socket created successfully." << std::endl;

    // Define the server address
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);

    // Convert IP address from text to binary form
    int inet_pton_result = inet_pton(AF_INET, server_ip, &server_address.sin_addr);
    if (inet_pton_result <= 0)
    {
        if (inet_pton_result == 0)
        {
            std::cerr << "[ERROR] Invalid IP address format: " << server_ip << std::endl;
        }
        else
        {
            std::cerr << "[ERROR] inet_pton failed: " << strerror(errno) << std::endl;
        }
        close(client_socket);
        return -1;
    }
    std::cout << "[DEBUG] Server address set to " << server_ip << ":" << SERVER_PORT << std::endl;

    // Connect to the server
    std::cout << "[DEBUG] Attempting to connect to the server..." << std::endl;
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        std::cerr << "[ERROR] Connection to the server failed: " << strerror(errno) << std::endl;
        close(client_socket);
        return -1;
    }
    std::cout << "[DEBUG] Connected to the server." << std::endl;

    // Message to send to the server
    while (true)
    {
        std::string input;
        std::getline(std::cin, input);
        if (input == "exit")
        {
            break;
        }
        std::vector<std::string> tokens = split(input, ' ');
        std::string ser = serializeArray(tokens);
        const char *message = ser.c_str();

        ssize_t bytes_sent = send(client_socket, message, strlen(message), 0);
        if (bytes_sent < 0)
        {
            std::cerr << "[ERROR] Failed to send message: " << strerror(errno) << std::endl;
            close(client_socket);
            return -1;
        }

        char buffer[BUFFER_SIZE] = {0};
        ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received > 0)
        {
            buffer[bytes_received] = '\0'; // Null-terminate the received data
            Message resp = deserialize(buffer);
            std::vector<std::string> pp = pp_msg(resp);
            std::cout << "[DEBUG][RESPONSE] " << pp[0] << " " << pp[1] << " " << pp[2] << std::endl;
        }

        else
        {
            std::cerr << "[ERROR] Failed to receive message: " << strerror(errno) << std::endl;
        }
    }

    std::cout << "[DEBUG] Closing the socket." << std::endl;
    close(client_socket);
    std::cout << "[DEBUG] Client terminated successfully." << std::endl;
    return 0;
}