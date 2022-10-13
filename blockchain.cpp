#include <vector> // delete
#include "block.cpp"
class Blockchain{
private:
    Block generate_genesis(){ // index = 0
        Transaction tra;
        time_t current;
        // initialize all tra members
        tra.data = "0";
        tra.timestamp = time(&current);
        string prev_hash_code_from_genesis = "0";
        string* ptr = &prev_hash_code_from_genesis;
        Block genesis(0, tra, ptr); // index = 0 ; data = tra ; prev_hash_code = "0" | sha256("0") <?>
        return genesis;
    }
public:
    vector<Block> chain; // needs our own implementation of vector to allocate memory dynamically [TODO] (maybe a forward circular linked list)

    Blockchain(){
        Block genesis = generate_genesis();
        chain.push_back(genesis);
    }

    Block* get_latest_block(){ // important not to use copies
        return &chain.back();
    }

    void add_block(string data){ // if we use string or transaction here will depend on the implementation of transaction
        // Block new_block // here we create the new block
        int index = (int)chain.size()-1; // this may change bc it depends on the implementation of the vector we use, we only need to get the next index in the chain.
        string latest_block_hash_code = get_latest_block()->get_hash_code();
        string* not_optimal = &latest_block_hash_code;
        Block new_block(index, data, not_optimal);
    }

    bool is_chain_valid(){
        // implement our own iterator for our own vector
        // The idea here is to iterate over the chain asking if the current block is valid (Block has a method called is_valid() that compare the hash_code whit a new generation of a hash code)
        //  so if there are any changes somewhare on the chain we just return false, and maybe the index block
        return true;
    }


};