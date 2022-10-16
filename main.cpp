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

int main(int argc, char *argv[]){ // test the blockchain, if it is reading correctly the data.
    // sha256_test();

    blockchain_test_1();

    return 0;
}