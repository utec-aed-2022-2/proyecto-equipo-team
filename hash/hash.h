#ifndef CRYPTOCHAIN_HASH_H
#define CRYPTOCHAIN_HASH_H

#include <forward_list>
#include <iostream>
#include <vector>
#include <functional>

using namespace std;

const float maxFillFactor = 0.5;
const int maxCollision = 3;

//codigo extraido de cooding room de clases
template<typename TK, typename TV>
class Hash
{
private:
    struct Entry{
        TK key;
        TV value;
        size_t hashcode;
        Entry(){}
        Entry(TK _k, TV _v, size_t h){
            key = _k;
            value = _v;
            hashcode = h;
        }
    };
    vector<size_t> bucket;
    forward_list<Entry>* array;
    int capacity;
    int size;
    hash<TK> getHash;


    bool exits(TK key, TV value, int index){
        for (auto j: array[index]){
            if(j.key == key){
                return true;
            }
        }
        return false;
    }

public:
    Hash(int _capacity = 5){
        this->capacity = _capacity;
        this->array = new forward_list<Entry>[this->capacity];
        this->size = 0;
    }

    float fillFactor(){
        return size / (capacity * maxCollision);
    }

    void insert(TK key, TV value){
        //Si se termina el espacio hacemos rehashing
        if(fillFactor() >= maxFillFactor) rehashing();
        size_t hashcode = getHash(key);
        int index = hashcode % capacity;

        if (!exits(key, value, index)) {
            array[index].push_front(Entry(key, value, hashcode));
            bucket.push_back(hashcode);
            size++;
        }
    }

    TV get(TK key){
        //Sacamos el index
        size_t hashcode = getHash(key);
        int index = hashcode % capacity;
        //TVerificamos si esta y si se encuentra imprime el valor
        for (auto it: array[index]){
            if(it.key == key){
                return it.value;
            }
        }
        return 0;
    }

    void rehashing(){
        //aplicamos nuestra nueva capacidad del hash
        int prev_capacidad = capacity;
        capacity = capacity*2;
        //creamos un nuevo forward con la nueva capacidad
        forward_list<Entry>* t = new forward_list<Entry>[this->capacity];
        auto dx = 0;
        //Recorremos nuestro anterior listay la incluimod dentro de la nueva creada
        for(int i = 0; i < prev_capacidad; i++){
            for(auto j : array[i]){
                int index = j.hashcode % capacity;
                t[index].push_front(Entry(j.key, j.value, j.hashcode));
            }
        }
        //limpiamos memoria
        delete [] array;
        array = t;
    }

    void remove(TK key){
        //Sacamos el index
        size_t hashcode = getHash(key);
        int index = hashcode % capacity;
        //Recorremos la lista con el index equivalente
        //Verificamos si encontramos la key
        //si se encuentra la borramos
        for(int i=0;i< array[index].size();i++){

            if(array[i].key == key){
                erase(array[index].array[i]);
                size--;
            }
        }
    }

    int bucket_count(){
        return capacity;
    }

    int bucket_size(int i_bucket){
        return std::distance(array[i_bucket].begin(), array[i_bucket].end());
    }

    typename forward_list<Entry>::iterator begin(int i_bucket){
        return array[i_bucket].begin();
    }

    typename forward_list<Entry>::iterator end(int i_bucket){
        return array[i_bucket].end();
    }

    ~Hash(){
        //TODO: liberar cada lista
        delete[] this->array;
    }
};

#endif //CRYPTOCHAIN_HASH_H
