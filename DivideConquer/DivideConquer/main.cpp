//
//  main.cpp
//  DivideConquer
//
//  Created by Justin Hertzberg on 10/26/19.
//  Copyright © 2019 ASU. All rights reserved.
//

#include <iostream>
#include <math.h>

void mergeSort(int*, int, int);
void merge (int*, int, int, int);
int binarySearch(int*, int, int, int);
void inversionDivide(int*, int, int);
int inversionCount(int*, int, int);

int main(int argc, const char * argv[]) {
    
    int arr[6] = {4, 2, 1, 8, 5, 7};
    
    int left = 0; int right = 5;
//    int value = binarySearch(arr, left, right, 5);
//    std::cout << value << "\n";
    mergeSort(arr, left, right);
//
    for (int i = 0; i <= right; i++) {
        std::cout << arr[i] <<"\n";
    }
    
    return 0;
}
 // find number of inversions, ie if k<j and A[k]>A[j]
int inversionCount(int* arr, int l, int r) {
    int count = 0;
    int k = l+1;
   
    for (int i = k; i <= r; i++) {
        if (arr[i] < arr[l]) count++;
    }
    
    return count;
}
void inversionDivide(int arr[], int left, int right) {
    int count = 0;
    if (left < right) {
        inversionDivide(arr, left+1, right);
        
        count += inversionCount(arr, left, right);
    }
    std::cout << count;
}
// array of size n, find value == k
int binarySearch(int* arr, int l, int r, int k) {
    if (l==r) { // base case, there is only one element
        if (arr[l] == k) { // the one argument matches
            return l;
        }
        else {
            return -1; // k not found
        }
    }
    else {
        int mid = (int)floor((l+r)/2); // find mid
        if (arr[mid] == k) { // mid == k, return index mid
            return mid;
        }
        // else k is to the right of mid
        else if (arr[mid] < k) return binarySearch(arr, mid+1, r, k);
        // or else k is to the left of mid
        else return binarySearch(arr, l, mid-1, k);
    }
}
void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = (int)floor((left+right)/2);
        
        // call merge sort for each half recursively
        mergeSort(arr, left, mid);
        mergeSort(arr, mid+1, right);
        
        merge(arr, left, mid, right);
    }
}
void merge(int arr[], int l, int m, int r)
{
    // create new subarray with all elements that need to be merged
    int subarr[r-l+1];
    int k = 0; // use as iterator for subarray
    // starting position for 'subarrays within subarray'
    int p = l; int q = m+1; // p = left, q = right
    
    for (int i = l; i <= r; i++) {
        // check if subarray1 or subarray2 are exhausted
        if (p > m) { // if left iterator is past mid = exhausted
            subarr[k++] = arr[q++];
        }
        else if (q > r) { // if right iterator is past end index
            subarr[k++] = arr[p++];
        }
        else if (arr[p] < arr[q]) { // if left element is larger
            subarr[k++] = arr[p++];
        }
        else { // lastly it must be add right element
            subarr[k++] = arr[q++];
        }
    }
    // now that subarr has all elements sorted, add it into arr
    for (int i = 0; i < k; i++) {
        arr[l++] = subarr[i];
    }
}
