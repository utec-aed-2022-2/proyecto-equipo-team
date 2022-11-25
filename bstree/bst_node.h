#ifndef BST_NODE_H
#define BST_NODE_H

template <typename T>
struct NodeBT {
    
    T data;
    NodeBT* left; 
    NodeBT* right;  
    
    NodeBT() : left(nullptr), right(nullptr) {}   
    NodeBT(T value) : data(value), left(nullptr), right(nullptr) {}   
    void KillSelf() {
        if (left != nullptr) left->KillSelf();
        if (right != nullptr) right->KillSelf();
        delete this;
    }
};

#endif
