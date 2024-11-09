#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    // Define the server IP address (your friend's IP address)
    const char* server_ip = argv[1]; // Replace this with your friend's IP address
    std::cout << "[DEBUG] Starting TCP client..." << std::endl;

    // Create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
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
    if (inet_pton_result <= 0) {
        if (inet_pton_result == 0) {
            std::cerr << "[ERROR] Invalid IP address format: " << server_ip << std::endl;
        } else {
            std::cerr << "[ERROR] inet_pton failed: " << strerror(errno) << std::endl;
        }
        close(client_socket);
        return -1;
    }
    std::cout << "[DEBUG] Server address set to " << server_ip << ":" << SERVER_PORT << std::endl;

    // Connect to the server
    std::cout << "[DEBUG] Attempting to connect to the server..." << std::endl;
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "[ERROR] Connection to the server failed: " << strerror(errno) << std::endl;
        close(client_socket);
        return -1;
    }
    std::cout << "[DEBUG] Connected to the server." << std::endl;

    // Message to send to the server
    const char* message = argv[2];
    ssize_t bytes_sent = send(client_socket, message, strlen(message), 0);
    if (bytes_sent < 0) {
        std::cerr << "[ERROR] Failed to send message: " << strerror(errno) << std::endl;
        close(client_socket);
        return -1;
    }
    std::cout << "[DEBUG] Message sent to server: " << message << std::endl;

    // Receive response from the server
    char buffer[BUFFER_SIZE] = {0};
    std::cout << "[DEBUG] Waiting for response from server..." << std::endl;
    ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';  // Null-terminate the received data
        std::cout << "[DEBUG] Message received from server: " << buffer << "\n";
    } else if (bytes_received == 0) {
        std::cout << "[DEBUG] Server closed the connection." << std::endl;
    } else {
        std::cerr << "[ERROR] Failed to receive message: " << strerror(errno) << std::endl;
    }

    // Close the socket
    std::cout << "[DEBUG] Closing the socket." << std::endl;
    close(client_socket);
    std::cout << "[DEBUG] Client terminated successfully." << std::endl;
    return 0;
}