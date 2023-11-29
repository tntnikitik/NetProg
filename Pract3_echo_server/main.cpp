#include <iostream>
#include <cstdlib> 
#include <cstring> 
#include <unistd.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 

void errorhandler(const char* why, const int exitCode = 1)
{
    std::cerr << why << std::endl;
    exit(exitCode);
}

int main()
{
    sockaddr_in* selfAddr = new (sockaddr_in);
    selfAddr->sin_family = AF_INET;
    selfAddr->sin_port = htons(44214);
    selfAddr->sin_addr.s_addr = INADDR_ANY;
    
    int mySocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (mySocket == -1) {
        errorhandler("Error open socket", 11);
    }

    int rc = bind(mySocket, (const sockaddr*)selfAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errorhandler("Error bind socket with local address", 12);
    }

    while (true) {
        char buf[1024];
        sockaddr_in client_addr;
        socklen_t len = sizeof(sockaddr_in);

        rc = recvfrom(mySocket, buf, 1024, 0, (sockaddr*)&client_addr, &len);
        if (rc == -1) {
            errorhandler("Error receive answer", 15);
        }

        std::cout << "Server receive: " << buf << std::endl;

        rc = sendto(mySocket, buf, 1024, 0, (sockaddr*)&client_addr, len);
        if (rc == -1) {
            errorhandler("Error send message", 16);
        }

        std::cout << "Server send: " << buf << std::endl;
    }

    delete selfAddr;
    return 0;
}
