#include <iostream>
#include <atomic>
#include <pthread.h>
#include <chrono>
#include <fstream>

using namespace std;
using namespace std::chrono;
class Node
{
public:
    Node(int num) : data(num), next(nullptr) {}

    int getData() const { return data; }
    Node *getNext() const { return next; }

    void setNext(Node *newNode)
    {
        next = newNode;
    }

private:
    int data;
    std::atomic<Node *> next;
};

class Stack
{
public:
    Stack()
    {
        this->head = nullptr;
        this->numElements = 0;
    }

    Node *getHead()
    {
        return this->head;
    }

    void pushFront(int number)
    {
        Node *newHead = new Node(number);

        Node *prevHead = this->head;

        this->head = newHead;
        newHead->setNext(prevHead);
        numElements++;
    }

    int getNumElements()
    {
        return this->numElements;
    }

private:
    std::atomic<Node *> head;
    std::atomic<int> numElements;
};

// Update the argument type passed to the thread functions
void *pushEvenNumbers(void *arg)
{
    void **args = static_cast<void **>(arg);
    Stack *stack = static_cast<Stack *>(args[0]);
    int n = *static_cast<int *>(args[1]);

    for (int i = 2; i <= n; i += 2)
    {
        stack->pushFront(i);
        // cout << "Even" << endl;
    }
    pthread_exit(NULL);
}

void *pushOddNumbers(void *arg)
{
    void **args = static_cast<void **>(arg);
    Stack *stack = static_cast<Stack *>(args[0]);
    int n = *static_cast<int *>(args[1]);

    for (int i = 1; i <= n; i += 2)
    {
        stack->pushFront(i);
        // cout << "Odd" << endl;
    }
    pthread_exit(NULL);
}

int twoThreadsVersion3(int n)
{
    Stack stack;

    pthread_t t1, t2;

    // Pass a pointer to the stack to both threads
    void *args[2];
    args[0] = &stack;
    args[1] = &n;

    pthread_create(&t1, NULL, pushEvenNumbers, args);
    pthread_create(&t2, NULL, pushOddNumbers, args);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return stack.getNumElements();
}

int oneThreadVersion3(int n)
{
    Stack stack;
    for (int i = 1; i <= n; ++i)
    {
        stack.pushFront(i);
    }

    return stack.getNumElements();
}
void accuracyTest(int numElementsPushed, int numTimesTested)
{

    int numCorrect1Thread = 0;
    int numCorrect2Threads = 0;

    for (int i = 0; i < numTimesTested; i++)
    {

        int length = oneThreadVersion3(numElementsPushed);

        if (numElementsPushed == length)
        {
            numCorrect1Thread++;
        }

        length = twoThreadsVersion3(numElementsPushed);

        if (numElementsPushed == length)
        {
            numCorrect2Threads++;
        }
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
        for (int n = 1000; n <= 1000; n += 50)
        {

            auto start = high_resolution_clock::now();

            oneThreadVersion3(n);

            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<nanoseconds>(stop - start);

            // Write the data to the text file
            dataFile << duration.count();

            twoThreadsVersion3(n);

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

void timeTest(int numElementsPushed)
{

    auto start1 = chrono::high_resolution_clock::now();
    oneThreadVersion3(numElementsPushed); // Push elements using one thread
    auto end1 = chrono::high_resolution_clock::now();
    chrono::nanoseconds elapsed1 = chrono::duration_cast<chrono::nanoseconds>(end1 - start1);

    cout << "1 thread:  " << elapsed1.count() << " nanoseconds" << endl;
    auto start2 = chrono::high_resolution_clock::now();
    twoThreadsVersion3(numElementsPushed); // Push elements using two threads
    auto end2 = chrono::high_resolution_clock::now();
    chrono::nanoseconds elapsed2 = chrono::duration_cast<chrono::nanoseconds>(end2 - start2);

    cout << "2 threads: " << elapsed2.count() << " nanoseconds" << endl;
}

int main()
{

    int numElementsPushed = 10000;
    int numTimesTestedForAccuracy = 1;
    accuracyTest(numElementsPushed, numTimesTestedForAccuracy);
    timeTest(numElementsPushed);

    // timeTestForGraph();
    return 0;
}
