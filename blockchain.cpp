#include <utility>
#include "block.cpp"
#include <iostream>
#include <fstream>
#include "forward.h"
#include "bstree/bst.h"
#include <chrono>
#include <iomanip>
#include "heaps/MaxHeap.h"
#include "heaps/MinHeap.h"
using namespace std;

#define num_transaction_in_block 100

class Blockchain
{
private:
    int size;
    static Block generate_genesis()
    { // index = 0
        ForwardList<Transaction> transactions;
        Transaction transaction;
        transactions.push_back(transaction);
        auto *ptr = new string("0");
        Block genesis(1, transactions, ptr); // index = 0 ; data = tra ; prev_hash_code = "0" | sha256("0") <?>
        return genesis;
    }

public:
    // bstrees
    BSTree<float> *bstree_open = new BSTree<float>();
    BSTree<float> *bstree_high = new BSTree<float>();
    BSTree<float> *bstree_lowest = new BSTree<float>();
    BSTree<float> *bstree_close = new BSTree<float>();
    BSTree<float> *bstree_volume = new BSTree<float>();
    BSTree<int> *bstree_trades = new BSTree<int>();

    MaxHeap<float> *maxheap_open = new MaxHeap<float>();
    MaxHeap<float> *maxheap_high = new MaxHeap<float>();
    MaxHeap<float> *maxheap_lowest = new MaxHeap<float>();
    MaxHeap<float> *maxheap_close = new MaxHeap<float>();
    MaxHeap<float> *maxheap_volume = new MaxHeap<float>();
    MaxHeap<int> *maxheap_trades = new MaxHeap<int>();

    MinHeap<float> *minheap_open = new MinHeap<float>();
    MinHeap<float> *minheap_high = new MinHeap<float>();
    MinHeap<float> *minheap_lowest = new MinHeap<float>();
    MinHeap<float> *minheap_close = new MinHeap<float>();
    MinHeap<float> *minheap_volume = new MinHeap<float>();
    MinHeap<int> *minheap_trades = new MinHeap<int>();

    void display_bstree(int opcion)
    {
        switch (opcion)
        {
        case 1:
            cout << bstree_open->displayInOrder() << endl;
            break;
        case 2:
            cout << bstree_high->displayInOrder() << endl;
            break;
        case 3:
            cout << bstree_lowest->displayInOrder() << endl;
            break;
        case 4:
            cout << bstree_close->displayInOrder() << endl;
            break;
        case 5:
            cout << bstree_volume->displayInOrder() << endl;
            break;
        case 6:
            cout << bstree_trades->displayInOrder() << endl;
            break;
        default:
            cerr << "Option is invalid" << endl;
            return;
        }
    }

    ForwardList<Block> chain; // needs our own implementation of vector to allocate memory dynamically [TODO] (maybe a forward circular linked list)

    Blockchain()
    {
        if (chain.is_empty())
        {
            Block genesis = generate_genesis();
            chain.push_back(genesis);
            size = 1; // deberia ser 1, pero como empezamos a contar desde 1, tenemos que añadir 1 al size para coordinar con los indices
        }
        else
        {
            cerr << "Error creating blockchain" << endl;
        }
    }

    Block *get_latest_block()
    { // important not to use copies
        if (size != 0)
        {
            return chain.back();
        }
        else
        {
            cerr << "Chain is empty" << endl;
            return nullptr;
        }
    }

    void add_to_bstrees(const Block &new_block)
    {
        ForwardList<Transaction> trans = new_block.transactions;
        for (int i = 1; i <= trans.size(); i++)
        {
            this->bstree_close->insert(trans[i].close);
            this->bstree_high->insert(trans[i].high);
            this->bstree_lowest->insert(trans[i].lowest);
            this->bstree_open->insert(trans[i].open);
            this->bstree_trades->insert(trans[i].number_of_trades);
            this->bstree_volume->insert(trans[i].volume);
        }
    }

