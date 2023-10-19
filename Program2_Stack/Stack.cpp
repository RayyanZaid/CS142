#include "Stack.h" // Include the Stack class header
#include <iostream>
using namespace std;

Stack::Stack()
{
    this->head = nullptr;
}

void Stack::pushFront(int number)
{
    Node *newHead = new Node(number);

    Node *prevHead = this->head;
    this->head = newHead;
    this->head->setNext(prevHead);
}

void Stack::popFront()
{
    if (this->head != nullptr)
    {
        Node *currHead = this->head;
        this->head = this->head->getNext();
        delete currHead;
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
