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
    pid_t pid;
    pid = fork();

    if(pid == 0)
    {
        std::cout << "I am a child process\n";
    }
    else
    {
        std::cout << "Child process  ID : " << pid << std::endl;
        sleep(30);
    }

    if(pid == 0)
        std::cout << "End child process\n";
    else 
        std::cout << "End parent process\n";
    return 0;
}