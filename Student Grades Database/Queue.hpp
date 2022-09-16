#ifndef _QUEUE_HPP_
#define _QUEUE_HPP_

#include <iostream>
#include <stdlib.h>

using namespace std;

const int MaxQueueSize = 1000;

// Circular queue is implemented.

// Class declaration part

template <class T>
class Queue
{
private:
    // queue array and its parameters
    int qfront, rear, qcount;
    T qlist[MaxQueueSize] ;
public:
    Queue(void); // constructor to initialize data members
    // queue modification operations
    void Insert(const T& item); // insert the item to the queue if the queue is not full
    T Delete(void);
    void ClearQueue(void);
    T QFront(void) const;
    // queue test methods
    int QLength(void) const;
    int QEmpty(void) const;
    int QFull(void) const;
};

// Class implementation part

// Queue constructor
// initialize queue front, rear, count
template <class T>
Queue<T>::Queue(void)
{
    qfront = 0;
    rear = 0;
    qcount = 0;
}

// Queue Operations
// Insert: insert item into the queue
template <class T>
void Queue<T>::Insert(const T& item)
{
    // terminate if queue is full
    if (qcount == MaxQueueSize)
    {
        cerr << "Queue overflow!" << endl;
        exit(1);
    }

    //increment count, assign item to qlist and update rear
    qcount++;
    qlist[rear] = item;
    rear = (rear + 1) % MaxQueueSize;
}

// Delete: delete element from the front of the queue and return its value
template <class T>
T Queue<T>::Delete(void)
{
    T temp;

    // if the queue is empty, terminate the program
    if (qcount == 0)
    {
        cerr << "Deleting from an empty queue!" << endl;
        exit(1);
    }

    //record value at the front of the queue
    temp = qlist[qfront] ;

    //decrement count, advance front and return former front
    qcount--;
    qfront = (qfront + 1) % MaxQueueSize;
    return temp;
}

// Clear Queue
template <class T>
void Queue<T>::ClearQueue(void)
{
    qfront = 0;
    rear = 0;
    qcount = 0;
}

template <class T>
T Queue<T>::QFront(void) const
{
    T temp;

    // if the queue is empty, terminate the program
    if (qcount == 0)
    {
        cerr << "Deleting from an empty queue!" << endl;
        exit(1);
    }

    //return value at the front of the queue
    return qlist[qfront];
}

template <class T>
int Queue<T>::QLength(void) const
{
    return qcount;
}

template <class T>
int Queue<T>::QEmpty(void) const
{
    return (qcount == 0);
}

template <class T>
int Queue<T>::QFull(void) const
{
    return (qcount == MaxQueueSize);
}

#endif // _QUEUE_HPP_
