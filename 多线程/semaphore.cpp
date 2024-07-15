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
#include <semaphore.h>

static int num = 0;
static sem_t Read;
static sem_t Accu;
int sum;

void* read(void* arg)
{
    for(int i=0;i<5;i++)
    {
        sem_wait(&Read);
        fputs("please input num : \n", stdout);
        std::cin >> num;
        sem_post(&Accu);
    }
}

void* accu(void* arg)
{
    for(int i=0;i<5;i++)
    {
        sem_wait(&Accu);
        sum += num;
        sem_post(&Read);
    }
}

int main(int argc, const char* argv[])
{
    pthread_t thread_read, thread_accu;
    
    // 初始化信号量
    sem_init(&Read, 0, 1);
    sem_init(&Accu, 0, 0);

    pthread_create(&thread_read, nullptr, read, nullptr);
    pthread_create(&thread_accu, nullptr, accu, nullptr);

    pthread_join(thread_read, nullptr);
    pthread_join(thread_accu, nullptr);

    std::cout << "sum = " << sum << std::endl;

    sem_destroy(&Read);
    sem_destroy(&Accu);

    return 0;
}