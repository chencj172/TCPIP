#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/wait.h>

const int BUF_SIZE = 1024;
void error_handling(const char* msg)
{
    std::cout << msg << std::endl;
    perror("Error msg");
    exit(1);
}

void handler(int sig)
{
    // 处理客户端断开的函数
    std::cout << "disconnected...\n";
    int status;
    waitpid(-1, &status, WNOHANG);
}

int main(int argc, const char* argv[])
{
    if(argc != 2)
        error_handling("argc error");

    int serv_sock, clnt_sock;
    socklen_t clnt_len;
    sockaddr_in serv_addr, clnt_addr;
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(sockaddr)) == -1)
        error_handling("bind() error");

    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    struct sigaction mysig;
    mysig.sa_handler = handler;
    sigemptyset(&mysig.sa_mask);
    // 加上这个就会使被中断的主进程回到被中断的地方继续执行，否则在执行accept函数时候会被中断
    mysig.sa_flags = SA_RESTART ;
    sigaction(SIGCHLD,  &mysig, 0);
    char buf[BUF_SIZE];

    pid_t pid;
    while(true)
    {
        clnt_len = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_len);
        if(clnt_sock == -1)
            error_handling("accept() error");

        std::cout << "connected...\n";
        // 创建进程负责处理请求
        pid = fork();
        if(pid == 0)
        {
            close(serv_sock);
            // int read_len = 0, index = 0;
            // 只有客户端退出才跳出循环
            // while((read_len = read(clnt_sock, &buf[index], BUF_SIZE)) != 0)
            // {
            //     std::cout << read_len << std::endl;
            //     if(read_len == -1) break;
            //     index += read_len;
            // }
            // buf[index] = '\0';
            // memset(buf, 0, sizeof(buf));
            // read(clnt_sock, buf, BUF_SIZE);
            memset(buf, 0, sizeof(buf));
            while(read(clnt_sock, buf, BUF_SIZE) != -1)
            {
                if(strcmp(buf, "q") == 0)
                {
                    write(clnt_sock, buf, sizeof(buf));
                    break;
                }
                std::cout << buf << std::endl;
                write(clnt_sock, "accepted", 10); 
                memset(buf, 0, sizeof(buf));
            }
            close(clnt_sock);

            return 0;
        }
        else
            close(clnt_sock);
    }

    close(serv_sock);

    return 0;
}