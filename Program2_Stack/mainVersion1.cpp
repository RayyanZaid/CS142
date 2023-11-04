#include <iostream>
#include <fstream>
#include <pthread.h>
#include "Stack.h"

using namespace std;
using namespace std::chrono;

Stack s = Stack();

void oneThreadVersion1(int n)
{
    for (int i = 1; i <= n; i++)
    {
        s.pushFront(i);
    }
}

void *threadFunction1(void *arg)
{
    int thread_id = *((int *)arg);
    int n = *((int *)arg);
    for (int i = 1; i <= n; i += 2)
    {
        s.pushFront(i);
    }
    return nullptr;
}

void *threadFunction2(void *arg)
{
    int n = *((int *)arg);
    for (int i = 2; i <= n; i += 2)
    {
        s.pushFront(i);
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

void accuracyTest(int numElementsPushed, int numTimesTested)
{

    int numCorrect1Thread = 0;
    int numCorrect2Threads = 0;

    for (int i = 0; i < numTimesTested; i++)
    {
        oneThreadVersion1(numElementsPushed);

        int length = s.getNumElements();

        if (numElementsPushed == length)
        {
            numCorrect1Thread++;
        }
        s = Stack();

        twoThreadsVersion1(numElementsPushed);
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

    s = Stack();
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

            std::chrono::high_resolution_clock::time_point start = high_resolution_clock::now();

            oneThreadVersion1(n);

            std::chrono::high_resolution_clock::time_point stop = high_resolution_clock::now();
            std::chrono::nanoseconds duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

            dataFile << duration.count();

            s = Stack();

            start = high_resolution_clock::now();

            twoThreadsVersion1(n);

            stop = high_resolution_clock::now();
            duration = duration_cast<nanoseconds>(stop - start);

            dataFile << ", " << duration.count();

            dataFile << ", " << n << endl;
        }
    }

    dataFile.close();

    s = Stack();
}

void timeTest(int numElementsPushed)
{

    s = Stack();

    std::chrono::high_resolution_clock::time_point start = high_resolution_clock::now();

    oneThreadVersion1(numElementsPushed);

    std::chrono::high_resolution_clock::time_point stop = high_resolution_clock::now();
    std::chrono::nanoseconds duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

    cout << "Duration for 1 threads pushing " << numElementsPushed << ": " << duration.count() << " nanoseconds" << endl;
    // s.printAllNodes();
    s = Stack();

    start = high_resolution_clock::now();

    twoThreadsVersion1(numElementsPushed);

    stop = high_resolution_clock::now();
    duration = duration_cast<nanoseconds>(stop - start);
    cout << "Duration for 2 threads pushing " << numElementsPushed << ": " << duration.count() << " nanoseconds" << endl;
    // s.printAllNodes();

    s = Stack();
}

int main()
{

    s = Stack();

    int numElementsPushed = 10500000;
    // int numElementsPushed = 100;
    int numTimesTestedForAccuracy = 10;
    // accuracyTest(numElementsPushed, numTimesTestedForAccuracy);
    // timeTest(numElementsPushed);

    timeTestForGraph();
    return 0;
}
