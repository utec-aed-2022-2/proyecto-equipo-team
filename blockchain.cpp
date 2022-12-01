#include <utility>
#include "block.cpp"
#include <iostream>
#include <fstream>
#include "forward.h"
#include "bstree/bst.h"
#include <chrono>
#include <ctime>
#include "Tuple.h"
#include "heaps/MaxHeap.h"
#include "heaps/MinHeap.h"
#include "hash/hash.h"
#include "trie/simpletrie.h"
using namespace std::chrono;
using namespace std;

#define num_transaction_in_block 100

class Blockchain{
private:
    int size;
    static Block generate_genesis(){ // index = 0
        ForwardList<Transaction> transactions;
        Transaction transaction;
        transactions.push_back(transaction);
        auto* ptr = new string("0");
        Block genesis(1, transactions, ptr); // index = 0 ; data = tra ; prev_hash_code = "0" | sha256("0") <?>
        return genesis;
    }
public:

// bstrees
    BSTree<float> *bstree_number = new BSTree<float> ();
    BSTree<time_t> *bstree_tiempo = new BSTree<time_t> ();

    // heaps
    MaxHeap<float> *maxheap_number = new MaxHeap<float>();
    MinHeap<float> *minheap_number = new MinHeap<float>();
    MaxHeap<float> *maxheap_date = new MaxHeap<float>();
    MinHeap<float> *minheap_date = new MinHeap<float>();

    // hash
    Hash<int, float> *hash_number = new Hash<int, float>();
    Hash<int, float> *hash_tiempo = new Hash<int, float>();

    // simpletrie

    TrieSimple* strie_str1 = new TrieSimple();
    TrieSimple* strie_str2 = new TrieSimple();

    void display_bstree(int opcion){
        switch(opcion){
            case 1:
                cout << bstree_number->displayInOrder() << endl;
                break;
            case 2:
                cout << bstree_tiempo->displayInOrder() << endl;
                break;
            default:
                cerr << "Option is invalid" << endl;
                return;
        }
    }

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

    Block* get_latest_block() { // important not to use copies
        if(size != 0){
            return chain.back();
        } else {
            cerr << "Chain is empty" << endl;
            return nullptr;
        }
    }

    void add_to_bstrees(const Block& new_block){
        ForwardList<Transaction> trans = new_block.transactions;
        for(int i = 1 ; i <= trans.size() ; i++){
            int index = trans[i].index;
            this->bstree_number->insert(index, trans[i].number);
        }
    }

    void add_to_hash(const Block& new_block){
        ForwardList<Transaction> trans = new_block.transactions;
        for (int i = 1; i <= trans.size(); i++)
        {
            this->hash_number->insert(trans[i].number, trans[i].index);
            this->hash_tiempo->insert(trans[i].tiempo, trans[i].index);
        }
    }

    void add_to_trie(const Block& new_block){
        ForwardList<Transaction> trans = new_block.transactions;
        for (int i = 1; i <= trans.size(); i++)
        {
            // send tuple to keep index
            this->strie_str1->insert(trans[i].index, trans[i].str1);
            this->strie_str2->insert(trans[i].index, trans[i].str2);
        }
    }

    void add_to_maxheaps(const Block &new_block)
    {
        ForwardList<Transaction> trans = new_block.transactions;
        for (int i = 1; i <= trans.size(); i++)
        {
            this->maxheap_number->push(trans[i].number);
            this->maxheap_date->push(trans[i].tiempo);
        }
    }

    void add_to_minheaps(const Block &new_block)
    {
        ForwardList<Transaction> trans = new_block.transactions;
        for (int i = 1; i <= trans.size(); i++)
        {
            this->minheap_number->push(trans[i].number);
            this->minheap_date->push(trans[i].tiempo);
        }
    }

    void show_transactions_by_index(ForwardList<int> indices){
        int counter = 1;

        indices.sort();
        cout << "   str1 | str2 | number | tiempo" << endl;
        for(int i = 1 ; i <= this->chain.size() ; i++){
            for(int j = 1 ; j <= this->chain[i].transactions.size() ; j++){
//                cout << this->chain[i].transactions[j].index << " | " << indices[counter] << endl;
                if(this->chain[i].transactions[j].index == indices[counter]){
                    cout << counter << ") ";
                    this->chain[i].transactions[j].short_display();
                    counter++;
                    if(counter > indices.size())
                        return;
                }
            }
        }
    }

