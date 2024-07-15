#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void error_headling(const char *);

int main(int argc, const char* argv[])
{
    // 定义服务端和客户端socket
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[30];
    char send_message[] = "accepted";

    // 检查有没有输入端口
    if(argc != 2)
    {
        std::cout << "Usage : " << argv[0] << "<port>\n";
        exit(1);
    }

    // 创建socket
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_headling("socket() error");

    // 创建服务端地址结构
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family =  AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    // 地址结构与socket进行绑定
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_headling("bind() error");

    // 进行监听
    if(listen(serv_sock, 5) == -1)
        error_headling("listen() error");

    int str_len;
    // 只能支持5个客户端
    for(int i=0;i<5;i++)
    {
        std::cout << "current index = " << i << std::endl;
        // 返回一个套接字用于数据的传输
        clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
        std::cout << "server created socket : " << clnt_sock << ", client port : " << ntohs(clnt_addr.sin_port)  << std::endl;
        if(clnt_sock == -1)
                error_headling("accept() error");

        // 不能进行并发，如果服务于一个客户端，就要一直服务，直到客户输入'###'
        while((str_len = read(clnt_sock, message, sizeof(message)  -1)) != 0)
        {
            std::cout << "read len : " << str_len << std::endl;
            std::cout << "message from clint : " << message << std::endl;
            write(clnt_sock, send_message, sizeof(send_message) - 1);
        }
        close(clnt_sock);
    }
    close(serv_sock);

    return 0;
}

void error_headling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}