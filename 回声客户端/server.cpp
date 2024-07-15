#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <vector>

int operate(const std::vector<int>&v, char op)
{
    int res = 0;
    if(op == '+')
    {
        for(auto num : v)
        res += num;
    }
    else if(op == '-')
    {
        for(auto num : v)
        res -= num;
    }
    else if(op == '*')
    {
        res = 1;
        for(auto num : v)
        res *= num;
    }
    return res;
}

void error_headling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, const char* argv[])
{
    int serv_sock;
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_headling("socket() error");

    if(argc != 2)
        error_headling("argc error");
    
    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(serv_sock, (sockaddr *)&serv_addr, sizeof(sockaddr)) == -1)
        error_headling("bind() error");
    
    if(listen(serv_sock, 5) == -1)
        error_headling("listen error");
    
    int clnt_sock;
    socklen_t clnt_len;
    sockaddr_in clnt_addr;
    int data_num, num_count;
    char op;
    std::vector<int>operate_num;
    // 支持五个客户端
    for(int i=0;i<5;i++)
    {
        int res = 0;
        std::cout << "current index : " << i << std::endl;
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_len);
        if(clnt_sock == -1)
            error_headling("accept() error");

        std::cout << "==========connected...===============\n";
        
        // 接收数字个数
        char numCount;
        read(clnt_sock, &numCount, 1);
        for(int j=0;j<(int)numCount;j++)
        {
            read(clnt_sock, (int*)&data_num, sizeof(data_num));
            operate_num.push_back(data_num);
        }

        read(clnt_sock, (char*)&op, sizeof(op));

        // 处理读到的数据
        if(op == '+' || op == '-' || op == '*')
        {
            res = operate(operate_num, op);
            write(clnt_sock, (char*)&res, sizeof(res));
            operate_num.clear();
            std::cout << "res = " << res << std::endl;
        }
        std::cout << "==========disconnected...=============\n";

        close(clnt_sock);
    }

    close(serv_sock);
    return 0;
}