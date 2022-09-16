#ifndef PQUEUE_CLASS
#define PQUEUE_CLASS

#include<iostream>
#include<stdlib.h>

#include "Node.h"
#include "Element.h"

using namespace std;

template <class T>
class PQueue
{
private:
    // a linked list to hold the queue items
    Node<T>* queueFront; // pointer to the head of the list
    int count; // number of elements in the the queue

public:
    // constructor
    PQueue(void);

    // queue access methods
    void PQInsert(const T& item); // inserts the item in the correct position according to its priority.
    T PQDelete(void); // Deletes the highest priority element at the queue front

    // queue access returns the highest priority element that is in the queue in O(1) time
    T PQFront(void);

    // queue test methods
    int PQLength(void) const;
    int PQEmpty(void) const; // return 1 if the queue is empty
    void PQClear(void); // dynamically allocated memory for the nodes must be returned
    void ShowPQ (void) const; // prints the content of the priority queue from front to rear on the standard output (with cout)

    Node<T> *GetNode(const T& item, Node<T> *nextPtr=NULL);

    // destructor
    ~PQueue(void);
};

template <class T>
PQueue<T>::PQueue(void)
{
    queueFront = NULL;
    count = 0;
}

template <class T>
void PQueue<T>::PQInsert(const T& item)
{
    // currPtr moves through list, trailed by prevPtr
    Node<T> *currPtr, *prevPtr, *newNode;
    prevPtr = NULL;
    currPtr = queueFront;

    // cycle through the list and find insertion point
    while (currPtr != NULL)
    {
        // found insertion point if (item < current data) is false
        if (!(item < currPtr->data))
            break;

        // advance currPtr so prevPtr trails it
        prevPtr = currPtr;
        currPtr = currPtr->NextNode();
    }

    // make the insertion
    if (prevPtr == NULL)
        // if prevPtr == NULL, insert at front
        queueFront = GetNode(item, queueFront);
    else
    {
        // insert new node after previous
        newNode = GetNode(item);
        prevPtr->InsertAfter(newNode);
    }

    count++;
}

template <class T>
T PQueue<T>::PQDelete(void)
{
    if (queueFront == NULL)
    {
        cerr << "Queue empty" << endl;
        exit(1);
    }

    Node<T> *temp = queueFront;
    T tempitem = temp->data;

    queueFront = queueFront->NextNode();
    delete temp; // delete the popped node

    count--;

    return tempitem; // return the data of the popped node
}

template <class T>
T PQueue<T>::PQFront(void)
{
    if (queueFront == NULL)
    {
        cerr << "Queue empty" << endl;
        exit(1);
    }

    return queueFront->data;
}

template <class T>
int PQueue<T>::PQLength(void) const
{
    return count;
}

template <class T>
int PQueue<T>::PQEmpty(void) const
{
    return (queueFront == NULL);
}

template <class T>
void PQueue<T>::PQClear(void)
{
    while(queueFront != NULL)
    {
        Node<T> *temp = queueFront;
        queueFront = queueFront->NextNode();
        delete temp; // deletes the nodes recursively until none remains.
    }

    count = 0;
}

template <class T>
void PQueue<T>::ShowPQ (void) const
{
    Node<T> *temp = queueFront;
    while(temp != NULL)
    {
        // Show queue contents iteratively until NextNode is NULL
        cout << temp->data.ShowData() << endl;
        temp = temp->NextNode();
    }
}

template <class T>
Node<T> *PQueue<T>::GetNode(const T& item, Node<T> *nextPtr)
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
PQueue<T>::~PQueue(void)
{
    // Clear the queue contents after the destructor is called
    PQClear();
}

#endif  // PQUEUE_CLASS
