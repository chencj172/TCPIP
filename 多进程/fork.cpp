#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int globalval = 10;
int main(int argc, const char* argv[])
{
    int localval = 20;

    // 创建进程
    pid_t pid = fork();

    // 子进程中pid为0，父进程中pid为子进程的pid
    // 两个进程同用一份程序
    if(pid == 0)
    globalval += 10, localval += 1;
    else 
    globalval += 1, localval += 10;

    if(pid == 0)
    std::cout << "child : " << globalval << " " << localval << std::endl;
    else 
    std::cout << "father : " << globalval << " " << localval << std::endl;
    return 0;
}