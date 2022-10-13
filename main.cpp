//
// Created by Neo Zapata on 12/10/22.
//
#include <iostream>
#include "sha256.h"

using namespace std;

int main(int argc, char *argv[]){
    string input = "something";
    string output = sha256(input);

    cout << "sha256(" << input <<") = " << output << endl;
    return 0;
}