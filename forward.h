#ifndef H_FORWARD
#define H_FORWARD

#include <iostream>
#include "list.h"
#include "node.h"
using namespace std;
// TODO: Implement all methods
// TODO: Implement all methods
template <typename T>
class ForwardList : public List<T> {
private:
    Node<T>* head;
    int nodes=0;

public:
    ForwardList() : List<T>() {}

    ~ForwardList(){
        // TODO
    }

    T front(){

        return this->head->data;
    }

    T* back(){
        Node<T>* temporal;
        temporal = this->head;
        while(temporal->next != nullptr){
            temporal = temporal->next;
        }
        return &(temporal->data);
    }

    virtual void push_front(T data){
        auto* newNode = new Node<T>(data);
        if(this->size()==0) {
            this->head = newNode;
        } else {
            newNode->next = this->head;
            this->head = newNode;
        }
        nodes=nodes+1;
    }

    void push_back(T data){
        auto* newNode = new Node<T>(data);
        if(this->size()==0) {
            this->head = newNode;
        } else {
            Node<T>* tempNode = this->head;
            int i = 0;
            while (true){
                if(i + 1  == this->nodes){
                    tempNode->next = newNode;
                    break;
                } else {
                    tempNode = tempNode->next;
                }
                i++;
            }
        }
        this->nodes = this->nodes + 1;
    }

    T pop_front(){
        Node<T>* deleteNode = this->head;
        T result = deleteNode->data;
        this->head = this->head->next;
        delete deleteNode;
        this->nodes = this->nodes - 1;
        return result;
    }

    T pop_back(){
        Node<T>* tempNode = this->head;
        T result;
        int i = 0;
        while (true){
            if(i + 2  == this->nodes){
                result = tempNode->next->data;
                tempNode->next = nullptr;
                break;
            } else {
                tempNode = tempNode->next;
            }
            i++;
        }
        this->nodes = this->nodes - 1;
        return result;
    }

    void insert(T data, int pos){
        auto* newNode = new Node<T>(data);
        Node<T>* tempNode1 = this->head;
        int i = 0;
        while (true){
            if(i + 2  == pos){
                Node<T>* tempNode2 = tempNode1->next;
                tempNode1->next = newNode;
                newNode->next = tempNode2;
                break;
            } else {
                tempNode1 = tempNode1->next;
            }
            i++;
        }
    }

//    void remove(int pos){
////        throw ("sin definir");
//    }

    T& operator[](int pos){
        Node<T>* tempNode = this->head;
        int i = 0;
        while (true){
            if(i + 1  == pos){
                return tempNode->data;
            } else {
                tempNode = tempNode->next;
            }
            i++;
        }
    }

    bool is_empty(){
        if(this->nodes == 0)return true;
        return false;
    }

    int size(){
        return this->nodes;
    }

    void clear(){
        Node<T>* tempNode = this->head;
        while(this->head != nullptr){
            if(this->head->next != nullptr){
                tempNode = this->head->next;
            }
            if(this->head->next != nullptr){
                this->head->killSelf();
                this->head = tempNode;
                if(this->head->next == nullptr){
                    this->head->killSelf();
                    this->head = nullptr;
                }
            }
            this->nodes--;
        }
        this->nodes--;
    }
};

#endif