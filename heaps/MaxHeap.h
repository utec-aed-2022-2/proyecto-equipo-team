#ifndef CRYPTOCHAIN_MAXHEAP_H
#define CRYPTOCHAIN_MAXHEAP_H

#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

template <typename T>
class MaxHeap
{
private:
    static int left(int index) { return index * 2 + 1; };
    static int right(int index) { return index * 2 + 2; };
    static int parent(int index) { return (index - 1) / 2; };

    static void heapify_down(vector<T> &vec, int p)
    {
        if (left(p) > vec.size() - 1)
        {
            return;
        }
        if (right(p) > vec.size() - 1 && left(p) < vec.size())
        {
            if (vec[p] < vec[left(p)])
            {
                T temp = vec[p];
                T val = vec[left(p)];
                vec[p] = val;
                vec[left(p)] = temp;
            }
        }
        else
        {
            if (vec[p] < vec[left(p)] || vec[p] < vec[right(p)])
            {
                T temp = vec[p];
                T val = max(vec[left(p)], vec[right(p)]);
                vec[p] = val;

                if (vec[left(p)] == val)
                {
                    vec[left(p)] = temp;
                    heapify_down(vec, left(p));
                }
                else
                {
                    vec[right(p)] = temp;
                    heapify_down(vec, right(p));
                }
            }
        }
    }
    static void heapify_up(vector<T> &vec, int p)
    {

        int par = parent(p);
        if (par >= 0)
        {
            if (vec[p] > vec[par])
            {
                swap(vec[par], vec[p]);
                heapify_up(vec, par);
            }
        }
    }


public:
    vector<T> elements;
    MaxHeap() = default;

    void clear(){
        this->elements.clear();
    }

    void push(T data)
    {
        elements.push_back(data);
//        cout << elements.size() << "a\n";
        heapify_up(elements, elements.size() - 1);
    }
    void pop()
    {
        elements[0] = elements[elements.size() - 1];
        elements.pop_back();
        heapify_down(elements, 0);
    }

    void display()
    {
        for (auto i : elements)
            cout << i << " ";
        cout << endl;
    }
    T get_value()
    {
        return elements[0];
    }
};

#endif //CRYPTOCHAIN_MAXHEAP_H
