#ifndef H_NODE
#define H_NODE
#include "Tuple.h"
template <typename T>
struct Node {
    int indice;
    T data;
    Node<T>* next;
    Node<T>* prev;

    Node(){
        indice = 0;
        data = 0;
        next = nullptr;
        prev = nullptr;

    }

    explicit Node(T value){
        // TODO         NULL==nullotr
        indice = 0;
        data = value;
        next = nullptr;
        prev = nullptr;
    }

    void killSelf(){
        // TODO
    }
};

#endif