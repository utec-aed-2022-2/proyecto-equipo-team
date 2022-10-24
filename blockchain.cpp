#include <utility>
#include "block.cpp"
#include <iostream>
#include <fstream>
#include "forward.h"
using namespace std;

#define num_transaction_in_block 100

class Blockchain{
private:
    int size;
    static Block generate_genesis(){ // index = 0
//        vector<Transaction> transactions; // calling default constructor and initializing variables
        ForwardList<Transaction> transactions;
        Transaction transaction;
        transactions.push_back(transaction);
        auto* ptr = new string("0");
        Block genesis(1, transactions, ptr); // index = 0 ; data = tra ; prev_hash_code = "0" | sha256("0") <?>
        return genesis;
    }
public:
    ForwardList<Block> chain; // needs our own implementation of vector to allocate memory dynamically [TODO] (maybe a forward circular linked list)

    Blockchain(){
        if(chain.is_empty()){
            Block genesis = generate_genesis();
            chain.push_back(genesis);
            size = 1; // deberia ser 1, pero como empezamos a contar desde 1, tenemos que añadir 1 al size para coordinar con los indices
        } else {
            cerr << "Error creating blockchain" << endl;
        }
    }

    Block* get_latest_block(){ // important not to use copies
        if(size != 0){
            return chain.back();
        } else {
            cerr << "Chain is empty" << endl;
            return nullptr;
        }
    }

    void add_block(const ForwardList<Transaction>& transactions){ // if we use string or transaction here will depend on the implementation of transaction
        // Block new_block // here we create the new block
        int index = size+1;
//        string aux = get_latest_block()->get_hash_code();
//        auto* latest_block_hash_code = new string(aux);
        string* latest_block_hash_code = get_latest_block()->get_hash_code();

        // cout << index << " | " << latest_block_hash_code << endl;
        Block new_block(index, transactions, latest_block_hash_code);

        size_t possible_nonce = 0;
        new_block.set_nonce(possible_nonce);
        cout << "mining..." << endl;
        while (!new_block.proof_of_work())
        {
            possible_nonce += 1;
            new_block.set_nonce(possible_nonce);
        }
        // new_block.short_display();
        chain.push_back(new_block);
        cout << "mined!" << endl;
        display_block(new_block);
        size++;
    }

    int is_chain_valid(){
        // implement our own iterator for our own vector
        // The idea here is to iterate over the chain asking if the current block is valid (Block has a method called is_valid() that compare the hash_code whit a new generation of a hash code)
        //  so if there are any changes somewhere on the chain we just return false, and maybe the index block
        int index_error = 0;
        for(int i=1; i<=chain.size();i++){
            if(!(chain[i].is_valid())){
                cout << "THE CHAIN WAS ALTERED, FIXING BLOCKCHAIN... \n";
                index_error = i;
                break;
            }
        }
        cout << "index error: " << index_error << endl;
        return index_error;
    }

    void validate_chain(){
        int index_error = is_chain_valid(); // index of block where error happened, if no error, then 0 is returned
        if(index_error){
            while(chain.size() > index_error) {
                cout << "chain size" << chain.size() << endl;
                chain.pop_back();
                size--;
            }
        }
    }


