#include <iostream>
#include <chrono>
#include <ctime>
#include <cstring>
#include <cstdlib>
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
    selfAddr->sin_port = htons(44214);
    selfAddr->sin_addr.s_addr = inet_addr("127.0.0.1");
    int mySocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mySocket == -1) {
        errorhandler("Error open socket", 11);
    }
    int rc = bind(mySocket, (const sockaddr*) selfAddr,
                  sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errorhandler("Error bind socket with local address", 12);
    }
    rc = listen(mySocket, 3);
    if (rc == -1) {
        close(mySocket);
        errorhandler("Error listen socket with local address", 13);
    }
    while (true) {
        char* buf = new char[1024];
        int msgLen = strlen(buf);
        sockaddr_in* remoteAddr = new (sockaddr_in);
        remoteAddr->sin_family = AF_INET; 
        remoteAddr->sin_port = 0;
        remoteAddr->sin_addr.s_addr = 0;
        sockaddr_in * client_addr = new sockaddr_in;
        socklen_t len = sizeof(sockaddr_in);
        int worker;
        worker = accept(mySocket, (sockaddr*)(client_addr), &len);
        if (rc == -1) {
            close(worker);
            errorhandler("Error accept socket with local address", 14);
        }
        rc = recv(worker, buf, 1024, 0);
        if (rc == -1) {
            close(worker);
            errorhandler("Error receive answer", 15);
        }
        buf[rc] = '\0';
        std::cout << "Server receive: " << buf << std::endl;
        auto now = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(now);
        strcpy(buf, std::ctime(&end_time));
        rc = send(worker, buf, 1024, 0);
        if (rc == -1) {
            close(worker);
            errorhandler("Error send message", 16);
        }
        std::cout << "Server send: " << buf << std::endl;
        close(worker);
        delete remoteAddr;
        delete[] buf;
    }
    delete selfAddr;
    return 0;
}
