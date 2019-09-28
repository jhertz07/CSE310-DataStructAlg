//
//  main.cpp
//  Project1
//
//  Created by Justin Hertzberg on 9/5/19.
//  Copyright © 2019 ASU. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <cstring>
using namespace std;

char** insertionSort(char**, int);
void print_arr(char**, int);
char** encode (char**, int, string);

int main(int argc, const char * argv[]) {
    //cout << "Input string here\n"; // input string to be encoded
    if (strcmp(argv[0], "insertion")) {
    string word;
    while(!getline(cin, word).eof()) { // store input in 'word'
    
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
    }
    //cout << "\nprinted array:\n";
    //print_arr(shift_arr, length);
    insertionSort(shift_arr, length);
    //cout << "\nsorted array:\n";
    //print_arr(shift_arr, length);
    encode(shift_arr, length, word);
    
    // delete dynamic array
        for (int i = 0; i < length; ++i) {
            delete[] shift_arr[i];
        }
        delete[] shift_arr;
    }
        
    return 0;
}

char** insertionSort(char** unsorted_arr, int length) {
    char** sorted = unsorted_arr;
    string temp;
    int j;
    for (int i = 1; i < length; i++) {
        j = i;
        // find ASCII value by printing int(char)
        while (j > 0 && int(sorted[j-1][0]) >= int(sorted[j][0])) {
            // if ascii value equal, then check next letter until a non-equivalence is found
            if (int(sorted[j-1][0]) == int(sorted[j][0])) {
                int z = 1;
                while (int(sorted[j-1][z]) == int(sorted[j][z])) {
                    if ((z+1) == length) break;
                    z++;
                }
                // if the letter is less than or equal in ascii value, then stop shifting
                if (int(sorted[j-1][z]) <= int(sorted[j][z])) {
                    break;
                }
            }
            // shift index to the left
            temp = sorted[j];
            strcpy(sorted[j], sorted[j-1]);
            strcpy(sorted[j-1], temp.c_str());
            j--;
        }
    }
    return sorted;
}
char** encode (char** array, int length, string original) {
    char** this_array = array; // copy array to local variable
    char output; // char to be outputted (last letter of each string
    int cluster = 1; // size of cluster
    // find and output original index
    for (int i = 0; i < length; i++) {
        // check for original word index
        if (this_array[i] == original) {
            // add index number to first index of encoded array
            cout << i << "\n";
        }
    }
    // encoded output
    for (int i = 0; i < length; i++) {
        output = this_array[i][length-1];
        if ((i < (length-1)) && this_array[i+1][length-1] == output) {
            cluster++;
        }
        else {
            cout << cluster << output;
            cluster = 1;
        }
    }
    cout << "\n";
    return this_array;
}

// print function
void print_arr(char** arr, int length) {
    for (int i = 0; i < length; i++) {
        cout << arr[i] << "\n";
    }
}