    void add_to_maxheaps(const Block &new_block)
    {
        ForwardList<Transaction> trans = new_block.transactions;
        for (int i = 1; i <= trans.size(); i++)
        {
            this->maxheap_close->push(trans[i].close);
            this->maxheap_high->push(trans[i].high);
            this->maxheap_lowest->push(trans[i].lowest);
            this->maxheap_open->push(trans[i].open);
            this->maxheap_trades->push(trans[i].number_of_trades);
            this->maxheap_volume->push(trans[i].volume);
        }
    }

    void add_to_minheaps(const Block &new_block)
    {
        ForwardList<Transaction> trans = new_block.transactions;
        for (int i = 1; i <= trans.size(); i++)
        {
            this->minheap_close->push(trans[i].close);
            this->minheap_high->push(trans[i].high);
            this->minheap_lowest->push(trans[i].lowest);
            this->minheap_open->push(trans[i].open);
            this->minheap_trades->push(trans[i].number_of_trades);
            this->minheap_volume->push(trans[i].volume);
        }
    }

    void add_block(const ForwardList<Transaction> &transactions, int dificulty)
    { // if we use string or transaction here will depend on the implementation of transaction
        int index = size + 1;
        string *latest_block_hash_code = get_latest_block()->get_hash_code();
        Block new_block(index, transactions, latest_block_hash_code);

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
        display_block(new_block);
        size++;
    }

    void fix()
    {
        int index1 = 1;
        bool cascade = false;
        for (int i = 1; i <= this->size; i++)
        {
            if (this->chain[i].valid == false)
                cascade = true;
            if (cascade)
            {
                this->chain[i].valid = false;
                // this->chain[i].update();
            }
        }
    }

    void read_and_load_csv(const string &filename, char delim = ',', int dificulty = 2)
    {
        ForwardList<Transaction> transactions;
        ifstream file(filename);

        if (file.is_open())
        {
            int number_of_transaction_read = 0; // this is important to store transaction into blocks
            string line, field;

            getline(file, line); // getting the first line (it only has the column names)

            while (getline(file, line))
            { // getting data line by line

                Transaction new_transaction;
                stringstream ss(line); // getting data line by line into stringstream

                getline(ss, field, delim);
                Time new_time(field);
                new_transaction.date = new_time;

                int aux = 1;
                while (getline(ss, field, delim))
                { // getting the rest of data
                    switch (aux)
                    {
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
                    }
                    aux++;
                }
                transactions.push_back(new_transaction);
                number_of_transaction_read++;
                if (number_of_transaction_read >= num_transaction_in_block)
                {
                    number_of_transaction_read = 0;
                    add_block(transactions, dificulty); // after this line, the block should be succesfully inserted in the blockchain
                    transactions.clear();
                }
            }
            add_block(transactions, dificulty); // adding the last transaction that couldn't make it to fill a block
            transactions.clear();
            file.close();
        }
        else
        {
            cerr << "error while opening file (read_csv_innto_transaction)" << endl;
        }
    }

    void display()
    {
        cout << "Size of chain: " << chain.size() << endl;

        for (int i = 1; i <= chain.size(); i++)
        {
            cout << " --------------------------------------------------------------------------------------------------" << endl;
            cout << "|                                                                                                  |" << endl;
            chain[i].short_display();
            cout << "|                                                                                                  |" << endl;
            cout << " --------------------------------------------------------------------------------------------------" << endl;
            cout << "                                                    |" << endl;
            cout << "                                                    V" << endl;
        }
    }

