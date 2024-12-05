#include <string>
#include <iostream>
#include <functional>

class TcpServer
{
public:
  TcpServer(int port);
  void receive(Message &message, char *buffer);
  void respond(Message message);
  ~TcpServer();

private:
  std::string messageInput;
  struct sockaddr_in address;
  int server_fd, new_socket;
  socklen_t addrlen;
};
