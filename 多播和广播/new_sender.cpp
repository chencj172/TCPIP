#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

const int TTL = 64;
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
    int serv_sock;
    int time_live = TTL;
    sockaddr_in clnt_addr;
    char buf[BUF_SIZE] = {0};
    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    
    memset(&clnt_addr, 0, sizeof(clnt_addr));
    clnt_addr.sin_family = AF_INET;
    clnt_addr.sin_port = htons(atoi(argv[2]));
    clnt_addr.sin_addr.s_addr = inet_addr(argv[1]);

    setsockopt(serv_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));

    FILE *fp;
    fp = fopen("text.txt", "r");
    while(fread(buf, 1, BUF_SIZE, fp))
    {
        sendto(serv_sock, buf, strlen(buf), 0, (sockaddr*)&clnt_addr, sizeof(sockaddr));
        sleep(1);
        memset(buf, 0, sizeof(buf));
    }

    fclose(fp);
    close(serv_sock);
    return 0;
}