    void read_and_load_csv(const string& filename, char delim = ','){
        // i need the delimeter and the newline character and the filename
        ForwardList<Transaction> transactions;
        ifstream file(filename);

        if(file.is_open()){

            int number_of_transaction_read = 0; // this is important to store transaction into blocks
            string line, field;

            getline(file, line); // getting the first line (it only has the column names)

            while(getline(file, line)){ // getting data line by line

                Transaction new_transaction;
                stringstream ss(line); // getting data line by line into stringstream

                getline(ss, field, delim);
                Time new_time(field);
                new_transaction.date = new_time;

                int aux = 1;
                while(getline(ss, field, delim)){ // getting the rest of data
                    switch(aux){
                        case 1:
                            new_transaction.open = stof(field);
                            break;
                        case 2:
                            new_transaction.high = stof(field);
                            break;
                        case 3:
                            new_transaction.lowest = stof(field);
                            break;
                        case 4:
                            new_transaction.close = stof(field);
                            break;
                        case 5:
                            new_transaction.volume = stof(field);
                            break;
                        case 6:
                            new_transaction.close_time = stod(field);
                            break;
                        case 7:
                            new_transaction.quote_asset_volume = stod(field);
                            break;
                        case 8:
                            new_transaction.number_of_trades = stoi(field);
                            break;
                        case 9:
                            new_transaction.taker_buy_base_asset_volume = stod(field);
                            break;
                        case 10:
                            new_transaction.taker_buy_quote_asset_volume = stod(field);
                            break;
                        default:
                            break;
                            // cout << "field 'ignore' ignored" << endl; // this should be the ignore field from the .csv
                    }
                    aux++;
                }
                transactions.push_back(new_transaction);
                // new_transaction->display(); // the reading works
                number_of_transaction_read++;
                // cout << number_of_transaction_read << endl;
                if(number_of_transaction_read >= num_transaction_in_block){
                    number_of_transaction_read = 0;
                    add_block(transactions); // after this line, the block should be succesfully inserted in the blockchain
                    // delete <?>
                    transactions.clear();
                }
            }
            add_block(transactions); // adding the last transaction that couldn't make it to fill a block
            transactions.clear();
            // cout << chain.size();
            file.close();
        } else {
            cerr << "error while opening file (read_csv_innto_transaction)" << endl;
        }
    }

    void display_genesis(){
        cout << " --------------------------------------------------------------------------------------------------" << endl;
        cout << "|                                                                                                  |" << endl;
        chain[0].short_display();
        cout << "|                                                                                                  |" << endl;
        cout << " --------------------------------------------------------------------------------------------------" << endl;
        cout << "                                                    |" << endl;
        cout << "                                                    V" << endl;
    }

    void display(){
        cout << "Size of chain: " << chain.size() << endl;

        for(int i=1; i<=chain.size();i++){
            cout << " --------------------------------------------------------------------------------------------------" << endl;
            cout << "|                                                                                                  |" << endl;
            chain[i].short_display();
            cout << "|                                                                                                  |" << endl;
            cout << " --------------------------------------------------------------------------------------------------" << endl;
            cout << "                                                    |" << endl;
            cout << "                                                    V" << endl;
        }
    }

    static void display_block(Block block){
        cout << " --------------------------------------------------------------------------------------------------" << endl;
        cout << "|                                                                                                  |" << endl;
        block.short_display();
        cout << "|                                                                                                  |" << endl;
        cout << " --------------------------------------------------------------------------------------------------" << endl;
        cout << "                                                    |" << endl;
        cout << "                                                    V" << endl;
    }

    void modify_index(int block_index, int new_index){
        Block to_modify = chain[block_index];
        to_modify.index = new_index;
        chain[block_index] = to_modify;
    }

    void modify_nonce(int block_index, int new_nonce){
        Block to_modify = chain[block_index];
        to_modify.nonce = new_nonce;
        chain[block_index] = to_modify;
    }

    void modify_hashcode(int block_index, const string &new_hashcode){
        Block to_modify = chain[block_index];
        to_modify.hash_code = new_hashcode;
        chain[block_index] = to_modify;
    }

    void modify_prev_hashcode(int block_index, const string &new_prev_hashcode){
        Block to_modify = chain[block_index];
        *(to_modify.prev_hash_code) = new_prev_hashcode;
        chain[block_index] = to_modify;
    }

    void modify_transaction(int block_index, int index_transaction){
        Block to_modify = chain[block_index];
        cout << "\n\n" << endl;
        Transaction new_transaction;
        new_transaction.create_transaction();
        to_modify.transactions[index_transaction] = new_transaction;
        chain[block_index] = to_modify;
    }

};