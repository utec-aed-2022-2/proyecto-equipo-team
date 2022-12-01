#ifndef BST_NODE_H
#define BST_NODE_H

template <typename T>
struct NodeBT {
    int indice;
    T data;
    NodeBT* left; 
    NodeBT* right;  
    
    NodeBT() : left(nullptr), right(nullptr) {}   
    NodeBT(T value) : indice(0), data(value), left(nullptr), right(nullptr) {}
    void KillSelf() {
        if (left != nullptr) left->KillSelf();
        if (right != nullptr) right->KillSelf();
        delete this;
    }
};

#endif
