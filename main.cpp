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

void query(Blockchain &cryptochain);
void bstree_query(Blockchain &cryptochain);
void choose_atr(Blockchain &cryptochain, int query_type);
void display_bstree(Blockchain &cryptochain);

void filling_blockchain_test(Blockchain &cryptochain, const string &filename, int dificulty)
{
    ifstream file;
    file.open(filename);
    if (file.is_open())
    {
        cryptochain.read_and_load_csv(filename, DELIM, dificulty);
    }
    else
    {
        cerr << "Error opening file. (filling_blockchain_test)" << endl;
    }
}

void Clear()
{
    cout << "\x1B[2J\x1B[H";
}

void add_transaction(Blockchain &cryptochain, int dificulty)
{
    Clear();
    cout << "\n\n Add transaction " << endl;
    cout << "--------------------" << endl;
    ForwardList<Transaction> transactions;
    int opcion;
    do
    {
        Transaction n_transaction;
        n_transaction.create_transaction();
        transactions.push_back(n_transaction);
        cout << "Do you want to add another transaction?" << endl;
        cout << "0. No" << endl;
        cout << "1. Yes" << endl;

        cout << "\nEnter an option: ";
        cin >> opcion;
        while (opcion != 0 and opcion != 1)
        {
            cerr << endl
                 << "Enter a valid option";
            cout << "\nEnter an option: ";
            cin >> opcion;
        }
    } while (opcion);
    cryptochain.add_block(transactions, dificulty);
}

void modification(Blockchain &cryptochain)
{
    Clear();
    int N;
    cout << "\n\n Modification " << endl;
    cout << "--------------------" << endl;
    do
    {
        cout << "Numero de bloque: ";
        cin >> N;
    } while (N < 1 or N > cryptochain.chain.size());
    // Incluir el modification
    int field;
    cout << "\n\n Select field to modify in block " << N << endl;
    cout << "--------------------" << endl;
    cout << "1. Index" << endl;
    cout << "2. Nonce" << endl;
    cout << "3. Hashcode" << endl;
    cout << "4. Prev Hashcode" << endl;
    cout << "5. Transactions" << endl;

    cout << "\nEnter an option: ";
    cin >> field;

    while (field != 1 and field != 2 and field != 3 and field != 4 and field != 5)
    {
        cerr << endl
             << "Enter a valid option";
        cout << "\nEnter an option: ";
        cin >> field;
    }

    string new_hashcode;
    string new_prev_hashcode;

    switch (field)
    {
    case 1:
        int new_index;
        cout << "\nEnter new index: ";
        cin >> new_index;
        cryptochain.modify_index(N, new_index);
        break;

    case 2:
        size_t new_nonce;
        cout << "\nEnter new nonce: ";
        cin >> new_nonce;
        cryptochain.modify_nonce(N, new_nonce);
        break;

    case 3:
        cout << "\nEnter new hashcode: ";
        cin >> new_hashcode;
        cryptochain.modify_hashcode(N, new_hashcode);
        break;

    case 4:
        cout << "\nEnter new prev. hashcode: ";
        cin >> new_prev_hashcode;
        cryptochain.modify_prev_hashcode(N, new_prev_hashcode);
        break;

    case 5:
        int index_transaction;
        cout << "\n\n Displaying all transactions of block " << N << endl;
        cout << "--------------------" << endl;
        cryptochain.chain[N].display_transactions();
        cout << "\nChoose a transaction to modify: ";
        cin >> index_transaction;

        while (index_transaction < 0 or index_transaction > cryptochain.chain[N].transactions.size())
        {
            cerr << endl
                 << "Enter a valid option";
            cout << "\nChoose a transaction to modify: ";
            cin >> index_transaction;
        }

        cryptochain.modify_transaction(N, index_transaction);
        break;
    default:
        cout << "Invalid option." << endl;
        break;
    }
    cryptochain.fix();
}

