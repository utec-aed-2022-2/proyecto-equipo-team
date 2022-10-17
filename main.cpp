// COMPILE: g++ main.cpp sha256.cpp block.cpp blockchain.cpp Transaction.cpp -std=c++17
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

void blockchain_test_1(Blockchain &cryptochain){
//    cryptochain.display_genesis();
    filling_blockchain_test(cryptochain, FILENAME);
//    cryptochain.display();
}

void display_blockchain(Blockchain &cryptochain){
    cryptochain.display();
}

void Clear()
{
    cout << "\x1B[2J\x1B[H";
}


void add_transaction(Blockchain &cryptochain){
    Clear();
//    string tex;
    cout << "\n\n Add transaction " << endl;
    cout << "--------------------"<<endl;
//    cout << "Add: ";
//    cin  >> tex;


    //Incluir el add
//    Transaction n_transaction;
    ForwardList<Transaction> transactions;
    int opcion;
    do {
        Transaction n_transaction;
        n_transaction.create_transaction();
        transactions.push_back(n_transaction);
        cout << "Do you want to add another transaction?" << endl;
        cout << "0. No" << endl;
        cout << "1. Yes" << endl;

        cout << "\nEnter an option: ";
        cin >> opcion;
        while(opcion != 0 and opcion != 1){
            cerr << endl << "Enter a valid option";
            cout << "\nEnter an option: ";
            cin >> opcion;
        }
    } while(opcion);
    cryptochain.add_block(transactions);
}

void modification(Blockchain &cryptochain){
    Clear();
    int N;
    cout << "\n\n Modification " << endl;
    cout << "--------------------"<<endl;
    do {
        cout << "Numero de bloque: ";
        cin >> N;
    } while(N < 1 or N > cryptochain.chain.size());
    //Incluir el modification
    int field;
    cout << "\n\n Select field to modify in block " << N << endl;
    cout << "--------------------"<<endl;
    cout << "1. Index" << endl;
    cout << "2. Nonce" << endl;
    cout << "3. Hashcode" << endl;
    cout << "4. Prev Hashcode" << endl;
    cout << "5. Transactions" << endl;

    cout << "\nEnter an option: ";
    cin >> field;

    while(field != 1 and field != 2 and field != 3 and field != 4 and field != 5){
        cerr << endl << "Enter a valid option";
        cout << "\nEnter an option: ";
        cin >> field;
    }

    string new_hashcode;
    string new_prev_hashcode;

    switch (field) {
        case 1:
            int new_index;
            cout << "\nEnter new index: "; cin >> new_index;
            cryptochain.modify_index(N, new_index);
            break;

        case 2:
            size_t new_nonce;
            cout << "\nEnter new nonce: "; cin >> new_nonce;
            cryptochain.modify_nonce(N, new_nonce);
            break;

        case 3:
            cout << "\nEnter new hashcode: "; cin >> new_hashcode;
            cryptochain.modify_hashcode(N, new_hashcode);
            break;

        case 4:
            cout << "\nEnter new prev. hashcode: "; cin >> new_prev_hashcode;
            cryptochain.modify_prev_hashcode(N, new_prev_hashcode);
            break;

        case 5:
            int index_transaction;
            cout << "\n\n Displaying all transactions of block " << N << endl;
            cout << "--------------------"<<endl;
            cryptochain.chain[N].display_transactions();
            cout << "\nChoose a transaction to modify: "; cin >> index_transaction;

            while(index_transaction < 0 or index_transaction > cryptochain.chain[N].transactions.size()){
                cerr << endl << "Enter a valid option";
                cout << "\nChoose a transaction to modify: "; cin >> index_transaction;
            }

            cryptochain.modify_transaction(N, index_transaction);
            break;
        default:
            cout << "Invalid option." << endl;
            break;
    }
//    cryptochain.validate_chain();
}

void Interactive_Menu(){
    Blockchain cryptochain;
    int opcion;
    bool repetir = true;

    do {
        Clear();
        cout << "\n\n        Menu " << endl;
        cout << "--------------------"<<endl;
        cout << "1. Read and load data from .csv" << endl;
        cout << "2. Add transaction" << endl;
        cout << "3. Modify transaction" << endl;
        cout << "4. Show blocks" << endl;
        cout << "5. display trannsactions" << endl;
        cout << "0. End" << endl;

        cout << "\nEnter an option: ";
        cin >> opcion;

        while(opcion != 0 and opcion != 1 and opcion != 2 and opcion != 3 and opcion != 4 and opcion != 5){
            cerr << endl << "Enter a valid option";
            cout << "\nEnter an option: ";
            cin >> opcion;
        }

        switch (opcion) {
            case 1:
                Clear();
                int V;
                blockchain_test_1(cryptochain);
                do{
                    cout<<endl<< "Ingrese 0 para salir: ";
                    cin>> V;
                    Clear();
                }while(V!=0);
                break;

            case 2:
                add_transaction(cryptochain);
                break;

            case 3:
                modification(cryptochain);
                break;

            case 4:
                Clear();
                int C;
                display_blockchain(cryptochain);
                do{
                    cout<<endl<< "Ingrese 0 para salir: ";
                    cin>> C;
                    Clear();
                }while(C!=0);
                break;

            case 5:
                int block;

                do {
                    cout << "\n Select block: ";
                    cin >> block;
                } while(block < 1 or block > cryptochain.chain.size());

                cout << "\n\n Displaying all transactions of block " << block << endl;
                cout << "--------------------"<<endl;
                cryptochain.chain[block].display_transactions();

                int G;
                do{
                    cout<<endl<< "Ingrese 0 para salir: ";
                    cin>> G;
                    Clear();
                }while(G != 0);

                break;

            case 0:
                repetir = false;
                break;
            default:
                cout << "Invalid option." << endl;
                break;
        }
    } while (repetir);
    // final check
    cryptochain.validate_chain();
    display_blockchain(cryptochain);
}

int main(){ // test the blockchain, if it is reading correctly the data.
    // sha256_test();

    Interactive_Menu();

    return 0;
}