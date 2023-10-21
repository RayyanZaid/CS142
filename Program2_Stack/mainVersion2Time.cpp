#include <iostream>
#include <fstream>
#include <pthread.h>
#include <chrono>
#include "Stack.h"

using namespace std;
using namespace std::chrono;

Stack s = Stack();
pthread_mutex_t myMutex; // myMutex for synchronization

void oneThreadVersion1(int n)
{
    for (int i = 1; i <= n; i++)
    {
        pthread_mutex_lock(&myMutex); // Lock before pushing
        s.pushFront(i);
        pthread_mutex_unlock(&myMutex); // Unlock after pushing
    }
}

void *threadFunction1(void *arg)
{
    int thread_id = *((int *)arg);
    int n = *((int *)arg); // Extract n from the argument
    for (int i = 1; i <= n; i += 2)
    {
        pthread_mutex_lock(&myMutex); // Lock before pushing
        s.pushFront(i);
        pthread_mutex_unlock(&myMutex); // Unlock after pushing
    }
    return nullptr;
}

void *threadFunction2(void *arg)
{
    int n = *((int *)arg); // Extract n from the argument
    for (int i = 2; i <= n; i += 2)
    {
        pthread_mutex_lock(&myMutex); // Lock before pushing
        s.pushFront(i);
        pthread_mutex_unlock(&myMutex); // Unlock after pushing
    }
    return nullptr;
}

void twoThreadsVersion1(int n)
{
    pthread_t thread1, thread2;
    int thread_id1 = 1;
    int thread_id2 = 2;

    if (pthread_create(&thread1, nullptr, threadFunction1, &n) != 0)
    {
        std::cerr << "Error creating thread 1" << std::endl;
    }

    if (pthread_create(&thread2, nullptr, threadFunction2, &n) != 0)
    {
        std::cerr << "Error creating thread 2" << std::endl;
    }

    if (pthread_join(thread1, nullptr) != 0)
    {
        std::cerr << "Error joining thread 1" << std::endl;
    }

    if (pthread_join(thread2, nullptr) != 0)
    {
        std::cerr << "Error joining thread 2" << std::endl;
    }
}

int main()
{
    // Initialize the myMutex
    if (pthread_mutex_init(&myMutex, nullptr) != 0)
    {
        std::cerr << "myMutex initialization failed" << std::endl;
        return 1;
    }

    // Rest of your code remains the same...

    // Close the myMutex
    pthread_mutex_destroy(&myMutex);

    return 0;
}
