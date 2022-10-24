#ifndef H_NODE
#define H_NODE

template <typename T>
struct Node {
    T data;
    Node<T>* next;
    Node<T>* prev;

    Node(){
        data = 0;
        next = nullptr;
        prev = nullptr;

    }

    explicit Node(T value){
        // TODO         NULL==nullotr
        data = value;
        next = nullptr;
        prev = nullptr;
    }

    void killSelf(){
        // TODO
    }
};

#endif