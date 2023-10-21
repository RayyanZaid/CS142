#include <iostream>
#include <atomic>
#include <pthread.h>
#include <chrono>
#include <fstream>

using namespace std;

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
        pthread_mutex_init(&mutex, nullptr);
    }

    Node *getHead()
    {
        return this->head;
    }

    void pushFront(int number)
    {
        Node *newHead = new Node(number);

        Node *prevHead = this->head;

        // Lock the mutex to ensure exclusive access to the stack
        pthread_mutex_lock(&mutex);

        this->head = newHead;
        newHead->setNext(prevHead);
        numElements++;

        // Unlock the mutex after modifying the stack
        pthread_mutex_unlock(&mutex);
    }

    int getNumElements()
    {
        return this->numElements;
    }

private:
    std::atomic<Node *> head;
    std::atomic<int> numElements;
    pthread_mutex_t mutex; // Mutex for synchronization
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
    }
    pthread_exit(NULL);
}

void twoThreadsVersion3(int n)
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
}

void pushNumbers(int n)
{
    Stack stack;
    for (int i = 1; i <= n; ++i)
    {
        stack.pushFront(i);
    }
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

        for (int n = 10000; n <= 100000; n += 10000)
        {
            auto start1 = chrono::high_resolution_clock::now();
            pushNumbers(n); // Push elements using one thread
            auto end1 = chrono::high_resolution_clock::now();
            chrono::nanoseconds elapsed1 = chrono::duration_cast<chrono::nanoseconds>(end1 - start1);

            auto start2 = chrono::high_resolution_clock::now();
            twoThreadsVersion3(n); // Push elements using two threads
            auto end2 = chrono::high_resolution_clock::now();
            chrono::nanoseconds elapsed2 = chrono::duration_cast<chrono::nanoseconds>(end2 - start2);

            dataFile << elapsed1.count() << ", " << elapsed2.count() << ", " << n << endl;
        }
    }

    // Close the text file
    dataFile.close();
}

int main()
{
    timeTest();
    return 0;
}
