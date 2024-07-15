#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

const int BUF_SIZE = 30;
void error_handling(const char *msg)
{
    std::cout << msg << std::endl;
    perror("ERROR : ");
    exit(1);
}

int main(int argc, const char* argv[])
{
    if(argc != 3)
        error_handling("argc error");
    int clnt_sock;
    char buf[BUF_SIZE];
    sockaddr_in clnt_addr, serv_addr;
    socklen_t serv_len;
    clnt_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(clnt_sock == -1)
        error_handling("socket() error");
    
    memset(&clnt_addr, 0, sizeof(clnt_addr));
    clnt_addr.sin_family = PF_INET;
    clnt_addr.sin_port = htons(atoi(argv[2]));
    clnt_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(clnt_sock, (sockaddr*)&clnt_addr, sizeof(sockaddr)) == -1)
        error_handling("bind() error");
    
    struct ip_mreq join_adr;
    join_adr.imr_multiaddr.s_addr = inet_addr(argv[1]);
    join_adr.imr_interface.s_addr = htonl(INADDR_ANY);
    setsockopt(clnt_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_adr, sizeof(join_adr));

    while(1)
    {
        int str_len = recvfrom(clnt_sock, buf, BUF_SIZE, 0, (sockaddr*)&serv_addr, &serv_len);
        if(str_len < 0) break;

        std::cout << buf << std::endl;
    }

    close(clnt_sock);
    return 0;
}