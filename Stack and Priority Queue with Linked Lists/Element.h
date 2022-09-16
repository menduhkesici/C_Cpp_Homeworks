#ifndef ELEMENT_CLASS
#define ELEMENT_CLASS

#include<iostream>
#include<stdlib.h>

using namespace std;

template <class T>
class Element
{
private:
    // a linked list object to hold the stack items
    T Data;
    int Priority; //non-negative value, similar to the processes in HW1 a smaller value shows a higher priority
public:
    // constructor
    Element(const T data, const int priority); //returns with error if priority is initialized a negative integer
    T ShowData(void);

    int ShowPriority (void) const;

    // overload the comparison operator < such that < returns true for
    // (element1 < element2) if (element2.Priority <= element1.Priority)
    bool operator< (const Element<T>& rhs) const;

};

template <class T>
Element<T>::Element(const T data, const int priority)
{
    Data = data;
    Priority = priority;
}

template <class T>
T Element<T>::ShowData(void)
{
    return Data;
}

template <class T>
int Element<T>::ShowPriority (void) const
{
    return Priority;
}

template <class T>
bool Element<T>::operator< (const Element<T>& rhs) const
{
    return (rhs.Priority <= Priority);
}

#endif // ELEMENT_CLASS