void Interactive_Menu()
{
    Blockchain cryptochain;
    int opcion;
    bool repetir = true;

    do
    {
        Clear();
        cout << "\n\n        Menu " << endl;
        cout << "--------------------" << endl;
        cout << "1. Read and load data from .csv" << endl;
        cout << "2. Add transaction" << endl;
        cout << "3. Modify transaction" << endl;
        cout << "4. Show blocks" << endl;
        cout << "5. Display transactions" << endl;
        cout << "6. query" << endl;
        cout << "7. display bstree" << endl;
        cout << "0. End" << endl;

        cout << "\nEnter an option: ";
        cin >> opcion;

        while (opcion != 0 and opcion != 1 and opcion != 2 and opcion != 3 and opcion != 4 and opcion != 5 and opcion != 6 and opcion != 7)
        {
            cerr << endl
                 << "Enter a valid option";
            cout << "\nEnter an option: ";
            cin >> opcion;
        }

        switch (opcion)
        {
        case 1:
            Clear();
            int V;
            int dificulty;
            cout << "\n\n        Insert dificulty for block inserting " << endl;
            cout << "--------------------" << endl;
            cout << "1. fastest" << endl;
            cout << "2. fast" << endl;
            cout << "3. normal" << endl;
            cout << "4. slow" << endl;
            cout << "5. the slowest" << endl;
            cout << "0. End" << endl;

            cout << "\nEnter an option: ";
            cin >> dificulty;

            while (dificulty != 0 and dificulty != 1 and dificulty != 2 and dificulty != 3 and dificulty != 4 and dificulty != 5)
            {
                cerr << endl
                     << "Enter a valid option";
                cout << "\nEnter an option: ";
                cin >> dificulty;
            }

            filling_blockchain_test(cryptochain, FILENAME, dificulty);
            do
            {
                cout << endl
                     << "Ingrese 0 para salir: ";
                cin >> V;
                Clear();
            } while (V != 0);
            break;

        case 2:
            dificulty = 2;
            add_transaction(cryptochain, dificulty);
            break;

        case 3:
            modification(cryptochain);
            break;

        case 4:
            Clear();
            int C;
            cryptochain.display();
            do
            {
                cout << endl
                     << "Ingrese 0 para salir: ";
                cin >> C;
                Clear();
            } while (C != 0);
            break;

        case 5:
            int block;

            do
            {
                cout << "\n Select block: ";
                cin >> block;
            } while (block < 1 or block > cryptochain.chain.size());

            cout << "\n\n Displaying all transactions of block " << block << endl;
            cout << "--------------------" << endl;
            cryptochain.chain[block].display_transactions();

            int G;
            do
            {
                cout << endl
                     << "Ingrese 0 para salir: ";
                cin >> G;
                Clear();
            } while (G != 0);

            break;

        case 6:
            query(cryptochain);
            break;

        case 7:
            display_bstree(cryptochain);
            break;

        case 0:
            repetir = false;
            break;
        default:
            cout << "Invalid option." << endl;
            break;
        }
    } while (repetir);
    cryptochain.fix();
    cryptochain.display();
}

void display_bstree(Blockchain &cryptochain)
{
    int opcion;
    bool repetir = true;
    do
    {
        Clear();
        cout << "\n\n        Choose a bstree to show in order: " << endl;
        cout << "--------------------" << endl;
        cout << "1. bstree_open" << endl;
        cout << "2. bstree_high" << endl;
        cout << "3. bstree_lowest" << endl;
        cout << "4. bstree_close" << endl;
        cout << "5. bstree_volume" << endl;
        cout << "6. bstree_trades" << endl;
        cout << "0. End" << endl;

        cout << "\nEnter an option: ";
        cin >> opcion;

        while (opcion != 0 and opcion != 1 and opcion != 2 and opcion != 3 and opcion != 4 and opcion != 5 and opcion != 6)
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
            cryptochain.display_bstree(opcion);
            int F;
            do
            {
                cout << endl
                     << "Ingrese 0 para salir: ";
                cin >> F;
                Clear();
            } while (F != 0);
        }
    } while (repetir);
}

void query(Blockchain &cryptochain)
{
    int opcion;
    bool repetir = true;
    do
    {
        Clear();
        cout << "\n\n        Choose a query type: " << endl;
        cout << "--------------------" << endl;
        cout << "1. Igual a X" << endl;
        cout << "2. Entre X y Y" << endl;
        cout << "3. Inicia con" << endl;
        cout << "4. Está contenido en" << endl;
        cout << "5. Máximo valor de" << endl;
        cout << "6. Mínimo valor de" << endl;
        cout << "0. End" << endl;

        cout << "\nEnter an option: ";
        cin >> opcion;

        while (opcion != 0 and opcion != 1 and opcion != 2 and opcion != 3 and opcion != 4 and opcion != 5 and opcion != 6)
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
            switch (opcion)
            {
            case 1:
                int F1;
                do
                {
                    cout << endl
                         << "Ingrese 0 para salir: ";
                    cin >> F1;
                } while (F1 != 0);
                break;
            case 2:
                cryptochain.entre_x_y();
                int F2;
                do
                {
                    cout << endl
                         << "Ingrese 0 para salir: ";
                    cin >> F2;
                } while (F2 != 0);
                break;
            case 3:
                cout << "no aplica" << endl;
                int F3;
                do
                {
                    cout << endl
                         << "Ingrese 0 para salir: ";
                    cin >> F3;
                } while (F3 != 0);
                break;
            case 4:
                cout << "no aplica" << endl;
                int F4;
                do
                {
                    cout << endl
                         << "Ingrese 0 para salir: ";
                    cin >> F4;
                    Clear();
                } while (F4 != 0);
                break;
            case 5:
                cryptochain.max_valor();
                int F5;
                do
                {
                    cout << endl
                         << "Ingrese 0 para salir: ";
                    cin >> F5;
                } while (F5 != 0);
                break;
            case 6:
                cryptochain.min_valor();
                int F5;
                do
                {
                    cout << endl
                         << "Ingrese 0 para salir: ";
                    cin >> F5;
                } while (F5 != 0);
                break;
            }
            //choose_atr(cryptochain, opcion);
        }
    } while (repetir);
}

