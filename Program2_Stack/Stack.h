#ifndef STACK_H
#define STACK_H

#include "Node.h" // Include the Node class header

class Stack
{
private:
    Node *head;

public:
    Stack();
    void pushFront(int number);
    void popFront();
    void printAllNodes();
};

#endif
