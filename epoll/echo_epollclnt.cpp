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
    perror("ERROR : ");
    std::cerr << msg << std::endl;
    exit(1);
}

int main(int argc, const char* argv[])
{
    int clnt_sock;
    sockaddr_in serv_addr;
    char buf[30];
    std::string str;
    if(argc != 2)
        error_handling("argc error");

    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(connect(clnt_sock, (sockaddr*)&serv_addr, sizeof(sockaddr)) == -1)
        error_handling("connect() error");
    
    while(1)
    {
        memset(buf, 0, sizeof(buf));
        std::cout << "please input message : ";
        std::cin >> buf;
        // std::getline(std::cin, str);
        // if(strcmp("q", buf) == 0) break;
        // if(str == "q") break;
        if(strcmp(buf, "q") == 0) break;
        // write(clnt_sock, str.c_str(), sizeof(str));
        write(clnt_sock, buf, strlen(buf));

        // int len = read(clnt_sock, buf, sizeof(buf));
        // buf[len] = 0;
        // std::cout << "message from server : " << buf << std::endl;
    }

    close(clnt_sock);
    return 0;
}