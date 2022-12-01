#include <iostream>
#include "sha256.h"
#include "blockchain.cpp"

using namespace std;
int INDICE = 1;
#define FILENAME "MOCK_DATA_DEMO.csv"
#define DELIM ','

void ingrese_0_para_salir();
void query(Blockchain &cryptochain);
void bstree_query(Blockchain &cryptochain);
//void choose_atr(Blockchain &cryptochain, int query_type);
void display_bstree(Blockchain &cryptochain);

void filling_blockchain_test(Blockchain &cryptochain, const string& filename, int dificulty){
    ifstream file;
    file.open(filename);
    if(file.is_open()){
        cryptochain.read_and_load_csv(INDICE, filename, DELIM, dificulty);
    } else {
        cerr << "Error opening file. (filling_blockchain_test)" << endl;
    }
}

void Clear()
{
    cout << "\x1B[2J\x1B[H";
}

void add_transaction_in_specific_block(Blockchain &cryptochain, int dificulty){
    Clear();
    int n_block;
    cout << "\n\n There are " << cryptochain.chain.size() << " blocks." << endl;
    cout << "Select block to add transactions:" << endl;
    cout << "--------------------"<<endl;

    cout << "\nEnter an option: ";
    cin >> n_block;

    while(n_block < 0 || n_block > cryptochain.chain.size()){
        cerr << endl << "Enter a valid option:";
        cout << "\nEnter an option: ";
        cin >> n_block;
    }

    Clear();
    cout << "Add transaction " << endl;
    cout << "--------------------"<<endl;
    ForwardList<Transaction> transactions;
    int opcion;
    do {
        Transaction n_transaction;
        n_transaction.create_transaction(INDICE);
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
    // add to specific block
    cryptochain.chain[n_block].insert_transactions(transactions);
//    cryptochain.add_block(transactions, dificulty);
}


void add_block_with_n_transactions(Blockchain &cryptochain, int dificulty){
    Clear();
    cout << "\n\n Creating new block!" << endl;
    cout << "Add transaction " << endl;
    cout << "--------------------"<<endl;
    ForwardList<Transaction> transactions;
    int opcion;
    do {
        Transaction n_transaction;
        n_transaction.create_transaction(INDICE);
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
    cryptochain.add_block(transactions, dificulty);
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
    cryptochain.fix();
}

int insert_difficulty(){
    int difficulty;
    cout << "\n\n        Insert dificulty for block inserting " << endl;
    cout << "--------------------"<<endl;
    cout << "1. fastest" << endl;
    cout << "2. fast" << endl;
    cout << "3. normal" << endl;
    cout << "4. slow" << endl;
    cout << "5. the slowest" << endl;
//    cout << "0. End" << endl;

    cout << "\nEnter an option: ";
    cin >> difficulty;

    while(difficulty != 1 and difficulty != 2 and difficulty != 3 and difficulty != 4 and difficulty != 5){
        cerr << endl << "Enter a valid option";
        cout << "\nEnter an option: ";
        cin >> difficulty;
    }
    return difficulty;
}

void Interactive_Menu(){
    Blockchain cryptochain;
    int opcion;
    bool repetir = true;
    int difficulty = insert_difficulty();

    do {
        Clear();
        cout << "\n\n        Menu " << endl;
        cout << "--------------------"<<endl;
        cout << "1. Read and load data from .csv" << endl;
        cout << "2. Add transactions in specific block" << endl;
        cout << "3. Add new block (with transactions)" << endl;
        cout << "4. Modify transaction" << endl;
        cout << "5. Show blocks" << endl;
        cout << "6. Display transactions" << endl;
        cout << "7. query" << endl;
        cout << "8. clean cryptochain" << endl;
        cout << "9. change difficulty" << endl;
        cout << "0. End" << endl;

        cout << "\nEnter an option: ";
        cin >> opcion;

        while(opcion != 0 and opcion != 1 and opcion != 2 and opcion != 3 and opcion != 4 and opcion != 5 and opcion != 6 and opcion != 7 and opcion != 8 and opcion != 9){
            cerr << endl << "Enter a valid option";
            cout << "\nEnter an option: ";
            cin >> opcion;
        }

        switch (opcion) {
            case 1:
                Clear();
                filling_blockchain_test(cryptochain, FILENAME, difficulty);
                ingrese_0_para_salir();
                break;
            case 2:
                add_transaction_in_specific_block(cryptochain, difficulty); // add transaction instead of add transaction, similar
                break;
            case 3:
                add_block_with_n_transactions(cryptochain, difficulty);
                break;

            case 4:
                modification(cryptochain);
                break;

            case 5:
                Clear();
                cryptochain.display();
                ingrese_0_para_salir();
                break;

            case 6:
                Clear();
                int block;
                do {
                    cout << "\n there are " << cryptochain.chain.size() << " blocks." << endl;
                    cout << "\n Select block: ";
                    cin >> block;
                } while(block < 1 or block > cryptochain.chain.size());

                cout << "\n\n Displaying all transactions from block " << block << endl;
                cout << "--------------------"<<endl;
                cryptochain.chain[block].display_transactions();
                ingrese_0_para_salir();
                break;

            case 7:
                query(cryptochain);
                break;
            
            case 8:
//                display_bstree(cryptochain);
                cryptochain.purge();
                break;
            case 9:
                difficulty = insert_difficulty();
                break;
            case 0:
                repetir = false;
                break;
            default:
                cout << "Invalid option." << endl;
                break;
        }

        cryptochain.fix();
    } while (repetir);
    cryptochain.display();
}

void display_bstree(Blockchain &cryptochain){
    int opcion;
    bool repetir = true;
    do{
        Clear();
        cout << "\n\n        Choose a bstree to show in order: " << endl;
        cout << "--------------------"<<endl;
        cout << "1. bstree_open" << endl;
        cout << "2. bstree_high" << endl;
        cout << "3. bstree_lowest" << endl;
        cout << "4. bstree_close" << endl;
        cout << "5. bstree_volume" << endl;
        cout << "6. bstree_trades" << endl;
        cout << "0. End" << endl;

        cout << "\nEnter an option: ";
        cin >> opcion;

        while(opcion != 0 and opcion != 1 and opcion != 2 and opcion != 3 and opcion != 4 and opcion != 5 and opcion != 6){
            cerr << endl << "Enter a valid option";
            cout << "\nEnter an option: ";
            cin >> opcion;
        }
        if (opcion == 0){
            repetir = 0;
        } else {
            cryptochain.display_bstree(opcion);
            ingrese_0_para_salir();
        }
    } while(repetir);
}

void query(Blockchain &cryptochain){
    int opcion;
    bool repetir = true;
    do{
        Clear();
        cout << "\n\n        Choose a query type: " << endl;
        cout << "--------------------"<<endl;
        cout << "1. Igual a X" << endl;
        cout << "2. Entre X y Y" << endl;
        cout << "3. Inicia con" << endl;
        cout << "4. Está contenido en" << endl;
        cout << "5. Máximo valor de" << endl;
        cout << "6. Mínimo valor de" << endl;
        cout << "0. End" << endl;

        cout << "\nEnter an option: ";
        cin >> opcion;

        while(opcion != 0 and opcion != 1 and opcion != 2 and opcion != 3 and opcion != 4 and opcion != 5 and opcion != 6){
            cerr << endl << "Enter a valid option";
            cout << "\nEnter an option: ";
            cin >> opcion;
        }
        if (opcion == 0){
            repetir = false;
        } else {
            switch(opcion){
                case 1:
                    cryptochain.igual_a_x();
                    ingrese_0_para_salir();
                    break;
                case 2:
                    cryptochain.entre_x_y();
                    ingrese_0_para_salir();
                    break;
                case 3:
                    cryptochain.inicia_con();
                    ingrese_0_para_salir();
                    break;
                case 4:
                    cryptochain.contenido_en();
                    ingrese_0_para_salir();
                    break;
                case 5:
                    cryptochain.max_valor();
                    ingrese_0_para_salir();
                    break;
                case 6:
                    cryptochain.min_valor();
                    ingrese_0_para_salir();
                    break;
                default:
                    cout << "option not valid" << endl;
                    break;
            }
//            choose_atr(cryptochain, opcion);
        }
    } while(repetir);
}

void ingrese_0_para_salir(){
    int F6;
    do{
        cout<<endl<< "Ingrese 0 para salir: ";
        cin>> F6;
        Clear();
    }while(F6 != 0);
}

void choose_atr(Blockchain &cryptochain, int query_type){
    int opcion;
    bool repetir = true;
    do {
        Clear();
        cout << "\n\n        Choose an atribute to query: " << endl;
        cout << "--------------------"<<endl;
        cout << "1. str1" << endl;
        cout << "2. str2" << endl;
        cout << "3. number" << endl;
        cout << "4. tiempo" << endl;
        cout << "0. End" << endl;

        cout << "\nEnter an option: ";
        cin >> opcion;

        while(opcion != 0 and opcion != 1 and opcion != 2 and opcion != 3 and opcion != 4){
            cerr << endl << "Enter a valid option";
            cout << "\nEnter an option: ";
            cin >> opcion;
        }

        switch (opcion) {
            case 1:
                break;

            case 2:
                // cryptochain.bstree_high.solve_query(query_type)
                ingrese_0_para_salir();
                break;

            case 3:
                // cryptochain.bstree_lowest.solve_query(query_type)
                ingrese_0_para_salir();
                break;

            case 4:
                // cryptochain.bstree_close.solve_query(query_type)
                ingrese_0_para_salir();
                break;
            case 0:
                repetir = false;
                break;
            default:
                cout << "Invalid option." << endl;
                break;
        }
    } while (repetir);
}

//void sha256_test(){
//    string input = "something";
//    string output = sha256(input);
//    cout << "sha256(" << input <<") = " << output << endl;
//}

int main(){ // test the blockchain, if it is reading correctly the data.
    // sha256_test();
    Interactive_Menu();
    return 0;
}
