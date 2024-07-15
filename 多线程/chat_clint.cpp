#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

const int BUF_SIZE = 100;
void* Read(void* arg);
void* Write(void* arg);
void error_headling(const char*);
pthread_t thread_read, thread_write;
int main(int argc, const char* argv[])
{
    if(argc != 3)
        error_headling("argc error");

    int clnt_sock;
    sockaddr_in serv_addr;
    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(clnt_sock == -1)
        error_headling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if((connect(clnt_sock, (sockaddr*)&serv_addr, sizeof(sockaddr))) == -1)
        error_headling("connect() error");

    // 开线程去负责读写，并分离IO
    int clnt_socktmp = dup(clnt_sock);
    FILE *readptr = fdopen(clnt_sock, "r");
    FILE *writeptr = fdopen(clnt_socktmp, "w");
    pthread_create(&thread_read, nullptr, Read, (void*)readptr);
    pthread_create(&thread_write, nullptr, Write, (void*)writeptr);
    pthread_join(thread_read, nullptr);
    pthread_join(thread_write, nullptr);

    return 0;
}

void error_headling(const char* msg)
{
    puts(msg);
    perror("ERROR");
    exit(1);
}

void* Read(void* arg)
{
    FILE *readptr = (FILE*)arg;
    char buf[BUF_SIZE];
    memset(buf, 0, sizeof(buf));
    while(1)
    {
        // int read_len = fread(buf, 1, strlen("accepted") + 7, readptr);
        // if(read_len == 0) break;
        // buf[read_len] = '\0';
        // std::cout << "read_len = " << read_len << std::endl;
        if(!fgets(buf, sizeof(buf), readptr)) break;
        fflush(readptr);
        std::cout << "from server message : " << buf ;
    }
    shutdown(fileno(readptr), SHUT_RD);
}

void* Write(void* arg)
{
    FILE* writeptr = (FILE*)arg;
    char buf[BUF_SIZE];
    memset(buf, 0, sizeof(buf));
    while(1)
    {
        std::cout << "please input msg : \n" ;
        std::cin >> buf;
        if(strcmp(buf, "q") == 0) break;
        fputs(buf, writeptr);
        fflush(writeptr);
    }
    shutdown(fileno(writeptr), SHUT_WR);
}