    void add_block(const ForwardList<Transaction>& transactions, int dificulty){ // if we use string or transaction here will depend on the implementation of transaction
        int inn = size + 1;
//        int index = this->prev_index;
//        this->prev_index++;
        string* latest_block_hash_code = get_latest_block()->get_hash_code();
        Block new_block(inn, transactions, latest_block_hash_code);

        size_t possible_nonce = 0;
        new_block.set_nonce(possible_nonce);
        cout << "mining..." << endl;
        while (!new_block.proof_of_work(dificulty))
        {
            possible_nonce += 1;
            new_block.set_nonce(possible_nonce);
        }
        chain.push_back(new_block);
        cout << "mined!" << endl;
        add_to_bstrees(new_block);
        add_to_maxheaps(new_block);
        add_to_minheaps(new_block);
        add_to_trie(new_block);
        add_to_hash(new_block);
        display_block(new_block);
        size++;
    }

    void fix(){
        for(int i = 1 ; i <= this->chain.size() ; i++){
            this->chain[i].update_();
        }
    }

    void read_and_load_csv(int& INDICE, const string& filename, char delim, int dificulty){
        ForwardList<Transaction> transactions;
        ifstream file(filename);

        if(file.is_open()){
            int number_of_transaction_read = 0; // this is important to store transaction into blocks
            string line, field;

            getline(file, line); // getting the first line (it only has the column names)

            while(getline(file, line)){ // getting data line by line

                Transaction new_transaction;
                new_transaction.index = INDICE;
                INDICE++;

                stringstream ss(line); // getting data line by line into stringstream

                int aux = 1;
                while(getline(ss, field, delim)){ // getting the rest of data
                    switch(aux){
                        case 1:
                            new_transaction.str1 = field;
                            break;
                        case 2:
                            new_transaction.str2 = field;
                            break;
                        case 3:
                            new_transaction.number = stof(field);
                            break;
                        case 4:
                            new_transaction.tiempo = stof(field);
                            break;
                        default:
                            break;
                    }
                    aux++;
                }
                transactions.push_back(new_transaction);
                number_of_transaction_read++;
                if(number_of_transaction_read >= num_transaction_in_block){
                    number_of_transaction_read = 0;
                    add_block(transactions, dificulty); // after this line, the block should be succesfully inserted in the blockchain
                    transactions.clear();
                }
            }
            add_block(transactions, dificulty); // adding the last transaction that couldn't make it to fill a block
            transactions.clear();
            file.close();
        } else {
            cerr << "error while opening file (read_csv_innto_transaction)" << endl;
        }
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

    void Clear()
    {
        cout << "\x1B[2J\x1B[H";
    }

    void igual_a_x(){

        int opcion;
        bool repetir = true;
        do{
            Clear();
            cout << "\n\n        Choose an attribute: " << endl;
            cout << "--------------------"<<endl;
            cout << "1. number" << endl;
            cout << "2. tiempo" << endl;
            cout << "0. End" << endl;

            cout << "\nEnter an option: ";
            cin >> opcion;

            while(opcion != 0 and opcion != 1 and opcion != 2){
                cerr << endl << "Enter a valid option";
                cout << "\nEnter an option: ";
                cin >> opcion;
            }
            if (opcion == 0){
                repetir = false;
            } else {
                float x;
                Clear();
                cout << "\n\n        Choose a range: " << endl;
                cout << "--------------------"<<endl;
                cout << "\nEnter X: ";
                cin >> x;
                Clear();
                switch (opcion) {
                    case 1: {
                        auto start1 = chrono::high_resolution_clock::now();
                        auto index = this->hash_number->get(x);
                        auto end1 = chrono::high_resolution_clock::now();
                        if(index != 0){
                            display_by_index(index);
                        } else {
                            cout << "NOT FOUND!" << endl;
                            return;
                        }
                        calculate_time(start1, end1);
                        ingrese_0_para_salir();
                        break;
                    }
                    case 2: {
                        auto start2 = chrono::high_resolution_clock::now();
                        auto index = this->hash_tiempo->get(x);
                        auto end2 = chrono::high_resolution_clock::now();
                        if(index != 0){
                            display_by_index(index);
                        } else {
                            cout << "NOT FOUND!" << endl;
                            return;
                        }
                        calculate_time(start2, end2);
                        ingrese_0_para_salir();
                        break;
                    }
                    default: {
                        cout << "opcion invalido." << endl;
                        ingrese_0_para_salir();
                    }
                }
            }
        } while(repetir);
    }

    void display_by_index(int index){
        cout << "   str1 | str2 | number | tiempo" << endl;
        for(int i = 1 ; i <= this->chain.size() ; i++){
            for(int j = 1 ; j <= this->chain[i].transactions.size() ; j++){
                if(this->chain[i].transactions[j].index == index) {
                    this->chain[i].transactions[j].short_display();
                    return;
                }
            }
        }
    }

    void entre_x_y(){
        int opcion;
        bool repetir = true;
        do{
            Clear();
            cout << "\n\n        Choose an attribute: " << endl;
            cout << "--------------------"<<endl;
            cout << "1. number" << endl;
            cout << "2. tiempo" << endl;
            cout << "0. End" << endl;

            cout << "\nEnter an option: ";
            cin >> opcion;  

            while(opcion != 0 and opcion != 1 and opcion != 2){
                cerr << endl << "Enter a valid option";
                cout << "\nEnter an option: ";
                cin >> opcion;
            }
            if (opcion == 0){
                repetir = false;
            } else {
                float x, y;
                Clear();
                cout << "\n\n        Choose a range: " << endl;
                cout << "--------------------"<<endl;
                cout << "\nEnter X: ";
                cin >> x;
                Clear();
                cout << "\n\n        Choose a range: " << endl;
                cout << "--------------------"<<endl;
                cout << "\nEnter Y: ";
                cin >> y;
                Clear();
                switch(opcion){
                    case 1:{
                        ForwardList<int> indices;
                        auto start1 = chrono::high_resolution_clock::now();
                        this->bstree_number->range_search(indices, x, y);
                        auto end1 = chrono::high_resolution_clock::now();
                        show_transactions_by_index(indices);
                        calculate_time(start1, end1);
                        ingrese_0_para_salir();
                        break;
                    }
                    case 2:{
                        ForwardList<int> indices;
                        auto start2 = chrono::high_resolution_clock::now();
                        this->bstree_tiempo->range_search(indices, x, y);
                        auto end2 = chrono::high_resolution_clock::now();
                        show_transactions_by_index(indices);
                        calculate_time(start2, end2);
                        ingrese_0_para_salir();
                        break;
                    }
                    default:
                        cout << "error in opcion " << endl;
                        ingrese_0_para_salir();
                        break;
                }
            }
        } while(repetir);
    }

    void inicia_con(){
        int opcion;
        bool repetir = true;
        do{
            Clear();
            cout << "\n\n        Choose an attribute: " << endl;
            cout << "--------------------"<<endl;
            cout << "1. str1" << endl;
            cout << "2. str2" << endl;
            cout << "0. End" << endl;

            cout << "\nEnter an option: ";
            cin >> opcion;

            while(opcion != 0 and opcion != 1 and opcion != 2){
                cerr << endl << "Enter a valid option";
                cout << "\nEnter an option: ";
                cin >> opcion;
            }
            if (opcion == 0){
                repetir = false;
            } else {
                string x;
                Clear();
                cout << "\n\n        Enter a char or string : " << endl;
                cout << "--------------------"<<endl;
                cout << "\n starts with: ";
                cin >> x;
                Clear();
                switch(opcion){
                    case 1:{
                        ForwardList<int> indices;
                        auto start1 = chrono::high_resolution_clock::now();
                        indices = this->strie_str1->starts_with(x);
                        auto end1 = chrono::high_resolution_clock::now();
                        show_transactions_by_index(indices);
                        calculate_time(start1, end1);
                        ingrese_0_para_salir();
                        break;
                    }
                    case 2:{
                        ForwardList<int> indices;
                        auto start2 = chrono::high_resolution_clock::now();
                        indices = this->strie_str2->starts_with(x);
                        auto end2 = chrono::high_resolution_clock::now();
                        show_transactions_by_index(indices);
                        calculate_time(start2, end2);
                        ingrese_0_para_salir();
                        break;
                    }
                    default:
                        cout << "error in opcion " << endl;
                        ingrese_0_para_salir();
                        break;
                }
            }
        } while(repetir);
    }

    void contenido_en(){
        int opcion;
        bool repetir = true;
        do{
            Clear();
            cout << "\n\n        Choose an attribute: " << endl;
            cout << "--------------------"<<endl;
            cout << "1. str1" << endl;
            cout << "2. str2" << endl;
            cout << "0. End" << endl;

            cout << "\nEnter an option: ";
            cin >> opcion;

            while(opcion != 0 and opcion != 1 and opcion != 2){
                cerr << endl << "Enter a valid option";
                cout << "\nEnter an option: ";
                cin >> opcion;
            }
            if (opcion == 0){
                repetir = false;
            } else {
                string x;
                Clear();
                cout << "\n\n        Enter a char or string : " << endl;
                cout << "--------------------"<<endl;
                cout << "\n starts with: ";
                cin >> x;
                Clear();
                switch(opcion){
                    case 1:{
                        ForwardList<int> indices;
                        auto start1 = chrono::high_resolution_clock::now();
                        indices = this->strie_str1->contained_in(x);
                        auto end1 = chrono::high_resolution_clock::now();
                        show_transactions_by_index(indices);
                        calculate_time(start1, end1);
                        ingrese_0_para_salir();
                        break;
                    }
                    case 2:{
                        ForwardList<int> indices;
                        auto start2 = chrono::high_resolution_clock::now();
                        indices = this->strie_str2->contained_in(x);
                        auto end2 = chrono::high_resolution_clock::now();
                        show_transactions_by_index(indices);
                        calculate_time(start2, end2);
                        ingrese_0_para_salir();
                        break;
                    }
                    default:
                        cout << "error in opcion " << endl;
                        ingrese_0_para_salir();
                        break;
                }
            }
        } while(repetir);
    }

    void modify_index(int block_index, int new_index){
        Block to_modify = this->chain[block_index];
        to_modify.index = new_index;
        this->chain[block_index].update(to_modify);

    }

    void modify_nonce(int block_index, size_t new_nonce){
        Block to_modify = this->chain[block_index];
        to_modify.nonce = new_nonce;
        this->chain[block_index].update(to_modify);
    }

    void modify_hashcode(int block_index, const string &new_hashcode){
        Block to_modify = this->chain[block_index];
        to_modify.hash_code = new_hashcode;
        this->chain[block_index].update(to_modify);
    }

    void modify_prev_hashcode(int block_index, const string &new_prev_hashcode){
        Block to_modify = this->chain[block_index];
        *(to_modify.prev_hash_code) = new_prev_hashcode;
        this->chain[block_index].update(to_modify);
    }

    void modify_transaction(int block_index, int index_transaction){
        Block to_modify = this->chain[block_index];
        cout << "\n\n" << endl;
        Transaction new_transaction;
        new_transaction.create_transaction(to_modify.transactions[index_transaction].index);
        to_modify.transactions[index_transaction] = new_transaction;
        this->chain[block_index].update(to_modify);
    }

    void purge(){
        ForwardList<ForwardList<Transaction>> trans;
        for(int i = 1 ; i <= this->chain.size() ; i++){
            if(!this->chain[i].valid){
                while(this->chain.size() >= i){
                    ForwardList<Transaction> aux = this->chain.pop_back().transactions;
//                    cout << aux.size() << endl;
                    remove_from_bstrees(aux);
                    trans.push_back(aux);
//                    cout << this->chain.size() << endl;
                }
//                remove_from_heaps(trans);
                create_new_heaps();
                return;
            }
        }
    }

    void create_new_heaps(){
        maxheap_number->clear();
        minheap_number->clear();
        maxheap_date->clear();
        minheap_date->clear();
        for(int i = 1 ; i <= this->chain.size() ; i++){
            for(int j = 1 ; j <= this->chain[i].transactions.size() ; j++){
                maxheap_number->push(this->chain[i].transactions[j].number);
                minheap_number->push(this->chain[i].transactions[j].number);
                maxheap_date->push(this->chain[i].transactions[j].tiempo);
                minheap_date->push(this->chain[i].transactions[j].tiempo);
            }
        }
        cout << maxheap_number->elements.size();

    }


    void remove_from_bstrees(ForwardList<Transaction>& trans){
//        cout << trans.size() << endl;
        for(int i = 0 ; i < trans.size() ; i++){
//            bstree_number->remove(trans[i+1].number);
            bstree_number->deleteNode(bstree_number->root, trans[i+1].number);
        }
    }

    void calculate_time(std::chrono::time_point<std::chrono::high_resolution_clock> start2, std::chrono::time_point<std::chrono::high_resolution_clock> end2){
        auto duration = duration_cast<milliseconds>(end2 - start2);
        cout << endl << "Time taken by program is : " << duration.count() << " milliseconds.";
        cout << " sec" << endl;
    }

    void ingrese_0_para_salir(){
        int F6;
        do{
            cout<<endl<< "Ingrese 0 para salir: ";
            cin>> F6;
            Clear();
        }while(F6 != 0);
    }

    void max_valor()
    {
        int opcion;
        bool repetir = true;
        do
        {
            Clear();
            cout << "\n\n        Choose an attribute: " << endl;
            cout << "--------------------" << endl;
            cout << "1. number" << endl;
            cout << "2. tiempo" << endl;
            cout << "0. End" << endl;

            cout << "\nEnter an option: ";
            cin >> opcion;

            while (opcion != 0 and opcion != 1 and opcion != 2)
            {
                cerr << endl
                     << "Enter a valid option";
                cout << "\nEnter an option: ";
                cin >> opcion;
            }

            if (opcion == 0)
            {
                repetir = false;
            }
            else
            {
                Clear();
                switch (opcion)
                {
                    case 1:
                    {
                        auto start1 = chrono::high_resolution_clock::now();
                        cout << this->maxheap_number->get_value() << endl;
                        auto end1 = chrono::high_resolution_clock::now();
                        // Calculating total time taken by the program.
                        calculate_time(start1, end1);
                        ingrese_0_para_salir();
                        break;
                    }
                    case 2:
                    {
                        auto start1 = chrono::high_resolution_clock::now();
                        cout << this->maxheap_date->get_value() << endl;
                        auto end1 = chrono::high_resolution_clock::now();
                        // Calculating total time taken by the program.
                        calculate_time(start1, end1);
                        ingrese_0_para_salir();
                        break;
                    }
                    default:
                        cout << "error in opcion " << endl;
                        ingrese_0_para_salir();
                        break;
                }
            }
        } while (repetir);
    }

    void min_valor()
    {
        int opcion;
        bool repetir = true;
        do
        {
            Clear();
            cout << "\n\n        Choose an attribute: " << endl;
            cout << "--------------------" << endl;
            cout << "1. number" << endl;
            cout << "2. tiempo" << endl;
            cout << "0. End" << endl;

            cout << "\nEnter an option: ";
            cin >> opcion;

            while (opcion != 0 and opcion != 1 and opcion != 2)
            {
                cerr << endl
                     << "Enter a valid option";
                cout << "\nEnter an option: ";
                cin >> opcion;
            }

            if (opcion == 0)
            {
                repetir = 0;
            }
            else
            {
                Clear();
                switch (opcion)
                {
                    case 1:
                    {
                        auto start1 = chrono::high_resolution_clock::now();
                        cout << this->minheap_number->get_value() << endl;
                        auto end1 = chrono::high_resolution_clock::now();
                        // Calculating total time taken by the program.
                        calculate_time(start1, end1);
                        ingrese_0_para_salir();
                        break;
                    }
                    case 2:
                    {
                        auto start1 = chrono::high_resolution_clock::now();
                        cout << this->minheap_date->get_value() << endl;
                        auto end1 = chrono::high_resolution_clock::now();
                        calculate_time(start1, end1);
                        ingrese_0_para_salir();
                        break;
                    }
                    default:
                        cout << "error in opcion " << endl;
                        ingrese_0_para_salir();
                        break;
                }
            }
        } while (repetir);
    }

//    template <typename T>
//    string simple_search(T x){
//
//    }
};
