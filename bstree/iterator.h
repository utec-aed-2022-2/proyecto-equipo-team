#ifndef ITERATOR_H
#define ITERATOR_H

#include "bst_node.h"

template <typename T>
class BSTIterator
{
public:
    enum Type {
        PreOrder, InOrder, PostOrder, BFS
    };

private:
    NodeBT<T> *current;
    Type type;

public:
    BSTIterator() : current(nullptr), type(InOrder) {};
    BSTIterator(NodeBT<T> *current, Type type=InOrder): type(type) {}
    BSTIterator<T> &operator=(BSTIterator<T> other);
    bool operator!=(BSTIterator<T> other);
    BSTIterator<T> &operator++();   //++it
    T operator*();
};

#endif