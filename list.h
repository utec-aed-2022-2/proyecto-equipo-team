#ifndef H_LIST
#define H_LIST

#include "node.h"

// TODO: Implement all methods
template <typename T>
class List {
private:
    Node<T>* head;
    Node<T>* tail;
    int nodes=0;
public:
    List() : head(nullptr), tail(nullptr), nodes(0) {
    };
    virtual ~List(){
        // TODO
    };

    virtual T front() = 0;
    virtual T* back() = 0;
    virtual void push_front(T)= 0;
    virtual void push_back(T) = 0;
    virtual T pop_front() = 0;
    virtual T pop_back() = 0;
    virtual void insert(T, int) = 0;
//    virtual void remove(int) = 0;
    virtual T& operator[](int) = 0;//debe ser declarado en cada clase hija
    virtual bool is_empty() = 0;
    virtual int size() = 0;
    virtual void clear()= 0;
};

#endif