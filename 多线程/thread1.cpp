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

void* thread_main(void*);
int main(int argc, const char* argv[])
{
    fputs("start...\n", stdout);
    pthread_t t_id;
    int thread_param = 5;
    pthread_create(&t_id, nullptr, thread_main, (void*)&thread_param);

    sleep(10);
    fputs("end...\n", stdout);
    return 0;
}

void* thread_main(void* arg)
{
    int cnt = *((int *)arg);

    for(int i=0;i<cnt;i++)
    {
        puts("running thread");
        sleep(1);
    }
}