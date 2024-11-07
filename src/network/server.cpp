#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    int buffer[1024];
    if(listening == -1){
        std::cerr << "no socket for you"<< std::endl;
        return -1;
    }
    std::cout << "TCP Test\n";
    return 0;
}