#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

const int BUF_SIZE = 1024;
const int OPSZ = 4;

void error_headling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, const char* argv[])
{
    int clnt_sock;
    clnt_sock  =socket(PF_INET, SOCK_STREAM, 0);
    if(clnt_sock == -1)
        error_headling("socket() error");
    
    if(argc != 3)
        error_headling("argc error");
    
    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(sockaddr)) == -1)
        error_headling("connect() error");
    
    char buf[BUF_SIZE];
    int num_count, num, num_res;
    std::cout << "please input num count : ";
    std::cin >> num_count;
    // char和int二进制低八位一样，但表示的含义不同
    buf[0] = static_cast<char>(num_count);

    for(int i=0;i<num_count;i++)
    {
        std::cout << "please input num : ";
        scanf("%d",(int*)&buf[i*OPSZ + 1]);
        // std::cin >> num;
        // memcmp(buf + i*OPSZ + 1, (char*)(&num), sizeof(num));
        // std::cout << (int)buf[i*OPSZ + 1] << std::endl;
    }

    std::cout << "please input operate : ";
    std::cin >> buf[num_count * OPSZ + 1];
    write(clnt_sock, buf, num_count * OPSZ + 2);

    read(clnt_sock, &num_res, sizeof(num_res));
    std::cout << "from server result is : " << num_res << std::endl;
    close(clnt_sock);
    
    return 0;
}