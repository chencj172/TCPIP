#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/uio.h>

int main(int argc, const char* argv[])
{
    char buf1[20] = "abcdef";
    char buf2[20] = "123456";
    struct iovec vec[2];
    vec[0].iov_base = buf1;
    vec[0].iov_len = 4;
    vec[1].iov_base = buf2;
    vec[1].iov_len = 3;

    int str_len;
    str_len = writev(1, vec, 2);
    std::cout << std::endl << str_len << std::endl;
    return 0;
}