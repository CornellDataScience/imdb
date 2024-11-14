// TcpClient.hpp
#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <string>

class TcpClient {
public:
    TcpClient(const std::string& server_ip, int server_port);
    ~TcpClient();

    bool connectToServer();
    void sendMessage(const std::string& message);
    std::string receiveMessage();

private:
    std::string server_ip_;
    int server_port_;
    int client_socket_;
};

#endif // TCPCLIENT_HPP
