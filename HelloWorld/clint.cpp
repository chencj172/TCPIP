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
    int clnt_sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len = 0;

    if(argc != 3)
    {
        std::cout << "Usage : " << argv[0] << "<port>\n";
        exit(1);
    }

    // 创建socket
    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);


    if(clnt_sock == -1)
        error_headling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family =  AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // 向服务器发送连接请求
    if(connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_headling("connect() error");
    
    int idx = 0, read_len = 0;
    while(read_len = read(clnt_sock, &message[idx++], 1 ))
    {
        if(read_len == -1)
            error_headling("read() error");

        str_len += read_len;
    }
    // str_len = read(clnt_sock, message, sizeof(message) - 1);
    // if(str_len == -1)
    //     error_headling("read() error");
    std::cout << "read count : " << str_len << std::endl;
    std::cout << "message from server :  " << message << std::endl;
    close(clnt_sock);

    return 0;
}

void error_headling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}