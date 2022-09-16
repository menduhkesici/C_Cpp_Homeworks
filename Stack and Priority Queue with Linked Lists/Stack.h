#ifndef STACK_CLASS
#define STACK_CLASS

#include<iostream>
#include<stdlib.h>

#include "Node.h"

using namespace std;

template <class T>
class Stack
{
private:
    // a linked list to hold the stack items
    Node<T>* stackTop; // top element of the stack unless the stack is empty

public:
    // constructor
    Stack(void);

    // stack access methods
    void Push(const T& item);
    T Pop(void);
    T Peek(void);

    // stack test and clear methods
    int StackEmpty(void) const; // return 1 if the stack is empty
    void ClearStack(void); // dynamically allocated memory for the nodes must be returned
    void ShowStack (void) const; // prints the content of the stack from top to bottom on the standard output (with cout)

    Node<T> *GetNode(const T& item, Node<T> *nextPtr=NULL);

    // destructor
    ~Stack(void);
};

// Class initialization
template <class T>
Stack<T>::Stack(void)
{
    stackTop = NULL;
}

template <class T>
void Stack<T>::Push(const T& item)
{
    stackTop = GetNode(item, stackTop);
}

template <class T>
T Stack<T>::Pop(void)
{
    if (stackTop == NULL)
    {
        cerr << "Stack empty" << endl;
        exit(1);
    }

    Node<T> *temp = stackTop;
    T tempitem = temp->data;

    stackTop = stackTop->NextNode();
    delete temp; // delete the popped node

    return tempitem; // return the data of the popped node
}

template <class T>
T Stack<T>::Peek(void)
{
    if (stackTop == NULL)
    {
        cerr << "Stack empty" << endl;
        exit(1);
    }

    return stackTop->data;
}

template <class T>
int Stack<T>::StackEmpty(void) const
{
    return (stackTop == NULL);
}

template <class T>
void Stack<T>::ClearStack(void)
{
    while(stackTop != NULL)
    {
        Node<T> *temp = stackTop;
        stackTop = stackTop->NextNode();
        delete temp; // deletes the nodes recursively until none remains.
    }
}

template <class T>
void Stack<T>::ShowStack (void) const
{
    Node<T> *temp = stackTop;
    while(temp != NULL)
    {
        // Show stack contents iteratively until NextNode is NULL
        cout << temp->data << endl;
        temp = temp->NextNode();
    }
}

template <class T>
Node<T> *Stack<T>::GetNode(const T& item, Node<T> *nextPtr)
{
    Node<T> *newNode; // declare pointer
    newNode = new Node<T>(item, nextPtr);
    // allocate memory and pass item and nextptr to the constructor which creates the object

    // terminate program if allocation not successful
    if (newNode == NULL)
    {
        cerr << "Memory allocation failed" << endl;
        exit(1);
    }
    return newNode;
}

template <class T>
Stack<T>::~Stack(void)
{
    // Clear the stack contents after the destructor is called
    ClearStack();
}

#endif  // STACK_CLASS
