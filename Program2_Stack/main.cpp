#include <iostream>
#include <pthread.h>
#include <chrono>
#include "Stack.h"

using namespace std;
using namespace std::chrono;

Stack s = Stack();
const int N = 100000000;

void oneThreadVersion1()
{
    for (int i = 1; i <= N; i++)
    {
        s.pushFront(i);
    }
}

void *threadFunction1(void *arg)
{
    int thread_id = *((int *)arg);
    for (int i = 1; i <= N / 2; i++)
    {
        s.pushFront(i);
    }
    return nullptr;
}

void *threadFunction2(void *arg)
{
    int thread_id = *((int *)arg);
    for (int i = (N / 2) + 1; i <= N; i++)
    {
        s.pushFront(i);
    }
    return nullptr;
}

void createAndRunThreads()
{
    pthread_t thread1, thread2;
    int thread_id1 = 1;
    int thread_id2 = 2;

    if (pthread_create(&thread1, nullptr, threadFunction1, &thread_id1) != 0)
    {
        std::cerr << "Error creating thread 1" << std::endl;
    }

    if (pthread_create(&thread2, nullptr, threadFunction2, &thread_id2) != 0)
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
    auto start = high_resolution_clock::now();

    oneThreadVersion1();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start); // or use microseconds

    cout << "Execution time with 1 thread: " << duration.count() << " nanoseconds" << endl; // or "microseconds"

    s = Stack();

    start = high_resolution_clock::now();

    createAndRunThreads();
    stop = high_resolution_clock::now();
    duration = duration_cast<nanoseconds>(stop - start); // or use microseconds

    cout << "Execution time with 2 thread: " << duration.count() << " nanoseconds" << endl; // or "microseconds"

    return 0;
}
