#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, const char* argv[])
{
    char buf[100] = {0};
    int serv_sock;
    sockaddr_in serv_addr;
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    bind(serv_sock, (sockaddr*)&serv_addr, sizeof(sockaddr));
    listen(serv_sock, 5);

    sockaddr_in clnt_addr;
    socklen_t clnt_len = sizeof(clnt_addr);
    int clnt_sock_read = accept(serv_sock, (sockaddr*)&clnt_addr, &clnt_len);
    // 复制套接字
    int clnt_sock_write = dup(clnt_sock_read);
    // 分离IO流
    FILE *readfp, *writefp;
    readfp = fdopen(clnt_sock_read, "r");
    writefp = fdopen(clnt_sock_write, "w");

    // 三次可能只有一次系统调用
    fputs("Hi~clint...", writefp);
    fputs("hhhhhhhhh", writefp);
    fputs("Hi~clint...", writefp);
    fflush(writefp);

    // 半关闭并且发送EOF
    shutdown(fileno(writefp), SHUT_WR);
    fclose(writefp);
    fgets(buf, sizeof(buf), readfp);
    fputs(buf, stdout);
    puts("");
    fclose(readfp);
    
    return 0;
}