void choose_atr(Blockchain &cryptochain, int query_type)
{
    int opcion;
    bool repetir = true;
    do
    {
        Clear();
        cout << "\n\n        Choose an atribute to query: " << endl;
        cout << "--------------------" << endl;
        cout << "1. open" << endl;
        cout << "2. high" << endl;
        cout << "3. lowest" << endl;
        cout << "4. close" << endl;
        cout << "5. volume" << endl;
        cout << "6. number of trades" << endl;
        cout << "0. End" << endl;

        cout << "\nEnter an option: ";
        cin >> opcion;

        while (opcion != 0 and opcion != 1 and opcion != 2 and opcion != 3 and opcion != 4 and opcion != 5 and opcion != 6)
        {
            cerr << endl
                 << "Enter a valid option";
            cout << "\nEnter an option: ";
            cin >> opcion;
        }

        switch (opcion)
        {
        case 1:
            Clear();
            float X;
            cout << "\n\n        Enter a price in open time to search: " << endl;
            cout << "--------------------" << endl;
            cout << "\nEnter an option: ";
            cin >> X;
            if (cryptochain.bstree_open->solve_query(X, query_type))
            {
                Clear();
                cout << "\n\n       FOUND!!! " << endl;
                cout << "--------------------" << endl;
            }
            else
            {
                Clear();
                cout << "\n\n   NOT FOUND!!! " << endl;
                cout << "--------------------" << endl;
            }
            int G;
            do
            {
                cout << endl
                     << "Ingrese 0 para salir: ";
                cin >> G;
                Clear();
            } while (G != 0);
            break;

        case 2:
            // cryptochain.bstree_high.solve_query(query_type)
            break;

        case 3:
            // cryptochain.bstree_lowest.solve_query(query_type)
            break;

        case 4:
            // cryptochain.bstree_close.solve_query(query_type)
            break;

        case 5:
            // cryptochain.bstree_volume.solve_query(query_type)
            break;

        case 6:
            // cryptochain.bstree_trades.solve_query(query_type)
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

// void sha256_test(){
//     string input = "something";
//     string output = sha256(input);
//     cout << "sha256(" << input <<") = " << output << endl;
// }

int main()
{ // test the blockchain, if it is reading correctly the data.
    // sha256_test();
    Interactive_Menu();
    return 0;
}

// bstree

// #include <iostream>
// #include <stdlib.h>
// #include "bst.h"
// #include "tester.h"

// using namespace std;

// int main()
// {
//     BSTree<int> *bstree = new BSTree<int>();
//     bstree->insert(8);
//     bstree->insert(3);
//     bstree->insert(1);
//     bstree->insert(6);
//     bstree->insert(4);
//     bstree->insert(7);
//     bstree->insert(10);
//     bstree->insert(14);
//     bstree->insert(13);

//     ASSERT(bstree->isBalanced() == false, "The function isBalanced is not working",1);

//     ASSERT(bstree->displayInOrder() == "1 3 4 6 7 8 10 13 14 ", "The function displayInOrder is not working",1);
//     ASSERT(bstree->displayBFS() == "8 3 10 1 6 14 4 7 13 ", "The function displayBFS is not working",1);
//     ASSERT(bstree->height() == 3, "The function height is not working",1);
//     cout<<bstree->height() <<endl;
//     ASSERT(bstree->minValue() == 1, "The function minValue is not working",1);
//     ASSERT(bstree->maxValue() == 14, "The function maxValue is not working",1);
//      ASSERT(bstree->size() == 9, "The function size is not working",1);
//     ASSERT(bstree->successor(7) == 8, "The function successor is not working",1);
//     // ASSERT(bstree->predecessor(4) == 3, "The function predecessor is not working",1);

//     // /*-----------------------iterator---------------------------------*/
//     // string result = "";
//     // BSTree<int>::iterator ite = bstree->begin(BSTIterator<int>::Type::PreOrder);
//     // while(ite != bstree->end()) {
//     //     result += std::to_string(*ite) + " ";
//     //     ++ite;
//     // }
//     // ASSERT(result == "8 3 1 6 4 7 10 14 13 ", "The PreOrder iterator is not working",3);

//     // result = "";
//     // ite = bstree->begin(BSTIterator<int>::Type::PostOrder);
//     // while(ite != bstree->end()) {
//     //     result += std::to_string(*ite) + " ";
//     //     ++ite;
//     // }
//     // ASSERT(result == "1 4 7 6 3 13 14 10 8 ", "The PostOrder iterator is not working",3);
//     // /*---------------------------------------------------------------*/

//     bstree->remove(10);
//     bstree->remove(8);
//     ASSERT(bstree->displayInOrder() == "1 3 4 6 7 13 14 ", "The function remove is not working",1);
//     bstree->clear();
//     bstree->insert(8);
//     ASSERT(bstree->size() == 1, "The function clear is not working",1);

//     // /*-----------------------BST from ordered array---------------------------------*/
//     // int array[] = {1,5,7,8,9,11,12,13,15,18,20,22,25,26};
//     bstree->buildFromOrderedArray(14);
//     // ASSERT(bstree->isBalanced() == true, "The function buildFromOrderedArray is not working",1);
//     // /*---------------------------------------------------------------*/

//     return 0;
// }