#include "sha256.h"
#include "Transaction.cpp"
#include <string>
#include <iostream>
#include "forward.h"
using namespace std;

class Block{
public:
    bool valid = true;
    int index{};
    size_t nonce{};
    string hash_code;
    string* prev_hash_code{};
    ForwardList<Transaction> transactions;

    void display_transactions(){
        cout << "Index | Year | Month | Day | Open | High | Lowest | Close | Volume | Close time | Number of trades" << endl;
        for(int i = 1 ; i <= transactions.size() ; i++){
            cout << i << " |     ";
            transactions[i].short_display();
        }
    }

    Block() = default;

    Block(int index, const ForwardList<Transaction>& transactions, string* prev_hash_code){
        this->index = index;
        this->transactions = transactions;
        this->prev_hash_code = prev_hash_code;
        this->hash_code = generate_hash_code();
        this->valid = true;
    }

    string generate_hash_code(){
        string transact;
        for(int i=1; i<=transactions.size();i++){
            transact += transactions[i].get_data_as_string();
        }
        hash_code = sha256( to_string(this->index) + to_string(this->nonce) + transact + *(this->prev_hash_code));
        return hash_code;
    }

    string* get_hash_code(){
        return &(hash_code);
    }

    void update(){
        string former_hash_code = this->hash_code;
        string new_hash_code = this->generate_hash_code();

        if(former_hash_code != new_hash_code)
            this->valid = false;
        this->hash_code = new_hash_code;
    }

    void update(const Block& new_block){
        string former_hash_code = this->hash_code;

        this->index = new_block.index;
        this->nonce = new_block.nonce;
        this->hash_code = new_block.hash_code;
        this->prev_hash_code = new_block.prev_hash_code;
        this->transactions = new_block.transactions;
        string new_hash_code = this->generate_hash_code();

        if(former_hash_code != new_hash_code)
            this->valid = false;
        this->hash_code = new_hash_code;
    }

    void set_nonce(size_t nonce_){
        this->nonce = nonce_;
    }

    bool proof_of_work(int dificulty) { // method: HashCash
        switch(dificulty){
            case 1:{
                return (this->generate_hash_code().substr(0, 1) == "0");
                break;
            }
            case 2:{
                return (this->generate_hash_code().substr(0, 2) == "00");
                break;
            }
            case 3:{
                return (this->generate_hash_code().substr(0, 3) == "000");
                break;
            }
            case 4:{
                return (this->generate_hash_code().substr(0, 4) == "0000");
                break;
            }
            case 5:{
                return (this->generate_hash_code().substr(0, 5) == "00000");
                break;
            }
            default:{
                cout << "dificulty mode error" << endl;
                return false;
            }
        }
    }

    void short_display(){
        cout << "       Index:              " << index << endl;
        cout << "       Transactions:       " << transactions.size() << endl;
        cout << "       Nonce:              " << nonce << endl;
        cout << "       Hash code:          " << hash_code << endl;
        cout << "       Prev. hash code:    " << *prev_hash_code << endl;
        cout << "       valid:              " << int(valid) << endl;
    }
};