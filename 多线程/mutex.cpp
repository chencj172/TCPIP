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

long long num;
pthread_mutex_t mutex;
void* pthread_des(void* arg)
{
    pthread_mutex_lock(&mutex);
    for(int i=0;i<100000;i++)
        num += 1;
    pthread_mutex_unlock(&mutex);
    return nullptr;
}

void* pthread_inc(void* arg)
{
    for(int i=0;i<100000;i++)
    {
        pthread_mutex_lock(&mutex);
        num -= 1;
        pthread_mutex_unlock(&mutex);
    }

    return nullptr;
}

int main(int argc, const char* argv[])
{
    pthread_t t_id[10000];
    pthread_mutex_init(&mutex, nullptr);

    for(int i=0;i<10000;i++)
    {
        if(i & 1)
            pthread_create(&t_id[i], nullptr, pthread_des, nullptr);
        else
            pthread_create(&t_id[i], nullptr, pthread_inc, nullptr);
    }

    for(int i=0;i<10000;i++)
        pthread_join(t_id[i], nullptr);

    std::cout << "num = " << num << std::endl;
    pthread_mutex_destroy(&mutex);
    return 0; 
}