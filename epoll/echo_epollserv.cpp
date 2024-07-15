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
#include <sys/epoll.h>

const int BUF_SIZE = 1024;
const int EPOLL_SIZE = 50;

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
    char buf[BUF_SIZE];
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
    
    // 最多等待7个未连接的客户端
    if(listen(serv_sock, 7) == -1)
        error_handling("listen() error");

    // 设置监听套接字的事件
    struct epoll_event event;
    // 返回活动套接字的集合
    struct epoll_event *ep_events;
    ep_events = (struct epoll_event*)malloc(sizeof(struct epoll_event) * EPOLL_SIZE);
    int epfd, event_cnt;
    // 创建epoll例程
    epfd = epoll_create(EPOLL_SIZE);
    // 设置监听的套接字
    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLLIN, serv_sock, &event);

    while(1)
    {
        // 阻塞等待客户端活跃
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if(event_cnt == -1)
            error_handling("epoll() error");
        for(int i=0;i<event_cnt;i++)
        {
            // std::cout << ep_events[i].data.fd << std::endl;
            if(ep_events[i].data.fd == serv_sock)
            {
                // 客户端请求连接
                clnt_len = sizeof(clnt_addr);
                clnt_sock = accept(serv_sock, (sockaddr*)&clnt_addr, &clnt_len);
                if(clnt_sock == -1) 
                    error_handling("accept error()");
                event.events = EPOLLIN;
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
                std::cout << "connected...\n";
            }
            else
            {
                int str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
                if(str_len == 0)
                {
                    // 关闭连接
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                    close(ep_events[i].data.fd);
                    std::cout << "disconnected...\n";
                }
                else 
                {
                    buf[str_len] = '0';
                    std::cout << "from client message : " << buf << std::endl;
                    write(ep_events[i].data.fd, "accept" , strlen("accept"));
                }
            }
        }
    }

    close(serv_sock);
    close(epfd);
    return 0;
}