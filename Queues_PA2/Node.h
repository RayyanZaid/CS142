#ifndef NODE_H
#define NODE_H

class Node
{
private:
    int number;
    Node *nextPtr;

public:
    Node(int number);
    int getNum();
    Node *getNext();
    void setNext(Node *nextNode);
};

#endif
