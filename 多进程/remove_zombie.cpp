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

void handler(int sig)
{
    int status;
    // 处理子进程
    if(sig == SIGCHLD)
        std::cout << "exit...\n";

    waitpid(-1, &status, WNOHANG);
    // 如果正常中止就打印子进程的返回值
    if(WIFEXITED(status))
        std::cout << "Child return: " << WEXITSTATUS(status) << std::endl;
}
int main(int argc, const char* argv[])
{
    struct sigaction mysighadler;
    mysighadler.sa_handler = handler;
    sigemptyset(&mysighadler.sa_mask);
    mysighadler.sa_flags = 0;
    sigaction(SIGCHLD, &mysighadler, 0);

    pid_t pid;
    pid = fork();

    if(pid == 0)
    {
        sleep(5);
        exit(2);
    }
    else
    {
        pid = fork();
        if(pid == 0)
        {
            sleep(2);
            exit(3);
        }
        else
        {
            // 每次子进程中止都会唤醒当前sleep的父进程，然后跳出sleep状态
            for(int i=0;i<5;i++)
            {
                std::cout << "wait...\n";
                sleep(5);
            }
        }
    }
    return 0;
}