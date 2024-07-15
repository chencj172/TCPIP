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

void error_handling(const char* msg)
{
    perror("ERROR");
    std::cerr << msg << std::endl;
    exit(1);
}

int main(int argc, const char* argv[])
{
    int serv_sock, clnt_sock;
    sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_len;
    char buf[1024];
    if(argc != 2)
        error_handling("argc error");
    
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (sockaddr*)&serv_addr, sizeof(sockaddr)) == -1)
        error_handling("bind() error");
    
    if(listen(serv_sock, 7) == -1)
        error_handling("listen() error");

    fd_set reads, tmps;
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    int max_num = serv_sock;

    while(1)
    {
        tmps = reads;
        // 每次检查看是否有活动的socket
        int act_num = select(max_num + 1, &tmps, 0, 0, 0);
        if(act_num == -1)
            error_handling("select() error");
        else if(act_num == 0)
        continue;
        else
        {
            for(int i=0;i<max_num + 1; i++)
            {
                // 检查监听的socket
                if(FD_ISSET(i, &tmps))
                {
                    if(i == serv_sock)
                    {
                        // 说明有新的客户端请求连接
                        clnt_sock = accept(serv_sock, (sockaddr*)&clnt_addr, &clnt_len);
                        if(clnt_sock == -1) error_handling("accept() error");
                        if(clnt_sock > max_num) max_num = clnt_sock;
                        FD_SET(clnt_sock, &reads);
                        std::cout << "connected...\n";
                    }
                    else
                    {
                        // 说明客户端有活动
                        // int len = 0, index = 0;
                        // while((len = read(i, buf, sizeof(buf))) != 0)
                        // {
                        //     index += len;
                        // }
                        int len = read(i, buf, sizeof(buf));
                        buf[len] = 0;
                        if(len ==0)
                        {
                            std::cout << "disconncted...\n";
                            FD_CLR(i, &reads);
                            close(i);
                        }
                        else
                        {
                            std::cout << "message from client : " << buf << std::endl;
                            write(i, "accepted", 9);
                        }
                    }
                }
            }
        }
    }

    close(serv_sock);
    return 0;
}