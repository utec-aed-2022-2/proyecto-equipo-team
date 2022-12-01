#ifndef CRYPTOCHAIN_SIMPLETRIE_H
#define CRYPTOCHAIN_SIMPLETRIE_H

#include <iostream>
#include "trie.h"

using namespace std;

class TrieSimple : public Trie
{
private:
    struct TrieNode
    {
        // opcion 1:   TrieNode **children;
        // opcion 2:   list<pair<char, TrieNode*>> children;
        // opcion 3:   map<pair<char, TrieNode*>> children;
        int index; // --
        TrieNode **children;
        bool endWord; // indica si es final de una palabra

        TrieNode()
        {
            this->children = new TrieNode *[ALPHA_SIZE];
            for (int i = 0; i < ALPHA_SIZE; ++i)
                this->children[i] = nullptr;
            this->endWord = false;
        }
        void killSelf()
        {
            for (int i = 0; i < ALPHA_SIZE; ++i)
                delete this->children[i];
            delete[] this->children;
            delete this;
        }
    };

    TrieNode *root;

public:
    TrieSimple() : root(nullptr) {}

    void insert(int indice, string key)
    {
        if (this->root == nullptr)
            this->root = new TrieNode();
        TrieNode *node = this->root;
        int index;
        for (int i = 0; i < key.size(); i++)
        {
            index = tolower(key[i]) - 'a';
            if (node->children[index] == nullptr)
                node->children[index] = new TrieNode();
            node = node->children[index];
        }
        node->endWord = true;
        node->index = indice;
    }

    bool search(string key)
    {
        auto iter = this->root;
        int index = 0;
        for (int i = 0; i < key.size(); i++)
        {
            index = key[i] - 'a';
            if (iter->children[index] != nullptr && iter->children[index]->endWord)
                return true;
            iter = iter->children[index];
        }

        return false;
    }

    ForwardList<int> contained_in(string x){
        ForwardList<string> strings;
        string estr;
        char str[20];
        display(strings, estr, root, str, 0);
        for(int i = 1 ; i <= strings.size() ; i++){
//            testIntuitivo(x, strings[i]);
//            for(int j = 0 ; j < strings[i].size() ; j++){
            if(match(x, strings[i])){
                cout << strings[i] << endl;
            }
//            }
        }
    }

    bool match(string x, string y){
        int counter1 = 0;
        for(int i = 0 ; i < x.size() ; i++){
            while(y[counter1] == x[i]){
                if(counter1 == y.size() - 1){
                    return true;
                }
                counter1++;
            }
            counter1 = 0;
        }
    }

    int* buildPositions(string patron){
        int* positions = new int[256];
        for(int i=0;i<256;i++) positions[i] = -1;
        for(int i=0;i<(int)patron.size();i++)
            positions[(int)patron[i]] = i;
        return positions;
    }

    void testIntuitivo(string texto, string patron){
//        auto start = std::chrono::steady_clock::now();
        int n = texto.size();
        int m = patron.size();

        int result = {};
        for (int j = 0; j < n-m; j++){
            int i = 0;
            while(patron[i] == texto[i] && i < m){
                i++;
            }
            if (i == m){
                result += j;
            }
        }
//        auto end = std::chrono::steady_clock::now();
//        cout<<"Intuitivo:"<<chrono::duration_cast<chrono::microseconds>(end-start).count()<<endl;
    }

    string testBoyerMoore(string texto, string patron){
        int m = patron.size();
        int n = texto.size();
        string rslt;

        int* positions = buildPositions(patron);

        int s = 0;
        while(s <= (n - m))
        {
            int j = m - 1;
            while(j >= 0 && patron[j] == texto[s + j])
                j--;
            if (j < 0)
            {
//                cout << "pattern occurs at shift = " <<  s << endl;
//                rslt.push_back(s);
                return patron;
                s += (s + m < n)? m-positions[texto[s + m]] : 1;
            }
            else
                s += max(1, j - positions[texto[s + j]]);
        }
        return rslt;
    }

    void remove(string key)
    {
        if (!search(key))
            throw("Key doesn't exit in trie");
        remove(this->root, key, 0);
    }

    // imprime ordenado
    string toString(string sep)
    {
        return toString(this->root, sep, "");
    }

    void display(ForwardList<string>& strings, string& estr, TrieNode* root, char str[], int level)
    {
        if (root->endWord)
        {
//            str[level] = '\0';
//            cout << str << endl;
            strings.push_back(estr);
            estr.clear();
        }
        for (int i = 0; i < ALPHA_SIZE; i++)
        {
            if (root->children[i])
            {
                str[level] = i + 'a';
                estr += i + 'a';
                display(strings, estr, root->children[i], str, level + 1);
            }
        }
    }

    ForwardList<int> starts_with(string str){
        ForwardList<int> rslt;

        auto iter = this->root;
        int index = 0;
        for (int i = 0; i < str.size(); i++)
        {
            index = tolower(str[i]) - 'a';
            if(iter->children[index] != nullptr)
                iter = iter->children[index];
        }
        // iter node
        diving(iter, rslt);

        return rslt;
    }

    void diving(TrieNode* iter, ForwardList<int>& result){
        if(iter->endWord){
            result.push_back(iter->index);
//            cout << endl;
        }
        for (int i = 0; i < ALPHA_SIZE; i++)
        {
            if (iter->children[i] != nullptr)
//                cout << char('a' + i);
                diving(iter->children[i], result);
        }
    }

private:

    string toString(TrieNode *node, string sep, string prefix)
    {
        if (node == nullptr)
            return "";
        string result = "";

        if (node->endWord)
            result += prefix + sep;

        for (int i = 0; i < ALPHA_SIZE; i++)
        {
            if (node->children[i] != nullptr)
                result += toString(node->children[i], sep,
                                   prefix + char('a' + i));
        }
        return result;
    }

    bool hasChildren(TrieNode *node)
    {
        if (node == nullptr)
            return false;
        for (int i = 0; i < ALPHA_SIZE; i++)
        {
            if (node->children[i] != nullptr)
                return true;
        }

        return false;
    }

    void remove(TrieNode *node, string key, int i)
    {
        if (i < key.size())
        {
            int index = key[i] - 'a';
            i++;
            remove(node->children[index], key, i);
        }

        if (node->endWord && i >= key.size())
        {
            node->endWord = false;

            if (!hasChildren(node))
            {
                node->killSelf();
                node = nullptr;
            }
        }
        else
        {
            if (!hasChildren(node))
            {
                node->killSelf();
                node = nullptr;
            }
        }
    }
};

#endif //CRYPTOCHAIN_SIMPLETRIE_H
