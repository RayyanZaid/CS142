#include <iostream>
#include <atomic>
#include <pthread.h>
#include <chrono>

using namespace std;

class Node
{
public:
    Node(int num) : data(num), next(nullptr) {}

    int getData() const { return data; }
    Node *getNext() const { return next.load(); }

    void setNext(Node *newNode)
    {
        next.store(newNode);
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
        this->head.store(nullptr);
        this->numElements.store(0); // Initialize the number of elements to 0
    }

    Node *getHead()
    {
        return this->head.load();
    }

    void pushFront(int number)
    {
        Node *newHead = new Node(number);

        Node *prevHead = this->head.load();
        newHead->setNext(prevHead);

        // Use compare_exchange to update the head atomically
        while (!this->head.compare_exchange_weak(prevHead, newHead))
        {
            newHead->setNext(prevHead);
        }

        numElements++; // Increment the number of elements when pushing
    }

    void popFront()
    {
        Node *currHead = this->head.load();
        if (currHead != nullptr)
        {
            Node *next = currHead->getNext();
            // Use compare_exchange to update the head atomically
            while (!this->head.compare_exchange_weak(currHead, next))
            {
                next = currHead->getNext();
            }
            delete currHead;
            numElements--; // Decrement the number of elements when popping
        }
    }

    void printAllNodes()
    {
        Node *currNode = this->head.load();

        while (currNode != nullptr)
        {
            int currNumber = currNode->getData();
            cout << currNumber << endl;
            currNode = currNode->getNext();
        }

        cout << "Done" << endl;
    }

    int getNumElements()
    {
        return this->numElements.load();
    }

private:
    std::atomic<Node *> head;
    std::atomic<int> numElements;
};

int numElements = 100000;

void *pushEvenNumbers(void *arg)
{
    Stack &stack = *reinterpret_cast<Stack *>(arg);
    for (int i = 2; i < numElements; i += 2) // Push even numbers less than 20
    {
        stack.pushFront(i);
    }
    pthread_exit(NULL);
}

void *pushOddNumbers(void *arg)
{
    Stack &stack = *reinterpret_cast<Stack *>(arg);
    for (int i = 1; i < numElements; i += 2) // Push odd numbers less than 20
    {
        stack.pushFront(i);
    }
    pthread_exit(NULL);
}
void twoThreadsVersion3()
{
    Stack stack;

    pthread_t t1, t2;

    auto start = chrono::high_resolution_clock::now();

    pthread_create(&t1, NULL, pushEvenNumbers, &stack);
    pthread_create(&t2, NULL, pushOddNumbers, &stack);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    auto end = chrono::high_resolution_clock::now();
    chrono::nanoseconds elapsed = chrono::duration_cast<chrono::nanoseconds>(end - start);

    cout << "Elements in the stack:" << endl;
    cout << "Number of elements: " << stack.getNumElements() << endl;
    cout << "Length of the stack: " << stack.getNumElements() << endl;
    cout << "Time taken by twoThreadsVersion3: " << elapsed.count() << " nanoseconds" << endl;
}

void pushNumbers(Stack &stack)
{
    for (int i = 1; i < numElements; ++i)
    {
        stack.pushFront(i);
    }
}

void oneThreadVersion3()
{
    Stack stack;
    auto start = chrono::high_resolution_clock::now();
    pushNumbers(stack);
    auto end = chrono::high_resolution_clock::now();
    chrono::nanoseconds elapsed = chrono::duration_cast<chrono::nanoseconds>(end - start);

    cout << "Elements in the stack:" << endl;
    cout << "Number of elements: " << stack.getNumElements() << endl;
    cout << "Length of the stack: " << stack.getNumElements() << endl;
    cout << "Time taken by oneThreadVersion3: " << elapsed.count() << " nanoseconds" << endl;
}

int main()
{
    twoThreadsVersion3();
    oneThreadVersion3();

    return 0;
}
