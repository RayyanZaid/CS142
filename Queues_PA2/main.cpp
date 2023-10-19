#include <iostream>
#include "Stack.h" // Include the Stack class header

using namespace std;

int main()
{
    Stack s = Stack();
    s.pushFront(10);
    s.printAllNodes();
}