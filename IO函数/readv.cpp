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
    char buf1[20] = "22";
    char buf2[20] = {0};

    struct iovec vec[2];
    vec[0].iov_base = buf1+2;
    vec[0].iov_len = 3;
    vec[1].iov_base = buf2;
    vec[1].iov_len = 1024;

    readv(0, vec, 2);

    std::cout << buf1 << std::endl << buf2 << std::endl;
    return 0;
}