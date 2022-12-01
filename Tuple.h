//
// Created by Neo Zapata on 30/11/22.
//

#ifndef CRYPTOCHAIN_TUPLE_H
#define CRYPTOCHAIN_TUPLE_H

template <typename T>
class Tuple{
public:
    int indice;
    T value;

    Tuple() = default;

    Tuple(int indice, T value){
        this->indice = indice;
        this->value = value;
    }
};

#endif //CRYPTOCHAIN_TUPLE_H
