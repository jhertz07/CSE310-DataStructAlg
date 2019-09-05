//
//  main.cpp
//  Project1
//
//  Created by Justin Hertzberg on 9/5/19.
//  Copyright © 2019 ASU. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, const char * argv[]) {
    cout << "Input string here\n"; // input string to be encoded
    string word;
    cin >> word; // store in 'word'
    
    //put string into an array
    int length = (unsigned)word.length(); // get length of string
    char* word_arr = new char[length]; // create char array using length
    strcpy(word_arr, word.c_str()); // copies string to char array
    
        // print char array for testing
        for (int i = 0; i < length; i++) {
            cout << word_arr[i] << " ";
        }
    
    // create an array of pointers to store each array of the shifted word
    char** original_arr = new char* [length+1];
    
    // move each char over to the left and save into new array
    for (int i = 0; i <= length+1; i++) {
        original_arr[i] = word_arr;
    }
    
    return 0;
}
