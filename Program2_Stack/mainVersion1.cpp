#include <iostream>
#include <fstream>
#include <pthread.h>
#include "Stack.h"

using namespace std;
using namespace std::chrono;

Stack s = Stack();
int N = 1000;

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
    int n = *((int *)arg); // Extract n from the argument
    for (int i = 1; i <= n; i += 2)
    {
        s.pushFront(i);
    }
    return nullptr;
}

void *threadFunction2(void *arg)
{
    int n = *((int *)arg); // Extract n from the argument
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

void accuracyTest()
{
    int numTimesTested = 100;

    int numCorrect1Thread = 0;
    int numCorrect2Threads = 0;

    for (int i = 0; i < 100; i++)
    {
        oneThreadVersion1(N);

        int length = s.getNumElements();

        if (N == length)
        {
            numCorrect1Thread++;
        }
        s = Stack();

        twoThreadsVersion1(N);
        length = s.getNumElements();

        if (N == length)
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

void timeTest()
{
    // Open a text file for writing the data
    ofstream dataFile("data.txt");

    if (!dataFile.is_open())
    {
        cout << "Error opening data file." << endl;
    }
    else
    {
        for (int n = 100; n < 10000; n += 50)
        {

            s = Stack();

            auto start = high_resolution_clock::now();

            oneThreadVersion1(n);

            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<nanoseconds>(stop - start);

            // Write the data to the text file
            dataFile << duration.count();

            s = Stack();

            start = high_resolution_clock::now();

            twoThreadsVersion1(n);

            stop = high_resolution_clock::now();
            duration = duration_cast<nanoseconds>(stop - start);

            // Append the data to the same text file
            dataFile << ", " << duration.count();

            dataFile << ", " << n << endl;
        }
    }

    // Close the text file
    dataFile.close();
}
int main()
{

    accuracyTest();
    timeTest();
    return 0;
}
