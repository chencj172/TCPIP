#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>

int main(int argc, const char* argv[])
{
    fd_set reads, tmps;
    char buf[30];
    struct timeval timeout;

    // 初始化fd_set数组
    FD_ZERO(&reads);
    // 监控标准输入
    FD_SET(0, &reads);

    while(true)
    {
        tmps = reads;
        // 秒优先级更高
        timeout.tv_sec = 3;
        // timeout.tv_usec = 5000;

        int result = select(1, &tmps, 0, 0, &timeout);
        if(result == -1)
        {
            std::cerr << "select() error\n";
            exit(1);
        }
        else if(result == 0)
        {
            std::cout << "time out...\n";
        }
        else 
        {
            if(FD_ISSET(0, &tmps))
            {
                memset(buf, 0, sizeof(buf));
                read(0, buf, sizeof(buf));
                std::cout << buf;
            }
        }
    }
    return 0;
}