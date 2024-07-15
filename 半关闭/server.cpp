#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void error_handling(const char* msg)
{
    std::cout << msg << std::endl;
    exit(1);
}

const int BUF_SIZE = 30;

int main(int argc, const char* argv[])
{
    int serv_sock, clnt_sock;
    socklen_t clnt_len;
    sockaddr_in serv_addr, clnt_addr;

    if(argc != 2)
        error_handling("argc error");

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (sockaddr*)&serv_addr, sizeof(sockaddr)) == -1)
        error_handling("bind() error");

    if(listen(serv_sock, 6) == -1)
        error_handling("listen() error");

    clnt_sock = accept(serv_sock, (sockaddr*)&clnt_addr, &clnt_len);
    if(clnt_sock == -1)
        error_handling("accept() error");
    FILE* fp = fopen("read.txt", "r");

    int read_len = 0;
    char buf[BUF_SIZE];
    // 读文件
    while(true)
    {
        read_len = fread((void *)buf, 1, BUF_SIZE, fp);
        if(read_len < BUF_SIZE)
        {
            // 表示读完了
            write(clnt_sock, buf, read_len);
            break;
        }
        write(clnt_sock, buf, BUF_SIZE);
    }

    // 断开输出流
    shutdown(clnt_sock, SHUT_WR);

    read(clnt_sock, buf, BUF_SIZE);
    std::cout << "message from clint : " << buf << std::endl;

    fclose(fp);
    close(clnt_sock);
    close(serv_sock);
    return 0;
}