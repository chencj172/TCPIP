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
    char buf[40] = {0};
    int clnt_sock;
    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    connect(clnt_sock, (sockaddr*)&serv_addr, sizeof(sockaddr));
    while(1)
    {
        if(read(clnt_sock, buf, sizeof(buf)) == 0)
            break;
        
        std::cout << buf << std::endl;
    }
    write(clnt_sock, "Thank you..", strlen("Thank you.."));

    close(clnt_sock);
    return 0;
}