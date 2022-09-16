#include <iostream>

#include "Node.h"
#include "Stack.h"
#include "Element.h"
#include "PQueue.h"

using namespace std;

int main()
{
    cout << "Example 1: " << endl << endl;

    {
        Stack<char> *Mystack;
        Mystack = new Stack<char>();
        cout << Mystack->StackEmpty() << endl;
        Mystack->Push('a');
        cout << Mystack->Peek() << endl;
        Mystack->Push('c');
        cout << Mystack->Peek() << endl;
        cout << Mystack->StackEmpty() << endl;
        Mystack->Push('b');
        cout << Mystack->Peek() << endl;
        Mystack->Push('d');
        cout << "Mystack: " << endl;
        Mystack->ShowStack();
        cout << Mystack->Pop() << endl;
        cout << Mystack->Pop() << endl;
        cout << Mystack->Peek() << endl;
        Mystack->ClearStack();
        cout << Mystack->StackEmpty() << endl;
    }

    cout << endl << "Example 2: " << endl << endl;

    {
        Element<char> *MyElement1, *MyElement2, *MyElement3, *MyElement4;
        MyElement1 = new Element<char>('e', 1);
        MyElement2 = new Element<char>('f', 0);
        MyElement3 = new Element<char>('g', 2);
        MyElement4 = new Element<char>('h', 1);
        PQueue< Element<char> > *MyPQ;
        MyPQ = new PQueue< Element<char> >();
        MyPQ->PQInsert(*MyElement1); // queue: e
        MyPQ->PQInsert(*MyElement2); // queue: fe
        MyPQ->PQInsert(*MyElement3); // queue: feg
        cout << "Current state of the queue:" << endl;
        MyPQ->ShowPQ();
        MyPQ->PQInsert(*MyElement4); // queue: fehg
        cout << "Starting to delete:" << endl;
        cout << (MyPQ->PQDelete()).ShowData() << endl;
        cout << (MyPQ->PQDelete()).ShowData() << endl;
        cout << (MyPQ->PQDelete()).ShowData() << endl;
        cout << (MyPQ->PQDelete()).ShowData() << endl;
    }

    cout << endl << "Example 3: " << endl << endl;

    {
        Element<char> *MyElement1, *MyElement2, *MyElement3, *MyElement4;
        MyElement1 = new Element<char>('e', 1);
        MyElement2 = new Element<char>('f', 0);
        MyElement3 = new Element<char>('g', 2);
        MyElement4 = new Element<char>('h', 1);
        PQueue< Element<char> > *MyPQ;
        MyPQ = new PQueue< Element<char> >();
        MyPQ->PQInsert(*MyElement1); // queue: e
        MyPQ->PQInsert(*MyElement2); // queue: fe
        MyPQ->PQInsert(*MyElement3); // queue: feg
        cout << "number of elements in the queue after three insertions is: " << MyPQ->PQLength() << endl; // length: 3
        cout << "what is at the front?: " << (MyPQ->PQFront()).ShowData() << endl; // front: f
        MyPQ->PQInsert(*MyElement4); // queue: fheg
        cout << "number of elements in the queue after three insertions is: " << MyPQ->PQLength() << endl; // length: 4
        cout << "what is at the front?: " << (MyPQ->PQFront()).ShowData() << endl; // front: f
        MyPQ->PQClear(); // queue:
        MyPQ->PQInsert(*MyElement3); // queue: g
        MyPQ->PQInsert(*MyElement1); // queue: eg
        cout << "number of elements in the queue after three insertions is: " << MyPQ->PQLength() << endl; // length: 2
        cout << "what is at the front?: " << (MyPQ->PQFront()).ShowData() << endl; // front: e
    }

    cout << endl << "Example 4: " << endl << endl;

    {
        Element<char> *MyElement1, *MyElement2, *MyElement3, *MyElement4;
        MyElement1 = new Element<char>('e', 1);
        MyElement2 = new Element<char>('f', 0);
        MyElement3 = new Element<char>('g', 2);
        MyElement4 = new Element<char>('h', 1);
        cout << MyElement1->ShowPriority() << endl;
        cout << MyElement2->ShowPriority() << endl;
        cout << MyElement3->ShowPriority() << endl;
        cout << MyElement4->ShowPriority() << endl;
        cout << (*MyElement1 < *MyElement2) << endl;
        cout << (*MyElement2 < *MyElement3) << endl;
        cout << (*MyElement1 < *MyElement4) << endl;
    }

    return 0;
}
