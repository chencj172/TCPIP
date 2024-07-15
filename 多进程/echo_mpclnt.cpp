#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>

const int BUF_SIZE = 1024;
const int OPSZ = 4;

void error_headling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void handler(int sig)
{
    int status;
    waitpid(-1, &status, WNOHANG);
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
    memset(buf, 0, sizeof(buf));

    struct sigaction sig;
    sig.sa_handler = handler;
    sig.sa_flags = SA_RESTART;
    sigemptyset(&sig.sa_mask);
    sigaction(SIGCHLD, &sig, 0);
    // 创建子进程负责写，父进程负责读
    pid_t pid;
    pid = fork();
    while(true)
    {
        if(pid == 0)
        {
            sleep(0.5);
            std::cout << "please input message : ";
            std::cin >> buf;
            write(clnt_sock, buf, strlen(buf));

            if(strcmp(buf, "q") == 0) 
            {
                close(clnt_sock);
                return 0;
            }
        }
        else
        {
            read(clnt_sock, &buf, sizeof(buf));
            if(strcmp(buf, "q") == 0)
            {
                close(clnt_sock);
                return 0;
            }
            std::cout << "from server result is : " << buf << std::endl;
        }
    }
    
    return 0;
}               