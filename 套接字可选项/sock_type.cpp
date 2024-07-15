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
    int sock_tcp, sock_udp;
    sock_tcp = socket(PF_INET, SOCK_STREAM, 0);
    sock_udp = socket(PF_INET, SOCK_DGRAM, 0);

    std::cout << "TCP type : " << SOCK_STREAM << std::endl;
    std::cout << "UDP type : " << SOCK_DGRAM << std::endl;

    int get_sock_type1, get_sock_type2;
    socklen_t len1 , len2;
    getsockopt(sock_tcp, SOL_SOCKET, SO_TYPE, (void*)&get_sock_type1, &len1);
    getsockopt(sock_udp, SOL_SOCKET, SO_TYPE, (void*)&get_sock_type2, &len2);

    std::cout << "get TCP type : " << get_sock_type1 << ", len : " << len1 << std::endl;
    std::cout << "get UDP type : " << get_sock_type2 << ", len : " << len2 << std::endl;

    int  get_sock_buf1, get_sock_buf2;
    getsockopt(sock_tcp, SOL_SOCKET, SO_SNDBUF, (void*)&get_sock_buf1, &len1);
    getsockopt(sock_udp, SOL_SOCKET, SO_SNDBUF, (void*)&get_sock_buf2, &len1);
    std::cout << "get TCP sendbuf : " << get_sock_buf1 << std::endl;
    std::cout << "get UDP sendbuf : " << get_sock_buf2 << std::endl;

    getsockopt(sock_tcp, SOL_SOCKET, SO_RCVBUF, (void*)&get_sock_buf1, &len1);
    getsockopt(sock_udp, SOL_SOCKET, SO_RCVBUF, (void*)&get_sock_buf2, &len1);
    std::cout << "get TCP recvbuf : " << get_sock_buf1 << std::endl;
    std::cout << "get UDP recvbuf : " << get_sock_buf2 << std::endl;


    close(sock_tcp);
    close(sock_udp);
    return 0;
}
