#include <iostream>
#include <fstream>
#include <pthread.h>
#include <chrono>
#include "Stack.h"

using namespace std;
using namespace std::chrono;

Stack s = Stack();
pthread_mutex_t myMutex; // myMutex for synchronization

void oneThreadVersion2(int n)
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

void twoThreadsVersion2(int n)
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
void accuracyTest(int numElementsPushed, int numTimesTested)
{

    int numCorrect1Thread = 0;
    int numCorrect2Threads = 0;

    for (int i = 0; i < numTimesTested; i++)
    {
        oneThreadVersion2(numElementsPushed);

        int length = s.getNumElements();

        if (numElementsPushed == length)
        {
            numCorrect1Thread++;
        }
        s = Stack();

        twoThreadsVersion2(numElementsPushed);
        length = s.getNumElements();

        if (numElementsPushed == length)
        {

            numCorrect2Threads++;
        }
        s = Stack();
    }

    double oneThreadAcc = ((1.0 * numCorrect1Thread) / (1.0 * numTimesTested)) * 100.0;
    cout << "Accuracy for 1 thread: " << oneThreadAcc << "%" << endl;

    double twoThreadsAcc = ((1.0 * numCorrect2Threads) / (1.0 * numTimesTested)) * 100.0;
    cout << "Accuracy for 2 threads: " << twoThreadsAcc << "%" << endl;
}

void timeTestForGraph()
{
    // Open a text file for writing the data
    ofstream dataFile("data.txt");

    if (!dataFile.is_open())
    {
        cout << "Error opening data file." << endl;
    }
    else
    {
        for (int n = 0; n <= 500000; n += 10000)
        {

            s = Stack();

            auto start = high_resolution_clock::now();

            oneThreadVersion2(n);

            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<nanoseconds>(stop - start);

            // Write the data to the text file
            dataFile << duration.count();

            s = Stack();

            twoThreadsVersion2(n);

            stop = high_resolution_clock::now();
            duration = duration_cast<nanoseconds>(stop - start);

            // s.printAllNodes();
            // Append the data to the same text file
            dataFile << ", " << duration.count();

            dataFile << ", " << n << endl;
        }
    }

    // Close the text file
    dataFile.close();
}

void timeTest(int numElementsPushed)
{

    s = Stack();

    auto start = high_resolution_clock::now();

    oneThreadVersion2(numElementsPushed);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);

    cout << "Duration for 1 threads pushing " << numElementsPushed << ": " << duration.count() << " nanoseconds" << endl;
    // s.printAllNodes();
    s = Stack();

    start = high_resolution_clock::now();

    twoThreadsVersion2(numElementsPushed);

    stop = high_resolution_clock::now();
    duration = duration_cast<nanoseconds>(stop - start);
    cout << "Duration for 2 threads pushing " << numElementsPushed << ": " << duration.count() << " nanoseconds" << endl;

    // s.printAllNodes();
}

int main()
{

    pthread_mutex_init(&myMutex, nullptr);

    int numElementsPushed = 10;
    int numTimesTestedForAccuracy = 100;
    s = Stack();
    accuracyTest(numElementsPushed, numTimesTestedForAccuracy);
    timeTest(numElementsPushed);
    timeTestForGraph();

    // timeTestForGraph();
    return 0;
}
