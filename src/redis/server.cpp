#include "server.hpp"
#include "parse.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <unistd.h>

#define BUFFER_SIZE 1024
/**
 *     // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "Socket creation failed\n";
        return -1;
    }

    // Forcefully attach socket to the port 8080
    if (setsockopt(server_fd, IPPROTO_TCP, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        std::cerr << "setsockopt failed\n";
        close(server_fd);
        return -1;
    }

    // Define the address structure
    address.sin_family = AF_INET;         // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Any IP address
    address.sin_port = htons(PORT);       // Port number

    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Bind failed\n";
        close(server_fd);
        return -1;
    }

    // Start listening for connections
    if (listen(server_fd, 3) < 0)
    {
        std::cerr << "Listen failed\n";
        close(server_fd);
        return -1;
    }
 */
// pass in the ip in the port, handle the bind socket in constructor
// constructor will be calling networking code

TcpServer::TcpServer(int port)
{

    // int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    address.sin_family = AF_INET;         // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Any IP address
    address.sin_port = htons(port);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "Socket creation failed\n";
    }

    // Forcefully attach socket to the port 8080
    if (setsockopt(server_fd, IPPROTO_TCP, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        std::cerr << "setsockopt failed\n";
        close(server_fd);
    }

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Bind failed\n";
        close(server_fd);
    }

    if (listen(server_fd, 3) < 0)
    {
        std::cerr << "Listen failed\n";
        close(server_fd);
    }
}

void TcpServer::respond(Message message)
{
    char resp_buff[BUFFER_SIZE] = {0};
    serialize(message, resp_buff);
    if (send(new_socket, resp_buff, strlen(resp_buff), 0) < 0)
    {
        std::cerr << "[ERROR] Failed to send message: " << strerror(errno) << std::endl;
    }
}

void TcpServer::receive(Message &message, char *buffer)
{
    // char buffer[BUFFER_SIZE] = {0};

    while (true)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            std::cerr << "Accept failed: " << strerror(errno) << std::endl;
            continue;
        }
        std::cout << "Connection accepted\n";

        // Persistent connection with client
        while (true)
        {
            int bytes_received = recv(new_socket, buffer, BUFFER_SIZE - 1, 0);
            if (bytes_received > 0)
            {
                buffer[bytes_received] = '\0';
                Message msg = deserialize(buffer);
                // Message resp = redisNode.handle_client_req(msg);
                // char resp_buff[BUFFER_SIZE] = {0};
                // serialize(resp, resp_buff);
                // if (send(new_socket, resp_buff, strlen(resp_buff), 0) < 0)
                // {
                //     std::cerr << "[ERROR] Failed to send message: " << strerror(errno) << std::endl;
                //     break;
                // }
            }
            else if (bytes_received == 0)
            {
                std::cout << "Client disconnected\n";
                break;
            }
            else
            {
                std::cerr << "[ERROR] recv failed: " << strerror(errno) << std::endl;
                break;
            }
        }

        close(new_socket);
    }
}
