#ifndef _STACK_HPP_
#define _STACK_HPP_

#include <iostream>
#include <stdlib.h>

using namespace std;

// Class declaration part

// template part, stack item class, and max. stack size, def. value is 10
template <class T, unsigned int MaxStackSize = 10>
class Stack
{
private:
    T stacklist[MaxStackSize];
    int top;
public:
    Stack(void); // constructor to initialize top modification operations
    void Push(const T& item); // push the item to the stack if the stack is not full
    T Pop(void); // pop the top element if the stack is not empty
    void ClearStack(void); //just copy top item without modifying stack contents
    T Peek(void) const; //check stack state, returns top element value without removal
    int StackEmpty(void) const; //returns true if the stack is empty
    int StackFull(void) const; //returns true if the stack is full
};

// Class implementation part

// Class initialization
template <class T, unsigned int MaxStackSize>
Stack<T, MaxStackSize>::Stack(void)
{
    top = -1;
}

// Push
template <class T, unsigned int MaxStackSize>
void Stack<T, MaxStackSize>::Push(const T& item)
{
    //can not push if stack has exceeded its limits
    if (top == MaxStackSize - 1)
    {
        cerr << "Stack overflow" << endl;
        exit(1);
    }

    // increment top pointer and copy item into list
    top++;
    stacklist[top] = item;
}

// Pop
template <class T, unsigned int MaxStackSize>
T Stack<T, MaxStackSize>::Pop(void)
{
    T temp;

    // if stack is empty, nothing to pop
    if (top == -1)
    {
        cerr << "Stack empty" << endl;
        exit(1);
    }

    //record the top element
    temp = stacklist[top];

    //decrement top and return the earlier top element
    top--;
    return temp;
}


//Peek is the same as Pop, except top is not moved
template <class T, unsigned int MaxStackSize>
T Stack<T, MaxStackSize>::Peek(void) const
{
    T temp;

    // if stack is empty, nothing to peek
    if (top == -1)
    {
        cerr << "Stack empty" << endl;
        exit(1);
    }

    // record the top element
    temp = stacklist[top];

    // return the earlier top element
    return temp;
}

//Clear Stack
template <class T, unsigned int MaxStackSize>
void Stack<T, MaxStackSize>::ClearStack(void)
{
    top = -1;
}

//Stack Empty
template <class T, unsigned int MaxStackSize>
int Stack<T, MaxStackSize>::StackEmpty(void) const
{
    return top == -1; // value is 1 if equal, 0 otherwise
}

// Stack Full
template <class T, unsigned int MaxStackSize>
int Stack<T, MaxStackSize>::StackFull(void) const
{
    return top == (MaxStackSize - 1);
}

#endif // _STACK_HPP_
