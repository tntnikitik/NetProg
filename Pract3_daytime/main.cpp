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
int main(int argc, char** argv)
{
    sockaddr_in* selfAddr = new (sockaddr_in);
    selfAddr->sin_family = AF_INET;
    selfAddr->sin_port = 0;
    selfAddr->sin_addr.s_addr = 0;
    sockaddr_in* remoteAddr = new (sockaddr_in);
    remoteAddr->sin_family = AF_INET;
    remoteAddr->sin_port = htons(44214);
    remoteAddr->sin_addr.s_addr = inet_addr("127.0.0.1"); 
    char* buf = new char[256];
    strcpy(buf, "hi its tcp client\n");
    int msgLen = strlen(buf);
    int mySocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mySocket == -1) {
        errorhandler("Error open socket", 11);
    }
    int rc = bind(mySocket, (const sockaddr*)selfAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errorhandler("Error bind socket with local address", 12);
    }
    rc = connect(mySocket, (const sockaddr*)remoteAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errorhandler("Error connect socket with remote server", 13);
    }
    rc = send(mySocket, buf, msgLen, 0);
    if (rc == -1) {
        close(mySocket);
        errorhandler("Error send message", 14);
    }
    std::cout << "Client send: " << buf << std::endl;
    rc = recv(mySocket, buf, 256, 0);
    if (rc == -1) {
        close(mySocket);
        errorhandler("Error receive answer", 15);
    }
    buf[rc] = '\0';
    std::cout << "Client receive: " << buf << std::endl;
    close(mySocket);
    delete selfAddr;
    delete remoteAddr;
    delete[] buf;
    return 0;
}
