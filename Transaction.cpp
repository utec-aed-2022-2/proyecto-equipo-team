#include <string>
#include <sstream>
#include <iostream>
#include <ctime>
using namespace std;

struct Transaction{
    int index = 0;
    string str1;
    string str2;
    float number;
    long tiempo;

    Transaction(){ // only for genesis
        index = 0;
        str1 = " - ";
        str2 = " - ";
        number = 0;
        tiempo = 0;
    }

    int get_index(){
        return this->index;
    }

    string get_data_as_string() const{
        return (to_string(number) + to_string(tiempo) + str1 + str2);
    }

    void create_transaction(int& INDICE){
//        index = this->index + 1;
        index = INDICE;
        INDICE++;
        cout << "str1: "; cin >> str1;
        cout << "str2: "; cin >> str2;
        cout << "number: "; cin >> number;
        cout << "tiempo: "; cin >> tiempo;
        cout << endl;
    }

    void short_display(){
        cout << str1 << " | " << str2 << " | " << number << " | " << tiempo << endl;
    }

    void display(){
        cout << "str1: " << str1 << endl;
        cout << "str2: " << str2 << endl;
        cout << "number: " << number << endl;
        cout << "tiempo: " << tiempo << endl;
        cout << endl;
    }

};

