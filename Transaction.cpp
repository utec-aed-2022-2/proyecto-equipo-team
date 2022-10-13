#include <ctime>
#include <string>
using namespace std;

struct Transaction{ //ledger
    string data; // it would be better a .txt file, or to accept different types of entries [TODO]
    time_t timestamp;

    Transaction() = default;

    Transaction(string data){
        this->data = data;
        time_t current;
        timestamp = time(&current);
    }
};