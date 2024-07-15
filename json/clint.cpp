#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <jsoncpp/json/json.h>
#include "people.h"

void error_headling(const char *);

// 将对象转成json
std::string getJsonString(const people& p);

int main(int argc, const char* argv[])
{
    int clnt_sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len = 0;
    if(argc != 3)
    {
        std::cout << "Usage : " << argv[0] << "<port>\n";
        exit(1);
    }

    // 创建socket
    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
    std::cout << "clint : " << clnt_sock << std::endl;

    if(clnt_sock == -1)
        error_headling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family =  AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // 向服务器发送连接请求
    if(connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_headling("connect() error");
    else 
        std::cout << "Connected...\n";

    people p;
    int cnt;
    std::vector<std::string>hobby;
    std::string h;
    while(true)
    {
        // 向服务端写
        std::cout << "please input name : ";
        std::cin >> p.name;
        std::cout << "please input age : ";
        std::cin >> p.age;
        std::cout << "please input sex : ";
        std::cin >> p.sex;
        std::cout << "please input hobby count : ";
        std::cin >> cnt;
        for(int i=0;i<cnt;i++)
        {
            std::cout << "please input hobby " << i << ":";
            std::cin >> h;
            hobby.push_back(h);
        }
        p.hobby = hobby;
        std::cout << "please input friend_name : ";
        std::cin >> p.friend_name;
        std::cout << "please input friend_age : ";
        std::cin >> p.friend_age;
        std::cout << "please input friend_sex : ";
        std::cin >> p.friend_sex;

        std::string res = getJsonString(p);
        std::cout << res;

        int len = write(clnt_sock, res.c_str(), res.length());
        std::cout << "write len : " << len << std::endl;
        str_len = read(clnt_sock, message, sizeof(message) - 1);
        if(str_len == -1)
            error_headling("read() error");
        std::cout << "message from server :  " << message << std::endl;

        std::cout << "input q can exit : ";
        std::cin >> message;
        if(strcmp(message, "q") == 0) break;
    }

    close(clnt_sock);

    return 0;
}

void error_headling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

std::string getJsonString(const people& p)
{
    Json::Value root;
    root["name"] = p.name;
    root["age"] = p.age;
    root["sex"] = p.sex;
    root["friend_name"] = p.friend_name;
    root["friend_age"] = p.friend_age;
    root["friend_sex"] = p.friend_sex;

    std::string h = "";
    for(const auto& v : p.hobby)
    {
        h = h + v + ",";
    }
    root["hobby"] = h;
    Json::FastWriter w;
    return w.write(root);
}