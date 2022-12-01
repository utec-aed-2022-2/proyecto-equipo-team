cmake_minimum_required(VERSION 3.23)
project(cryptochain)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(bstree)

add_executable(cryptochain
        bstree/bst.h
        bstree/bst_node.h
        bstree/iterator.h
        block.cpp
        blockchain.cpp
        forward.h
        list.h
        main.cpp
        node.h
        sha256.cpp
        sha256.h
        Transaction.cpp Tuple.h heap/MaxHeap.h heap/MinHeap.h hash/hash.h trie/trie.h trie/simpletrie.h)
