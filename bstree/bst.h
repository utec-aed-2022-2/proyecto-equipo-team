#ifndef BSTREE_H
#define BSTREE_H

#include "iterator.h"
#include <string>
using namespace std;
#include "bst_node.h"

// #include <stack>
// #include <queue>
template <typename T>
class BSTree
{
public:
    typedef BSTIterator<T> iterator;

private:
    NodeBT<T> *root;
    string inOrder_string;

public:
    BSTree()
    {
        root = nullptr;
    }

    bool solve_query(T X, int query_type){
        return this->find(X);
    }

    void insert(T value)
    {
        insert(this->root, value);
    }
    bool find(T value)
    {
        auto trav = root;
        while (true)
        {
            if (trav == nullptr)
            {
                return false;
            }
            else if (value > trav->data)
            {
                trav = trav->right;
            }
            else if (value < trav->data)
            {
                trav = trav->left;
            }
            else if (value == trav->data)
            {
                return true;
            }
        }
    }

    string displayInOrder()
    {
        string vacio;
        displayInOrder(this->root, vacio);
        return vacio;
    }
    // string displayPreOrder()
    // {
    //     stack<NodeBT<T> *> Bolsita;
    //     Bolsita.push(root);
    //     string texto = "";
    //     while (!Bolsita.empty())
    //     {
    //         NodeBT<T> *tempo = Bolsita.top();
    //         texto += to_string(tempo->data);
    //         texto += " ";
    //         Bolsita.pop();
    //         if (tempo->right != nullptr)
    //             Bolsita.push(tempo->right);
    //         if (tempo->left != nullptr)
    //             Bolsita.push(tempo->left);
    //     }
    //     return texto;
    // }
    string displayPostOrder();
    // int height();
    int height()
    {
        return height(this->root);
    }

    T minValue()
    {
        return minValue(this->root);
    }
    T maxValue()
    {
        return maxValue(this->root);
    }
    void remove(T value)
    {
        auto trav = root;
        auto prev = trav;
        bool flag = true;
        while (flag)
        {
            if (value > trav->data)
            {
                prev = trav;
                trav = trav->right;
            }
            else if (value < trav->data)
            {
                prev = trav;
                trav = trav->left;
            }
            else if (value == trav->data)
            {
                // return true; found
                if (trav->right == nullptr && trav->left == nullptr)
                { // it is a leaf
                    // cout << "it is a leaf" << endl;
                    if(prev->left == trav){
                        prev->left = nullptr;
                    } else if(prev->right == trav){
                        prev->right = nullptr;
                    } 
                    // delete trav;
                    flag = false;
                    // break;
                }
                else if (trav->right != nullptr && trav->left == nullptr)
                {
                    // it has only 1 child
                    // cout <<"it has only one child " << endl;
                    // trav->data = trav->right->data;
                    T data = trav->right->data;
                    remove(trav->right->data);
                    // flag = false;
                    trav->data = data;

                }
                else if (trav->right == nullptr && trav->left != nullptr)
                {
                    // cout <<"it has only one child2 " << endl;
                    // trav->data = trav->left->data;
                    T data = trav->left->data;
                    remove(trav->left->data);
                    // flag = false;
                    trav->data = data;
                    // if(trav->left->data > trav->data){
                    //     trav->right = trav->left;
                    //     trav->left = nullptr;
                    // }
                }
                else if (trav->right != nullptr && trav->left != nullptr)
                {
                    // two child
                    int i = 1;
                    int aux = value - i;
                    while (!find(aux))
                    { // buscar menores
                        i = i + 1;
                        aux = value - i;
                    }
                    remove(aux);
                    trav->data = aux;
                }
                // cout << "so far" << endl;
                if(trav->right != nullptr){
                    if(trav->right->data < trav->data){
                    // cout << "so far1\n" << endl;
                    T aux = trav->right->data;
                    trav->right = nullptr;
                    insert(aux);
                    // cout << "so far2\n" << endl;
                    }
                }
                flag = false;
            }
        }
    }
    bool isBalanced()
    {
        return isBalanced(this->root);
    }

    int size()
    {
        return size(this->root);
    }
    // int size();
    bool isFull(){ // es aquel en el que todos los nodos tienen 0 o 2 hijos
    return (isFullt(this->root));
    }
    bool isFullt(NodeBT<T>* &node){
        if(node == nullptr){
            return true;
        }
        if(node->left == nullptr && node->right == nullptr){
            return true;
        } 
        if((node->left) && (node->right)){ // true if different from 0 or null
            return (isFullt(node->left) && isFullt(node->right));
        }
        return false;
    }
    T successor(T value)
    {
        NodeBT<T> *temp;
        // return successor(this->root, temp, value);
        return successor(this->root, value);
    }
    // Retornar el valor siguiente de "value" en el arbol
    T predecessor(T value);                      // Retornar el valor anterior de "value" en el arbol
        void clear(){
          clear(this->root);
        } // Liberar todos los nodos (usar root->KillSelf)
    void buildFromOrderedArray(int n){ // dado un array ordenado construir un arbol binario balanceado
        int array[] = {1,5,7,8,9,11,12,13,15,18,20,22,25,26};
        this->root = bould_ordered_arr(array, 0, n);
    }
    NodeBT<T>* bould_ordered_arr(T array[], int start, int end){
        if (start > end){
            return nullptr;
        }
        int mid = (start+end)/2;
        NodeBT<T>* aux = new NodeBT<T>(array[mid]);
        if(this->root == nullptr){
            this->root = aux;
        }
        aux->left = bould_ordered_arr(array, start, mid-1);
        aux->right = bould_ordered_arr(array, mid+1, end);
        return aux;
    }
    
