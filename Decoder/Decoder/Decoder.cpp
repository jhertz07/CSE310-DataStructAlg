
//  Decode.cpp
//  Project1
//
//  Created by Justin Hertzberg on 9/29/19.
//  Copyright © 2019 ASU. All rights reserved.
//
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
using namespace std;

string lastString(string);
string insertionSortDecode(string);
void mergeSortDecode(char[], int, int);
void merge(char[], int, int, int);
int* nextIndex(string, string);
string decode(string, int*, int);

int main(int argc, const char * argv[]) {
    string str_origindex;
    int orig_index;
    bool is_int = true;
    string encoded;
    bool insertion_sort = false;
    // detect merge or insertion input
    if (strcmp(argv[1], "insertion") == 0) {
        insertion_sort = true;
    }
    else if (strcmp(argv[1], "merge") == 0) {
        insertion_sort = false;
    }
    else {
        cout << "please specify 'insertion' or 'merge'";
        return 0;
    }
    // read in line
    while(getline(cin, str_origindex)) {
        if (str_origindex == "") {
            cout << "\n";
        }
        else {
            // iteratre thru each character of string, if it is not an integer then break while-loop
            for (int i = 0; i < str_origindex.length(); i++) {
                if (!isdigit(str_origindex[i])) {
                    is_int = false;
                    break;
                }
            }
            if (!is_int) break; // exit while-loop if not digit
            try { // exit while loop if nothing is entered
                (orig_index = stoi(str_origindex));// convert string to int
            }
            catch (exception &a) {
                break;
            }
            // read in the encdoded input
            getline(cin, encoded);
            string last = lastString(encoded);
            // use either insertion sort or merge sort
            string sorted = "";
            if (insertion_sort == true) {
                sorted = insertionSortDecode(last);
            }
            else {
                int left = 0;
                int right = (unsigned)last.length()-1;
                // put the 'last' string into an in array
                char* last_arr = new char[last.length()];
                for (int i = 0; i < last.length(); i++) {
                    last_arr[i] = last[i];
                }
                mergeSortDecode(last_arr, left, right);
                sorted = last_arr;
                delete[] last_arr;
                last_arr = nullptr;
            }
            int* next_arr = nextIndex(sorted, last);
            string decoded = decode(last, next_arr, orig_index);
            
            delete[] next_arr;
            next_arr = nullptr;
            
            cout << decoded << "\n";
        }
    }
    return 0;
}

string lastString(string encoded) {
    string last = "";
    int num = 0;
    bool skipspace = false;
    
    for(int i = 0; i < encoded.length(); i++) {
        if (!skipspace) { // alternate skipping a character (to account for the spaces)
            num = encoded[i] - '0';
            i+=2;
            for (int j = 0; j < num; j++) {
                last += encoded[i];
            }
            skipspace = true;
        }
        else skipspace = false;
    }
    
    return last;
}

string insertionSortDecode(string last) {
    char temp;
    int j;
    
    for (int i = 1; i < last.length(); i++) {
        j = i;
        //compare ASCII values
        while(j > 0 && int(last[j-1]) >= int(last[j])) {
            if (int(last[j-1]) <= int(last[j])) {
                break;
            }
            //swap characters
            temp = last[j];
            last[j] = last[j-1];
            last[j-1] = temp;
            j--;
        }
    }
    
    return last;
}
/* l is for left index and r is right index of the
 sub-array of arr to be sorted */
void mergeSortDecode(char arr[], int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;
        
        // Sort first and second halves
        mergeSortDecode(arr, l, m);
        mergeSortDecode(arr, m+1, r);
        
        merge(arr, l, m, r);
    }
}
void merge(char arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
    
    /* create temp arrays */
    int L[n1], R[n2];
    
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
    
    /* Merge the temp arrays back into arr[l..r]*/
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

// generate a string of the 'next' indexes
int* nextIndex(string sorted, string last) {
    int* next = new int[sorted.length()];
    int index = 0;
    int count = 1; // count of how many letters in a row
    char temp;
    
    for (int i = 0; i < sorted.length(); i++) {
        temp = sorted[i];
        count = 1; // reset count variable
        // for loop to count how many letters in a row
        for (int k = (i+1); k < sorted.length(); k++) {
            if (temp == sorted[k]) {
                count++;
                i++;
            }
            else break;
        }
        // iterate thru 'last' and find matching pairs until count == 0
        for (int j = 0; j < last.length(); j++) {
            if (temp == last[j]) {
                next[index] = j;
                index++;
                count--;
            }
            if (count == 0) break;
        }
    }
    return next;
}
// output the finalized decoded string
string decode(string last, int* next, int orig_index) {
    string decoded = "";
    int index = next[orig_index];
    for (int i = 0; i < last.length(); i++) {
        decoded += last[index];
        index = next[index];
    }
    return decoded;
}
