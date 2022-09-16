#ifndef BTREENODE_HPP
#define BTREENODE_HPP

template <class T>
class BTreeNode
{
public:
    BTreeNode<T>* Children[3];
    T data[2];
    BTreeNode (void); // void constructor initializes all children pointers to null
    BTreeNode (T* items, BTreeNode<T>** C);
};

template <class T>
BTreeNode<T>::BTreeNode(void)
{
    for (int i = 0; i < 3; i++)
    {
        Children[i] = NULL;
    }
}

template <class T>
BTreeNode<T>::BTreeNode (T* items, BTreeNode<T>** C)
{
    for (int i = 0; i < 2; i++)
    {
        data[i] = items[i];
    }

    for (int i = 0; i < 3; i++)
    {
        Children[i] = C[i];
    }
}

#endif // BTREENODE_HPP
