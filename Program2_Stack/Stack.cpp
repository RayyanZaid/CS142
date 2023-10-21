#include "Stack.h" // Include the Stack class header
#include "Node.h"
#include <iostream>
using namespace std;

Stack::Stack()
{
    this->head = nullptr;
    this->numElements = 0;
}

Node *Stack::getHead()
{
    return this->head;
}

void Stack::pushFront(int number)
{
    Node *newHead = new Node(number);

    Node *prevHead = this->head;
    this->head = newHead;
    this->head->setNext(prevHead);
    numElements++;
}

void Stack::popFront()
{
    if (this->head != nullptr)
    {
        Node *currHead = this->head;
        this->head = this->head->getNext();
        delete currHead;
        numElements--;
    }
}

void Stack::printAllNodes()
{
    Node *currNode = this->head;

    while (currNode != nullptr)
    {
        int currNumber = currNode->getNum();
        cout << currNumber << endl;
        currNode = currNode->getNext();
    }

    cout << "Done" << endl;
}

int Stack::getNumElements()
{
    return this->numElements;
}