    static void display_block(Block block)
    {
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

    void entre_x_y()
    {
        int opcion;
        bool repetir = true;
        do
        {
            Clear();
            cout << "\n\n        Choose an attribute: " << endl;
            cout << "--------------------" << endl;
            cout << "1. open" << endl;
            cout << "2. high" << endl;
            cout << "3. lowest" << endl;
            cout << "4. close" << endl;
            cout << "5. volume" << endl;
            cout << "6. trades" << endl;
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
                float x, y;
                Clear();
                cout << "\n\n        Choose a range: " << endl;
                cout << "--------------------" << endl;
                cout << "\nEnter X: ";
                cin >> x;
                Clear();
                cout << "\n\n        Choose a range: " << endl;
                cout << "--------------------" << endl;
                cout << "\nEnter Y: ";
                cin >> y;
                // function
                // find x in opcion tree and print in order until node == y
                Clear();
                switch (opcion)
                {
                case 1:
                {
                    auto start1 = chrono::high_resolution_clock::now();
                    this->bstree_open->range_search(x, y);
                    auto end1 = chrono::high_resolution_clock::now();
                    // Calculating total time taken by the program.
                    double time_taken1 = chrono::duration_cast<chrono::nanoseconds>(end1 - start1).count();
                    time_taken1 *= 1e-9;
                    cout << endl
                         << "Time taken by program is : " << time_taken1 << setprecision(9);
                    cout << " sec" << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
                    break;
                }
                case 2:
                {
                    auto start2 = chrono::high_resolution_clock::now();
                    this->bstree_high->range_search(x, y);
                    auto end2 = chrono::high_resolution_clock::now();
                    // Calculating total time taken by the program.
                    double time_taken2 = chrono::duration_cast<chrono::nanoseconds>(end2 - start2).count();
                    time_taken2 *= 1e-9;
                    cout << endl
                         << "Time taken by program is : " << time_taken2 << setprecision(9);
                    cout << " sec" << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
                    break;
                }
                case 3:
                {
                    auto start3 = chrono::high_resolution_clock::now();
                    this->bstree_lowest->range_search(x, y);
                    auto end3 = chrono::high_resolution_clock::now();
                    // Calculating total time taken by the program.
                    double time_taken3 = chrono::duration_cast<chrono::nanoseconds>(end3 - start3).count();
                    time_taken3 *= 1e-9;
                    cout << endl
                         << "Time taken by program is : " << time_taken3 << setprecision(9);
                    cout << " sec" << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
                    break;
                }
                case 4:
                {
                    auto start4 = chrono::high_resolution_clock::now();
                    this->bstree_close->range_search(x, y);
                    auto end4 = chrono::high_resolution_clock::now();
                    // Calculating total time taken by the program.
                    double time_taken4 = chrono::duration_cast<chrono::nanoseconds>(end4 - start4).count();
                    time_taken4 *= 1e-9;
                    cout << endl
                         << "Time taken by program is : " << time_taken4 << setprecision(9);
                    cout << " sec" << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
                    break;
                }
                case 5:
                {
                    auto start5 = chrono::high_resolution_clock::now();
                    this->bstree_volume->range_search(x, y);
                    auto end5 = chrono::high_resolution_clock::now();
                    // Calculating total time taken by the program.
                    double time_taken5 = chrono::duration_cast<chrono::nanoseconds>(end5 - start5).count();
                    time_taken5 *= 1e-9;
                    cout << endl
                         << "Time taken by program is : " << time_taken5 << setprecision(9);
                    cout << " sec" << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
                    break;
                }
                case 6:
                {
                    auto start6 = chrono::high_resolution_clock::now();
                    this->bstree_trades->range_search(x, y);
                    auto end6 = chrono::high_resolution_clock::now();
                    // Calculating total time taken by the program.
                    double time_taken6 = chrono::duration_cast<chrono::nanoseconds>(end6 - start6).count();
                    time_taken6 *= 1e-9;
                    cout << endl
                         << "Time taken by program is : " << time_taken6 << setprecision(9);
                    cout << " sec" << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
                    break;
                }
                default:
                    cout << "error in opcion " << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
                    break;
                }
            }
        } while (repetir);
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
            cout << "1. open" << endl;
            cout << "2. high" << endl;
            cout << "3. lowest" << endl;
            cout << "4. close" << endl;
            cout << "5. volume" << endl;
            cout << "6. trades" << endl;
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
                Clear();
                switch (opcion)
                {
                case 1:
                {
                    auto start1 = chrono::high_resolution_clock::now();
                    cout << this->maxheap_open->get_value() << endl;
                    auto end1 = chrono::high_resolution_clock::now();
                    // Calculating total time taken by the program.
                    double time_taken1 = chrono::duration_cast<chrono::nanoseconds>(end1 - start1).count();
                    time_taken1 *= 1e-9;
                    cout << endl
                         << "Time taken by program is : " << time_taken1 << setprecision(9);
                    cout << " sec" << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
                    break;
                }
                case 2:
                {
                    auto start1 = chrono::high_resolution_clock::now();
                    cout << this->maxheap_high->get_value() << endl;
                    auto end1 = chrono::high_resolution_clock::now();
                    // Calculating total time taken by the program.
                    double time_taken1 = chrono::duration_cast<chrono::nanoseconds>(end1 - start1).count();
                    time_taken1 *= 1e-9;
                    cout << endl
                         << "Time taken by program is : " << time_taken1 << setprecision(9);
                    cout << " sec" << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
                    break;
                }
                case 3:
                {
                    auto start1 = chrono::high_resolution_clock::now();
                    cout << this->maxheap_lowest->get_value() << endl;
                    auto end1 = chrono::high_resolution_clock::now();
                    // Calculating total time taken by the program.
                    double time_taken1 = chrono::duration_cast<chrono::nanoseconds>(end1 - start1).count();
                    time_taken1 *= 1e-9;
                    cout << endl
                         << "Time taken by program is : " << time_taken1 << setprecision(9);
                    cout << " sec" << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
                    break;
                }
                case 4:
                {
                    auto start1 = chrono::high_resolution_clock::now();
                    cout << this->maxheap_close->get_value() << endl;
                    auto end1 = chrono::high_resolution_clock::now();
                    // Calculating total time taken by the program.
                    double time_taken1 = chrono::duration_cast<chrono::nanoseconds>(end1 - start1).count();
                    time_taken1 *= 1e-9;
                    cout << endl
                         << "Time taken by program is : " << time_taken1 << setprecision(9);
                    cout << " sec" << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
                    break;
                }
                case 5:
                {
                    auto start1 = chrono::high_resolution_clock::now();
                    cout << this->maxheap_volume->get_value() << endl;
                    auto end1 = chrono::high_resolution_clock::now();
                    // Calculating total time taken by the program.
                    double time_taken1 = chrono::duration_cast<chrono::nanoseconds>(end1 - start1).count();
                    time_taken1 *= 1e-9;
                    cout << endl
                         << "Time taken by program is : " << time_taken1 << setprecision(9);
                    cout << " sec" << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
                    break;
                }
                case 6:
                {
                    auto start1 = chrono::high_resolution_clock::now();
                    cout << this->maxheap_trades->get_value() << endl;
                    auto end1 = chrono::high_resolution_clock::now();
                    // Calculating total time taken by the program.
                    double time_taken1 = chrono::duration_cast<chrono::nanoseconds>(end1 - start1).count();
                    time_taken1 *= 1e-9;
                    cout << endl
                         << "Time taken by program is : " << time_taken1 << setprecision(9);
                    cout << " sec" << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
                    break;
                }
                default:
                    cout << "error in opcion " << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
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
            cout << "1. open" << endl;
            cout << "2. high" << endl;
            cout << "3. lowest" << endl;
            cout << "4. close" << endl;
            cout << "5. volume" << endl;
            cout << "6. trades" << endl;
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
                Clear();
                switch (opcion)
                {
                case 1:
                {
                    auto start1 = chrono::high_resolution_clock::now();
                    cout << this->minheap_open->get_value() << endl;
                    auto end1 = chrono::high_resolution_clock::now();
                    // Calculating total time taken by the program.
                    double time_taken1 = chrono::duration_cast<chrono::nanoseconds>(end1 - start1).count();
                    time_taken1 *= 1e-9;
                    cout << endl
                         << "Time taken by program is : " << time_taken1 << setprecision(9);
                    cout << " sec" << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
                    break;
                }
                case 2:
                {
                    auto start1 = chrono::high_resolution_clock::now();
                    cout << this->minheap_high->get_value() << endl;
                    auto end1 = chrono::high_resolution_clock::now();
                    // Calculating total time taken by the program.
                    double time_taken1 = chrono::duration_cast<chrono::nanoseconds>(end1 - start1).count();
                    time_taken1 *= 1e-9;
                    cout << endl
                         << "Time taken by program is : " << time_taken1 << setprecision(9);
                    cout << " sec" << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
                    break;
                }
                case 3:
                {
                    auto start1 = chrono::high_resolution_clock::now();
                    cout << this->minheap_lowest->get_value() << endl;
                    auto end1 = chrono::high_resolution_clock::now();
                    // Calculating total time taken by the program.
                    double time_taken1 = chrono::duration_cast<chrono::nanoseconds>(end1 - start1).count();
                    time_taken1 *= 1e-9;
                    cout << endl
                         << "Time taken by program is : " << time_taken1 << setprecision(9);
                    cout << " sec" << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
                    break;
                }
                case 4:
                {
                    auto start1 = chrono::high_resolution_clock::now();
                    cout << this->minheap_close->get_value() << endl;
                    auto end1 = chrono::high_resolution_clock::now();
                    // Calculating total time taken by the program.
                    double time_taken1 = chrono::duration_cast<chrono::nanoseconds>(end1 - start1).count();
                    time_taken1 *= 1e-9;
                    cout << endl
                         << "Time taken by program is : " << time_taken1 << setprecision(9);
                    cout << " sec" << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
                    break;
                }
                case 5:
                {
                    auto start1 = chrono::high_resolution_clock::now();
                    cout << this->minheap_volume->get_value() << endl;
                    auto end1 = chrono::high_resolution_clock::now();
                    // Calculating total time taken by the program.
                    double time_taken1 = chrono::duration_cast<chrono::nanoseconds>(end1 - start1).count();
                    time_taken1 *= 1e-9;
                    cout << endl
                         << "Time taken by program is : " << time_taken1 << setprecision(9);
                    cout << " sec" << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
                    break;
                }
                case 6:
                {
                    auto start1 = chrono::high_resolution_clock::now();
                    cout << this->minheap_trades->get_value() << endl;
                    auto end1 = chrono::high_resolution_clock::now();
                    // Calculating total time taken by the program.
                    double time_taken1 = chrono::duration_cast<chrono::nanoseconds>(end1 - start1).count();
                    time_taken1 *= 1e-9;
                    cout << endl
                         << "Time taken by program is : " << time_taken1 << setprecision(9);
                    cout << " sec" << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
                    break;
                }
                default:
                    cout << "error in opcion " << endl;
                    int V;
                    do
                    {
                        cout << endl
                             << "Ingrese 0 para salir: ";
                        cin >> V;
                        Clear();
                    } while (V != 0);
                    break;
                }
            }
        } while (repetir);
    }

    void modify_index(int block_index, int new_index)
    {
        Block to_modify = this->chain[block_index];
        to_modify.index = new_index;
        this->chain[block_index].update(to_modify);
    }

    void modify_nonce(int block_index, size_t new_nonce)
    {
        Block to_modify = this->chain[block_index];
        to_modify.nonce = new_nonce;
        this->chain[block_index].update(to_modify);
    }

    void modify_hashcode(int block_index, const string &new_hashcode)
    {
        Block to_modify = this->chain[block_index];
        to_modify.hash_code = new_hashcode;
        this->chain[block_index].update(to_modify);
    }

    void modify_prev_hashcode(int block_index, const string &new_prev_hashcode)
    {
        Block to_modify = this->chain[block_index];
        *(to_modify.prev_hash_code) = new_prev_hashcode;
        this->chain[block_index].update(to_modify);
    }

    void modify_transaction(int block_index, int index_transaction)
    {
        Block to_modify = this->chain[block_index];
        cout << "\n\n"
             << endl;
        Transaction new_transaction;
        new_transaction.create_transaction();
        to_modify.transactions[index_transaction] = new_transaction;
        this->chain[block_index].update(to_modify);
    }

    template <typename T>
    string simple_search(T x)
    {
    }
};