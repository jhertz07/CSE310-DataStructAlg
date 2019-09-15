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
    
    // create an array of pointers to store each array of the shifted word
    char** shift_arr = new char*[length];
    for (int i = 0; i < length; ++i) {
        shift_arr[i] = new char[length];
    }
    // Take the inputted word and save it to the first index of shift_arr
    strcpy(shift_arr[0], word.c_str());
    
    char temp; //temp variable to shift elements
    string copy; //temp variable to copy character arrays
    // Cyclic shift loop
    for (int i = 1; i < length; i++) {
        // copy previous shifted word to current index
        copy = shift_arr[i-1];
        strcpy(shift_arr[i], copy.c_str());
        // save first element in temp
        temp = shift_arr[i][0];
        // loop to shift elements left
        for (int j = 0; j < length-1; j++) { // stop loop one before last element
            shift_arr[i][j] = shift_arr[i][j+1];
        }
        shift_arr[i][length-1] = temp; // make last element equal temp (first element)
        cout << shift_arr[i] <<"\n"; // print shift_arr for visual purposes
    }
    return 0;
}

char** insertionSort(char** unsorted_arr, int length) {
    char** sorted = unsorted_arr;
    
    return sorted;
}
