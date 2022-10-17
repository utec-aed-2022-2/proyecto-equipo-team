//
// Created by Neo Zapata on 12/10/22.
//
#include <iostream>
#include "sha256.h"
#include "blockchain.cpp"

using namespace std;

#define FILENAME "btc_data_bi_day.csv"
#define DELIM ','

void sha256_test(){
    string input = "something";
    string output = sha256(input);
    cout << "sha256(" << input <<") = " << output << endl;
}

void filling_blockchain_test(Blockchain &cryptochain, const string& filename){
    ifstream file;
    file.open(filename);
    if(file.is_open()){
        cryptochain.read_and_load_csv(filename, DELIM);
    } else {
        cerr << "Error opening file. (filling_blockchain_test)" << endl;
    }
}

void blockchain_test_1(){
    Blockchain cryptochain; 
    filling_blockchain_test(cryptochain, FILENAME);
    cryptochain.display();
}

void Clear()
{
    cout << "\x1B[2J\x1B[H";
}


void add_transaction(){
    Clear();
    string tex;
    cout << "\n\n Add_trasnsaction " << endl;
    cout << "--------------------"<<endl;
    cout << "Add: ";
    cin  >> tex;


    //Incluir el add
}

void modification(){
    Clear();
    int N;
    cout << "\n\n Modification " << endl;
    cout << "--------------------"<<endl;
    cout << "Numero de bloque: ";
    cin  >> N;


    //Incluir el modification
}

void Interactive_Menu(){
  int opcion;
    bool repetir = true;
    
    do {
        Clear();
        cout << "\n\n        Menu " << endl;
        cout << "--------------------"<<endl;
        cout << "1. Show blocks" << endl;
        cout << "2. Add transaction" << endl;
        cout << "3. Modify transaction" << endl;
        cout << "0. End" << endl;
        
        cout << "\nEnter an option: ";
        cin >> opcion;
        
        
        switch (opcion) {
            case 1:
                Clear();
                int V;
                blockchain_test_1();
                do{
                  cout<<endl<< "Ingrese 0 para salir: ";
                  cin>> V;
                  Clear();
                }while(V!=0);
                break;
                
            case 2:
                add_transaction();
                break;
                
            case 3:
                modification();
                break;
                
            case 0:
            	repetir = false;
            	break;
        }        
    } while (repetir);

}

int main(int argc, char *argv[]){ // test the blockchain, if it is reading correctly the data.
    // sha256_test();

    Interactive_Menu();

    return 0;
}