    string displayBFS()
    {
        string cadena;
        displayBFS(this->root, cadena);
        return cadena;
    }
    // Recorrido en anchura (BreadthFirstSearch)
    string displayDFS(){                      // Recorrido en profundidad (DepthFirstSearch)
        string result;
        result = DFS(this->root);
        return result;
    }
    string DFS(NodeBT<T>* node){
        string result;
        
        if(node == nullptr){
            return result;
        }
        result = result + to_string(node->data);
        result = result + " ";
        DFS(node->left);
        DFS(node->right);

        return result;
    }
    
    iterator begin(BSTIterator<int>::Type _); // Retorna el inicio del iterador
    iterator end();                           // Retorna el final del iterador
    ~BSTree();                                // Destructor del arbol

private:

    NodeBT<T> *insert(NodeBT<T> *&node, T value)
    {
        if (node == nullptr)
        {
            node = new NodeBT<T>();
            node->data = value;
            node->left = node->right = nullptr;
            return node;
        }
        if (value < node->data)
            return insert(node->left, value);

        if (value > node->data)
            return insert(node->right, value);
        return 0;
    }

    int size(NodeBT<T> *node)
    {
        if (node == NULL)
            return 0;
        return size(node->left) + size(node->right) + 1;
    }

    int height(NodeBT<T> *node)
    {

        if (node == nullptr)
            return 0;
        int left = height(node->left);
        int right = height(node->right);
        if (left > right)
            return right + 1;
        else if (right > left)
            return right + 1;
        return right + 1;
    }
    bool isBalanced(NodeBT<T> *node)
    {
        //            int th=0;
        //            int rh=0;
        //            if(node== nullptr){
        //                return true;
        //            }
        //            th=height(node->left);
        //            rh=height(node->right);
        //            if(abs(th-rh)<=1 && isBalanced(node->left) && isBalanced(node->right)){
        //                return 1;
        //            }
        //            return 0;
        int th = 0;
        int rh = 0;
        if (node == nullptr)
        {
            return true;
        }
        th = height(node->left);
        rh = height(node->right);
        if (abs(th - rh) <= 1 && isBalanced(node->left) && isBalanced(node->right))
        {
            return 0;
        }
        return 0;
    }
    void displayInOrder(NodeBT<T> *node, string &value)
    {
        string result;

        if (node == nullptr)
            return;
        displayInOrder(node->left, value);
        value = value + to_string(node->data);
        value = value + " ";
        displayInOrder(node->right, value);
    }
    T minValue(NodeBT<T> *&node)
    {
        // el bst tiene los mayores en la derecha
        NodeBT<T> *temp = node;
        while (temp->left != NULL)
        {
            temp = temp->left;
        }
        return (temp->data);
    }
    T maxValue(NodeBT<T> *&node)
    {
        // el bst tiene los mayores en la derecha
        NodeBT<T> *temp = node;
        while (temp->right != NULL)
        {
            temp = temp->right;
        }
        return (temp->data);
    }


    T successor(NodeBT<T> *&node,  T value){
        NodeBT<T> *sucesor=NULL;
        NodeBT<T> *temp=node;
        bool inicio=true;
        while(inicio!=false){
            if(value> temp->data){
                temp=temp->right;
            }
            else if(value< temp->data){
                sucesor=temp;
                temp=temp->left;
            }
            else{
                if(temp->left){
                    while (sucesor->left!= nullptr) {
                        sucesor = sucesor->left;
                    }

                }
                    inicio=false;
            }
        }
        return sucesor->data;
    }

    T predecessor(NodeBT<T> *&node,  T value){
        NodeBT<T> *predecesor=NULL;
        NodeBT<T> *temp=node;
        bool inicio=true;
        while(inicio!=false){
            if(value< temp->data){
                temp=temp->left;
            }
            else if(value> temp->data){
                predecesor=temp;
                temp=temp->right;
            }
            else{
                if(temp->left){
                    while (predecesor->right!= nullptr) {
                        predecesor = predecesor->right;
                    }
                }
                inicio=false;
            }
        }
        return predecesor->data;
    }
    // string displayBFS(NodeBT<T> *&node, string &BFS_string) {
    //     queue<NodeBT<T>*> kiu;
    //     BFS_string = BFS_string + to_string(node->data);
    //     BFS_string = BFS_string + " ";

    //     if(node->left != nullptr)
    //         kiu.push(node->left); 
    //     if(node->right != nullptr)
    //         kiu.push(node->right);

    //     while(!kiu.empty()){
    //         NodeBT<T>* aux = kiu.front();
    //         kiu.pop();
    //         if(aux->left != nullptr)
    //             kiu.push(aux->left); 
    //         if(aux->right != nullptr)
    //             kiu.push(aux->right);
    //         BFS_string = BFS_string + to_string(aux->data);
    //         BFS_string = BFS_string + " ";
    //     }
    //     return BFS_string;
    // }
    void clear(NodeBT<T> *&node){
        root->KillSelf();
        root = nullptr;
        }
};

#endif