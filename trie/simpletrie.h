//
// Created by Neo Zapata on 30/11/22.
//

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

    void insert(string key)
    {
        if (this->root == nullptr)
            this->root = new TrieNode();
        TrieNode *node = this->root;
        int index;
        for (int i = 0; i < key.size(); i++)
        {
            index = key[i] - 'a';
            if (node->children[index] == nullptr)
                node->children[index] = new TrieNode();
            node = node->children[index];
        }
        node->endWord = true;
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
