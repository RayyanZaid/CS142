#include "Node.h"

Node::Node(int number)
{
    this->number = number;
}

int Node::getNum()
{
    return this->number;
}

Node *Node::getNext()
{
    return this->nextPtr;
}

void Node::setNext(Node *nextNode)
{
    this->nextPtr = nextNode;
}
