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

    // 这样就可以使用read，write
    connect(serv_sock, (sockaddr*)&serv_addr, sizeof(sockaddr));

    std::cout << "please input message : ";
    std::cin >> message;
    int send_len = write(serv_sock, message, sizeof(message));
    std::cout << "send message len : " << send_len << std::endl;

    std::cout << "please input message again : ";
    std::cin >> message;
    send_len = write(serv_sock, message, sizeof(message));
    std::cout << "send message len : " << send_len << std::endl;
    memset(message, 0, sizeof(message));
    read(serv_sock, message, sizeof(message));
    std::cout << "from server : " << message << std::endl;

    return 0;
}