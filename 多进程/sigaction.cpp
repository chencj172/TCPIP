#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>

void time_headler(int sig)
{
    if(sig == SIGALRM)
    std::cout << "time out...\n";

    alarm(2);
}
int main(int argc, const char* argv[])
{
    struct sigaction mySigaction;
    mySigaction.sa_handler = time_headler;
    sigemptyset(&mySigaction.sa_mask);
    mySigaction.sa_flags = 0;

    sigaction(SIGALRM, &mySigaction, 0);

    alarm(2);

    for(int i=0;i<3;i++)
    {
        std::cout << "wait..\n";
        sleep(100);
    }
    return 0;
}
