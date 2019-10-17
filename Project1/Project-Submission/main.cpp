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

void insertionSort(char**, int);
void print_arr(char**, int);
void encode (char**, int, string);
void mergeSort(char**, int, int);
void Merge(char**, int, int, int);

int main(int argc, const char * argv[]) {
    //cout << "Input string here\n"; // input string to be encoded
        int length;
        string word;
    bool insertion_sort;
        // detect merge or insertion input
        if (strcmp(argv[1], "insertion") == 0) {
            insertion_sort = true;
        }
        else if (strcmp(argv[1], "merge") == 0) {
            insertion_sort = true;
        }
        else {
            cout << "please specify 'insertion' or 'merge'";
            return 0;
        }
        while(!getline(cin, word).eof()){ // store input in 'word'
            //put string into an array
            length = (unsigned)word.length(); // get length of string
            
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
            if (insertion_sort) {
                insertionSort(shift_arr, length); // look up how to pass pointers (dereference?)
            }
            else if (!insertion_sort) {
                mergeSort(shift_arr, 0, length);
            }
            //cout << "\nsorted array:\n";
            //print_arr(shift_arr, length);
            encode(shift_arr, length, word);
            // delete dynamic arraiy
            // for (int i = 0; i < length; i++) {
            // delete[] shift_arr[i];
            // }
            delete[] shift_arr;
        }
    return 0;
}

void insertionSort(char** unsorted_arr, int length) {
    string temp;
    int j;
    for (int i = 1; i < length; i++) {
        j = i;
        // find ASCII value by printing int(char)
        while (j > 0 && int(unsorted_arr[j-1][0]) >= int(unsorted_arr[j][0])) {
            // if ascii value equal, then check next letter until a non-equivalence is found
            if (int(unsorted_arr[j-1][0]) == int(unsorted_arr[j][0])) {
                int z = 1;
                while (int(unsorted_arr[j-1][z]) == int(unsorted_arr[j][z])) {
                    if ((z+1) == length) break;
                    z++;
                }
                // if the letter is less than or equal in ascii value, then stop shifting
                if (int(unsorted_arr[j-1][z]) <= int(unsorted_arr[j][z])) {
                    break;
                }
            }
            // shift index to the left
            temp = unsorted_arr[j];
            strcpy(unsorted_arr[j], unsorted_arr[j-1]);
            strcpy(unsorted_arr[j-1], temp.c_str());
            j--;
        }
    }
}
void mergeSort(char* arr[], int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;
        
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        
        Merge(arr, l, m, r);
    }
}
void Merge(char* arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
    
    // create temp arrays
    char* L[n1];
    char* R[n2];
    
    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
    
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    /* Copy the remaining elements of L[], if there
     are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    /* Copy the remaining elements of R[], if there
     are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}
void encode (char** array, int length, string original) {
    char output; // char to be outputted (last letter of each string
    int cluster = 1; // size of cluster
    // find and output original index
    for (int i = 0; i < length; i++) {
        // check for original word index
        if (array[i] == original) {
            // add index number to first index of encoded array
            cout << i << "\n";
        }
    }
    // encoded output
    for (int i = 0; i < length; i++) {
        output = array[i][length-1];
        if ((i < (length-1)) && array[i+1][length-1] == output) {
            cluster++;
        }
        else {
            cout << cluster << " " << output << " "; // *THERE SHOULDN'T BE A SPACE AT THE VERY END*
            cluster = 1;
        }
    }
    cout << "\n";
}

// print function
void print_arr(char** arr, int length) {
    for (int i = 0; i < length; i++) {
        cout << arr[i] << "\n";
    }
}
