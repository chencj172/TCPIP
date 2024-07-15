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
    int clnt_sock = accept(serv_sock, (sockaddr*)&clnt_addr, &clnt_len);
    // 三次write都会系统调用
    write(clnt_sock, "hhhh", strlen("hhhh"));
    write(clnt_sock, "tttttttt", strlen("tttttttt"));
    write(clnt_sock, "hhhh", strlen("hhhh"));

    // 半关闭并且发送EOF
    shutdown(clnt_sock, SHUT_WR);
    read(clnt_sock, buf, sizeof(buf));
    std::cout << buf << std::endl;

    close(clnt_sock);
    close(serv_sock);
    return 0;
}