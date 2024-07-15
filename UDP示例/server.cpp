#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void error_headling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, const char* argv[])
{
    int serv_sock;
    char message[1024];

    if(argc != 2)
        error_headling("argc error");

    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == -1)
        error_headling("socket() error");

    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (sockaddr*)&serv_addr, sizeof(sockaddr)) == -1)
        error_headling("bind() error");

    sockaddr_in clnt_addr;
    memset(&clnt_addr, 0, sizeof(clnt_addr));
    socklen_t clnt_len = sizeof(clnt_addr);
    while(true)
    {
        int recv_len = recvfrom(serv_sock, message, sizeof(message) -1, 0, (struct sockaddr*)&clnt_addr, &clnt_len);
        std::cout << "=====================\n";
        std::cout << "client IP : " << inet_ntoa(clnt_addr.sin_addr) << std::endl;
        std::cout << "client PORT : " << ntohs(clnt_addr.sin_port) << std::endl;
        std::cout << "client MESSAGE : " << message << std::endl;
        std::cout << "recv len : " << recv_len << std::endl;
        std::cout << "=====================\n";

        memset(message, 0, sizeof(message));
        sendto(serv_sock, "accepted", 10, 0, (sockaddr*)&clnt_addr, clnt_len);
    }

    return 0;
}