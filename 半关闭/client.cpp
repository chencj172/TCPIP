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
    int clnt_sock;
    sockaddr_in serv_addr;

    if(argc != 3)
        error_handling("argc error");

    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(clnt_sock == -1)
        error_handling("socket() error");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(clnt_sock, (sockaddr*)&serv_addr, sizeof(sockaddr)) == -1)
        error_handling("connect() error");
    
    char buf[BUF_SIZE];
    FILE *fp = fopen("write.txt", "w");
    int read_len = 0;
    while(true)
    {
        read_len = read(clnt_sock, buf, BUF_SIZE);
        if(read_len < BUF_SIZE)
        {
            fwrite((void*)buf, 1, read_len, fp);
            break;
        }
        fwrite((void*)buf, 1, BUF_SIZE, fp);
    }
    shutdown(clnt_sock, SHUT_RD);
    write(clnt_sock, "thanks", 10);

    fclose(fp);
    close(clnt_sock);
    return 0;
}