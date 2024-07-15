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
    int fds[2];
    pid_t pid;

    pipe(fds);
    pid = fork();
    char buf[20];
    memset(buf, 0, sizeof(buf));
    if(pid == 0)
    {
        write(fds[1], "i am child", 10);
        return 0;
    }
    else
    {
        read(fds[0], buf, sizeof(buf));
        std::cout << buf << std::endl;
    }
    return 0;
}