#ifndef STACK_H
#define STACK_H

#include "Node.h" // Include the Node class header

class Stack
{
private:
    Node *head;
    int numElements;

public:
    Stack();
    void pushFront(int number);
    void popFront();
    void printAllNodes();
    int getNumElements();
    bool compareAndSetHead(Node *old_head, Node *new_head);
    Node *getHead();
};

#endif
