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

    int tmp_buf = 1024 * 3, get_sock_buf1;
    socklen_t len1;
    setsockopt(sock_tcp, SOL_SOCKET, SO_SNDBUF, &tmp_buf, 4);
    getsockopt(sock_tcp, SOL_SOCKET, SO_SNDBUF, (void*)&get_sock_buf1, &len1);
    std::cout << "get TCP sendbuf : " << get_sock_buf1 << std::endl;

    int get_sock_buf2;
    socklen_t len2;
    setsockopt(sock_udp, SOL_SOCKET, SO_SNDBUF, &tmp_buf, 4);
    getsockopt(sock_udp, SOL_SOCKET, SO_SNDBUF, (void*)&get_sock_buf2, &len2);
    std::cout << "get UDP sendbuf : " << get_sock_buf2 << std::endl;

    close(sock_tcp);
    close(sock_udp);
    return 0;
}
