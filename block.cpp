#include "sha256.h"
#include "Transaction.cpp"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

class Block{
private:
    int index{};
    size_t nonce;
    string hash_code;
    string* prev_hash_code{};
    // string copy_prev_hash_code; // it is not safe te keep a copy of the code (it is better bc we do not need to generate the code again, but how?)
public:
    vector<Transaction> transactions;  //vector of transaction of fixed size

    Block() = default;

    Block(int index, vector<Transaction> transactions, string* prev_hash_code){ // [this need to change when Transaction is fully implemented, we are assuming string for simplicity]
        this->index = index;
        this->transactions = std::move(transactions); 
        this->prev_hash_code = prev_hash_code;
        this->hash_code = generate_hash_code();
    }

    string generate_hash_code(){
        // implement our own hash structure, to transform our data into a hashed string we do not know about and use it as input for the sha256 [TODO]
        // smth like this -> return sha256( our_hash(index) + our_hash(data) + our_hash(prev_hash_code) )
        string transact;
        for(auto & transaction : transactions){
            transact += transaction.get_data_as_string();
        }

        return sha256( to_string(index) + to_string(nonce) + transact + (*prev_hash_code) ); // missing the rest of members of data 
    }

    string get_hash_code(){
        return hash_code;
    }

    string get_prev_hash_code(){
        return (*prev_hash_code);
    }

    size_t get_nonce() {
        return nonce;
    }

    void set_nonce(size_t nonce){
        this->nonce = nonce;
    }

    bool is_valid(){
        return (hash_code == generate_hash_code());
    }

    bool proof_of_work() {
        // auto aux = this->generate_hash_code();
        // cout << aux << endl;
        return (this->generate_hash_code().substr(0, 3) == "000");
    }

    // bool is_full_transactions_capacity() {
    //     return (transactions.size() == MAX_NUMBER_OF_TRANSACTIONS);
    // }

    void short_display(){
        cout << "       Index:              " << index << endl;
        cout << "       Nonce:              " << nonce << endl;
        cout << "       Hash code:          " << hash_code << endl;
        cout << "       Prev. hash code:    " << *prev_hash_code << endl;
    }

};