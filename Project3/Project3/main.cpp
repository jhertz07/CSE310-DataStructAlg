//
//  main.cpp
//  Project3
//
//  Created by Justin Hertzberg on 11/12/19.
//  Copyright © 2019 ASU. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include "Graph.h"

void buildGraph(int, int);
bool isEdge(int, int, int, int, float);
void addEdge(int, int, int, int);

// global variables which will be used throughout the program
float cells[63][63][832];
const int week_size = 3969; // total cells in a week
const int land_nodes = 783; // total number of land nodes
const int total_weeks = 832; // total weeks*years measured
struct adjList graph[63][63]; // adjacency list represnting entire graph
struct adjList compared[63][63]; //adjacency list keeping track of nodes that have been compared

int main(int argc, const char * argv[]) {
    
    // 1D array as a list of all values
    float numList[3969];
    
    // read in bit file
    int week = 1; int year = 1990; string file;
    ifstream inputFile;
    // loop through all weeks
    for (int w = 0; w < total_weeks; w++) {
        // open next file
        // check if single digit so a leading zero can be added
        if (week < 10) file = to_string(year)+"/Beaufort_Sea_diffw0"+to_string(week)+"y"+to_string(year)+"+landmask";
        else file = to_string(year)+"/Beaufort_Sea_diffw"+to_string(week)+"y"+to_string(year)+"+landmask";
        inputFile.open( file, ios::in | ios::binary );
        
        float dataIn = 0;
        for (int i = 0; i <= week_size; i++) {
            inputFile.read( (char*)&dataIn, 4 ); // Read 4 bytes of data
            numList[i] = dataIn;
        }
        inputFile.close();
        // now put 1D array into a 2D 63x63 array
        int index = 0; // used for iterating thru numList
        for (int i = 0; i < 63; i++) {
            for (int k = 0; k < 63; k++) {
                cells[i][k][w] = numList[index];
                index++;
            }
        }
        // iterate week from 1-52
        if (week == 52) {
            week = 1;
            year++;
        }
        else week++;
    }
    
    // set graph to all null
    for (int i = 0; i < 63; i++) {
        for (int k = 0; k < 63; k++) {
            graph[i][k].head = nullptr;
            compared[i][k].head = nullptr;
        }
    }
    // compare nodes starting at (0,0);
    buildGraph(0, 0);
//    // check for how many edges there are (testing purposes)
//    int edgeNum = 0;
//    for (int x1 = 0; x1 < 63; x1++) {
//        cout << "x1: " << x1 << "\n";
//        for (int y1 = 0; y1 < 63; y1++) {
//            cout << "y1: " << y1 << "\n";
//            for (int x2 = 0; x2 < 63; x2++) {
//                cout << "x2: " << x2 << "\n";
//                for (int y2 = 0; y2 < 63; y2++) {
//                    cout << "y2: " << y2 << "\n";
//                    // first check if nodes have already been compared
//                    if (hasCompared(x1, y1, x2, y2));
//                    // if not compared, addCompared and check if edge exists
//                    else {
//                        addCompared(x1, y1, x2, y2);
//                        if (isEdge(x1, y1, x2, y2, 0.95)) {
//                            addEdge(x1, y1, x2, y2);
//                            edgeNum++;
//                        }
//                    }
//                }
//            }
//        }
//    }
    cout << edgeNum;
    return 0;
}
// check whether two vertices, over a period of time (n), are over given threshold (.95, .925, or .90)
bool isEdge(int x1, int y1, int x2, int y2, float threshold) {
    int size = total_weeks; // go thru all weeks
    float median_1 = 0; float median_2 = 0;
    float sum_1 = 0; float sum_2 = 0; float sum_1_2 = 0;
    
    // check if land cell, dont do calculation if so
    if (cells[x1][y1][0] == 168 || cells[x2][y2][0] == 168) return false;
    
    // subtract const land_nodes from median
    // iterate thru data sets to find median
    for (int i = 0; i < size; i++) {
        median_1 += cells[x1][y1][i];
        median_2 += cells[x2][y2][i];
    }
    // divide sum by size to get median
    median_1 = median_1/size;
    median_2 = median_2/size;
    
    // sum up 1, 2, and 1_2
    for (int i = 0; i < size; i++) {
        sum_1 += pow((cells[x1][y1][i] - median_1), 2);
        sum_2 += pow((cells[x2][y2][i] - median_2), 2);
        sum_1_2 += (cells[x1][y1][i] - median_1) * (cells[x2][y2][i] - median_2);
    }
    // calculate total using 'r' formula and compare to threshold
    float total = sum_1_2 / sqrt(sum_1 * sum_2);
    total = abs(total);
    if (total >= threshold) { // must be greater than threshold
        return true;
    }
    else return false;
}
// add edge to graph/adjacency list
void addEdge(int x1, int y1, int x2, int y2) {
    // add edge from 1->2
    struct adjNode* temp_1 = new struct adjNode; // create new memory for node
    temp_1->x = x2;
    temp_1->y = y2;
    // if no other nodes exist at location
    if (graph[x1][y1].head == nullptr) {
        temp_1->next = nullptr;
        graph[x1][y1].head = temp_1;
    }
    // otherwise replace as new head
    else {
        temp_1->next = graph[x1][y1].head; // point next towards old head
        graph[x1][y1].head = temp_1; // replace as new head
    }
    // add edge from 2->1, repeat process as above with values in reverse
    struct adjNode* temp_2 = new struct adjNode;
    temp_2->x = x1;
    temp_2->y = y1;
    if (graph[x2][y2].head == nullptr) {
        temp_2->next = nullptr;
        graph[x2][y2].head = temp_2;
    }
    else {
        temp_2->next = graph[x2][y2].head;
        graph[x2][y2].head = temp_2;
    }
}
//void addCompared(int x1, int y1, int x2, int y2) {
//    // create new node with x2, y2 values
//    struct adjNode *temp = new struct adjNode;
//    temp->x = x2;
//    temp->y = y2;
//    // if no head, make head
//    if (compared[x1][y1].head == nullptr) {
//        temp->next = nullptr;
//        compared[x1][y1].head = temp;
//    }
//    // otherwise replace as new head
//    else {
//        temp->next = compared[x1][y1].head;
//        compared[x1][y1].head = temp;
//    }
//}
//bool hasCompared(int x1, int y1, int x2, int y2) {
//    struct adjNode *temp = compared[x2][y2].head; // create temp pointer
//    // iterate thru list to see if node exists
//    while (temp != nullptr) {
//        // if a match is found return true
//        if ((temp->x == x1) && (temp->y == y1)) return true;
//        temp = temp->next;
//    }
//    // if hit nullptr, has not been compared yet
//    return false;
//}
