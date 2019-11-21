//
//  main.cpp
//  Heaps
//
//  Created by Justin Hertzberg on 10/27/19.
//  Copyright © 2019 ASU. All rights reserved.
//

#include <iostream>
#include <math.h>

void maxHeapify(int*, int, int);
void buildHeap();

int main(int argc, const char * argv[]) {
    int arr[6] = {1, 9, 4, 18, 5, 3};
    buildHeap();
    
    return 1;
}
void buildHeap(){
    int arr[6] = {1, 9, 4, 18, 5, 4};
    int n = 6;
    int index = (int)floor(n/2);
    
    while (index >= 0) {
        maxHeapify(arr, index, n);
        index -= 1;
    }
}
// i = index to max_heapify on, n = size of array
void maxHeapify(int* arr, int i, int n) {
    int left = (i*2)+1; int right = (i*2)+2; // left and right children
    int temp; int largest; // largest used to denote where to swap (if at all)
    
    // if left child exists and is larger than i, swap
    if (left <= n && arr[left] > arr[i]) {
        largest = left;
    }
    else largest = i;
    if (right <= n && arr[right] > arr[largest]) {
        largest = right;
    }
    if (largest != i) {
        // swap
        temp = arr[largest];
        arr[largest] = arr[i];
        arr[i] = temp;
        maxHeapify(arr, largest, n);
    }
}
