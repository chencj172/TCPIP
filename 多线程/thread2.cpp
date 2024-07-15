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
    void *thr_ret;
    pthread_create(&t_id, nullptr, thread_main, (void*)&thread_param);
    pthread_join(t_id, &thr_ret);

    std::cout << (char*)thr_ret << std::endl;
    free(thr_ret);
    fputs("end...\n", stdout);
    return 0;
}

void* thread_main(void* arg)
{
    int cnt = *((int *)arg);
    char *msg = (char*)malloc(sizeof(char) * 50);
    strcpy(msg, "hihihihi");

    for(int i=0;i<cnt;i++)
    {
        fputs("running thread\n", stdout);
        sleep(1);
        fflush(stdout);
    }

    return (void*)msg;
}