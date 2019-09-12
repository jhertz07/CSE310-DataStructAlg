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
    
    // create an array of pointers to store each array of the shifted word
    char** original_arr = new char* [length-1];
    char temp; // used for shifting array elements
    
    // move each char over to the left and save into new array
    for (int i = 0; i < length-1; i++) {
        
//        // print char array for testing
//        for (int v = 0; v < length; v++) {
//            cout << word_arr[v] << " ";
//        }
//        cout << "\n";
        
        original_arr[i] = word_arr;
        temp = word_arr[0];
        // loop to shift elements left
        for (int j = 0; j < length-1; j++) { // stop loop one before last element
            word_arr[j] = word_arr[j+1];
        }
        word_arr[length-1] = temp; // make last element equal temp (first element)
        cout << original_arr[i] <<"\n";
    }
    
    
//    for (int i = 0; i <= length; i++) {
//        cout << i << "\n";
//        for (int j = 0; j < length; j++) {
//            cout << original_arr[i] << " ";
//        }
//        cout << "\n";
//    }
    
    return 0;
}
