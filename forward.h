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
    int nodes = 0;

    Node<T>* merge(Node<T>* n1, Node<T>* n2){
        Node<T>* temp = new Node<T>();
        Node<T>* merged = temp;
        Node<T>* ptr = temp;
        while(n1 != nullptr && n2 != nullptr){
            if (n1->data <= n2->data){
                temp->next = n1;
                n1 = n1->next;
            }else{
                temp->next = n2;
                n2 = n2->next;
            }
            temp = temp->next;
        }

        while(n1 != nullptr){
            temp->next = n1;
            n1= n1->next;
            temp = temp->next;
        }

        while(n2 != nullptr){
            temp->next = n2;
            n2 = n2->next;
            temp = temp->next;
        }
        merged = merged->next;
        delete ptr;
        return merged;
    }

    Node<T>* middle(Node<T>* head){
        Node<T>* slow = head;
        Node<T>* fast = head->next;

        while(fast != nullptr && fast->next != nullptr){
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }

    Node<T>* merge_sort(Node<T>* head){
        if (head == nullptr || head->next == nullptr){
            return head;
        }
        Node<T>* mid_node = this->middle(head);
        Node<T>* head2 = mid_node->next;
        mid_node->next = nullptr;

        Node<T>* ans = this->merge(this->merge_sort(head), this->merge_sort(head2));

        return ans;
    }

public:
    ForwardList() : List<T>() {}

    ~ForwardList(){
        // TODO
    }

    void sort(){
        if(this->nodes <= 1) return;
        this->head = this->merge_sort(this->head);
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
//
//    T pop_back(){
//        Node<T>* tempNode = this->head;
//        T result;
//        int i = 0;
//        while (true){
//            if(i + 2  == this->nodes){
//                result = tempNode->next->data;
//                tempNode->next = nullptr;
//                break;
//            } else {
//                tempNode = tempNode->next;
//            }
//            i++;
//        }
//        this->nodes = this->nodes - 1;
//        return result;
//    }

    T pop_back(){
        if (this->head == nullptr){
            throw std::runtime_error("Forward List has no elements");
        } else if (this->head->next == nullptr){
            T aux = this->head->data;
            delete this->head;
            this->head = nullptr;
            this->nodes = 0;
            return aux;
        }else{
            Node<T> * temp = head;
            while (temp->next->next != nullptr)
                temp = temp->next;
            T aux = temp->next->data;
            delete temp->next;
            temp->next = nullptr;
            this->nodes--;
            return aux;
        }
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
