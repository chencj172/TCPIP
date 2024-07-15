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

    if(argc != 3)
        error_headling("argc error");

    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == -1)
        error_headling("socket() error");

    sockaddr_in serv_addr, recv_addr;
    socklen_t recv_len;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));

    connect(serv_sock, (sockaddr*)&serv_addr, sizeof(sockaddr));

    std::cout << "please input message : ";
    std::cin >> message;
    int send_len = sendto(serv_sock, message, strlen(message), 0, (struct sockaddr*)&serv_addr, sizeof(sockaddr));
    std::cout << "send message len : " << send_len << std::endl;

    std::cout << "please input message again : ";
    std::cin >> message;
    send_len = sendto(serv_sock, message, strlen(message), 0, (struct sockaddr*)&serv_addr, sizeof(sockaddr));
    std::cout << "send message len : " << send_len << std::endl;
    memset(message, 0, sizeof(message));
    recvfrom(serv_sock, message, sizeof(message) -1, 0, (sockaddr*)&recv_addr, &recv_len);
    std::cout << "from server : " << message << std::endl;

    return 0;
}