#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

const int BUF_SIZE = 100;
const int MAX_CLINT = 256;
void* handle_clnt(void* arg);
void error_headling(const char*);
pthread_mutex_t mutex;
std::vector<int>clnt_socks(MAX_CLINT);

int main(int argc, const char* argv[])
{
    if(argc != 2)
        error_headling("argc error");
    
    pthread_t t_id;
    int serv_sock, clnt_sock;
    socklen_t clnt_len;
    sockaddr_in serv_addr, clnt_addr;
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) 
        error_headling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if((bind(serv_sock, (sockaddr*)&serv_addr, sizeof(sockaddr))) == -1)
        error_headling("bind() error");
    
    listen(serv_sock, 5);

    while(1)
    {
        clnt_sock = accept(serv_sock, (sockaddr*)&clnt_addr, &clnt_len);
        pthread_mutex_lock(&mutex);
        clnt_socks.push_back(clnt_sock);
        pthread_mutex_unlock(&mutex);
        
        // 开一个线程去服务客户端
        pthread_create(&t_id, nullptr, handle_clnt, (void*)&clnt_sock);
        pthread_detach(t_id);
        puts("Connected...");
    }

    close(serv_sock);
    return 0;
}

// 每个线程有自己的栈空间
void* handle_clnt(void* arg)
{
    char buf[BUF_SIZE];
    int  clnt_sock = *((int*)arg);
    memset(buf, 0, sizeof(buf));
    while(1)
    {
        int read_len = read(clnt_sock, buf, sizeof(buf));
        if(read_len == 0) break;
        buf[read_len] = '\0';
        std::cout << "from clint message : " << buf << std::endl;
        write(clnt_sock, "accepted\n", strlen("accepted\n"));
    }
    std::cout << "disconnected...\n";
    pthread_mutex_lock(&mutex);
    clnt_socks.erase(std::find(clnt_socks.begin(), clnt_socks.end(), clnt_sock));
    pthread_mutex_unlock(&mutex);
    close(clnt_sock);
}

void error_headling(const char* msg)
{
    puts(msg);
    perror("ERROR");
    exit(1);
}