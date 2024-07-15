#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>

int main(int argc, const char* argv[])
{
    int status;
    pid_t pid;
    pid = fork();

    if(pid == 0)
    {
        return 1;
    }
    else
    {
        std::cout << "Child pid : " << pid << std::endl;
        pid = fork();
        if(pid == 0)
        {
            sleep(2);
            exit(2);
        }
        else
        {
            std::cout << "Child pid : " << pid << std::endl;
            waitpid(-1, &status, WNOHANG);
            if(WIFEXITED(status))
                std::cout << "Child send one : " << WEXITSTATUS(status) << std::endl;

            waitpid(-1, &status, WNOHANG);
            if(WIFEXITED(status))
                std::cout << "Child send two : " << WEXITSTATUS(status) << std::endl;

            sleep(2);
        }
    }
    return 0;
}