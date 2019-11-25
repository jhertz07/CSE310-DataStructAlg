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
#include <cmath>
#include "Graph.h"

void buildGraph(int, int);
void getMedians(); // and sum
bool* isEdge(int, int, int, int);
void addEdge(int, int, int, int, struct adjList[63][63]);
void degreeDistribution(struct adjList[63][63]);
void DFS(struct adjList[63][63]);
void DFSRecur(struct adjList[63][63], bool[63][63], int, int);

// global variables which will be used throughout the program
float cells[63][63][832];
const int week_size = 3969; // total cells in a week
const int land_nodes = 783; // total number of land nodes
const int total_weeks = 832; // total weeks*years measured
struct adjList graph950[63][63]; // adjacency list represnting entire graph
struct adjList graph925[63][63];
struct adjList graph900[63][63];
float median[63][63]; // median value for each cell
float sum[63][63]; // Sxx value for each cell
int comp_size;

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
            graph950[i][k].head = nullptr;
            graph925[i][k].head = nullptr;
            graph900[i][k].head = nullptr;
        }
    }
    // get medians for time efficiency
    getMedians();
    // build graph by comparing nodes, starting at (0,0)
    buildGraph(0, 0);
    // print degrees for given graph
    cout << "\nDegree Distribution for 0.95\n--------------------------\n";
    degreeDistribution(graph950);
    // print component sizes
    DFS(graph950);

    return 0;
}
// recursive algorithm to build graph by comparing nodes to each other (combinations)
void buildGraph(int x1, int y1) {
    cout << x1 << ", " << y1 << "\n"; // output x1,y1 for testing purposes
    int sx2, sy2;
    // base case, if end of nodes is reached
    if (x1 == 62 && y1 == 62) return;

    // set initial x2, y2 values to be the next cell after x1, y1
    if (x1 == 62) {
        sx2 = 0;
        sy2 = y1 + 1;
    }
    else {
        sx2 = x1 + 1;
        sy2 = y1;
    }
    // loop thru nodes starting at sx2, sy2
    int x2 = sx2; int y2 = sy2;
    do {
        // check if edge exists
        bool* edge = isEdge(x1, y1, x2, y2);
        // add edges depending on isEdge output
        if (edge[0]) addEdge(x1, y1, x2, y2, graph950);
        if (edge[1]) addEdge(x1, y1, x2, y2, graph925);
        if (edge[2]) addEdge(x1, y1, x2, y2, graph900);
        // increment x2, y2
        if (x2 == 62) {
            x2 = 0;
            y2++;
        }
        else x2++;
        
    }
    // break loop at (0,63), which is out of bounds (last node is (62, 62))
    while(y2 != 63);
    
    //increment x1, y1
    if (x1 == 62) {
        x1 = 0;
        y1++;
    }
    else x1++;
    //recursively call
    buildGraph(x1, y1);
}
// check whether two vertices, over a period of time (n), are over given threshold (.95, .925, or .90)
bool* isEdge(int x1, int y1, int x2, int y2) {
    // 0 = 0.95, 1 = 0.925, 2 = 0.9
    bool* edge = new bool[3];
    edge[0] = false; edge[1] = false; edge[2] = false;
    
    int size = total_weeks; // go thru all weeks
    float sum_1_2 = 0;
    
    // check if land cell, dont do calculation if so
    if (cells[x1][y1][0] == 168 || cells[x2][y2][0] == 168) return edge;
    
    // get median values
    float median_1 = median[x1][y1];
    float median_2 = median[x2][y2];
    // get sum values
    float sum_1 = sum[x1][y1];
    float sum_2 = sum[x2][y2];
    
    // sum up 1_2
    for (int i = 0; i < size; i++) {
        sum_1_2 += (cells[x1][y1][i] - median_1) * (cells[x2][y2][i] - median_2);
    }
    // calculate total using 'r' formula and compare to threshold
    float total = sum_1_2 / sqrt(sum_1 * sum_2);
    total = abs(total);
    
    // check total against each threshold (must be greater or equal)
    if (total >= 0.95) edge[0] = true;
    if (total >= 0.925) edge[1] = true;
    if (total >= 0.9) edge[2] = true;
    return edge;
}
// add edge to graph/adjacency list
void addEdge(int x1, int y1, int x2, int y2, struct adjList graph[63][63]) {
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
void getMedians() {
    float med, s;
    // iterate thru all cells
    for (int x = 0; x < 63; x++) {
        for (int y = 0; y < 63; y++) {
            med = 0; // reset median
            if (cells[x][y][0] == 168) med = -1; // check if land cell
            else for (int w = 0; w < total_weeks; w++) {
                med += cells[x][y][w];
            }
            if (med == -1);
            else med = med/total_weeks;
            median[x][y] = med;
        }
    }
    // now calculate sum
    for (int x = 0; x < 63; x++) {
        for (int y = 0; y < 63; y++) {
            s = 0; // reset sum
            if (cells[x][y][0] == 168) s = -1; // check if land cell
            else for (int w = 0; w < total_weeks; w++) {
                s += pow(cells[x][y][w] - median[x][y], 2);
            }
            sum[x][y] = s;
        }
    }
}
// print degree distribution for given graph
void degreeDistribution(struct adjList graph[63][63]) {
    // create array to store degrees and initialize to 0
    int degrees[3968];
    for (int i = 0; i < 3968; i++) {
        degrees[i] = 0;
    }
    adjNode* node = nullptr; int count;
    // iterate thru graph
    for (int x = 0; x < 63; x++) {
        for (int y = 0; y < 63; y++) {
            count = 0; // reset count
            node = graph[x][y].head; // assign node to head
            // iterate thru linked list and count number of edges
            while (node != nullptr) {
                count++;
                node = node->next;
            }
            // increment index of degrees
            degrees[count]++;
        }
    }
    // subtract land nodes from degree 0, as we don't want these included
    degrees[0] -= land_nodes;
    // find highest degree index
    int highest = 0;
    for (int i = 0; i < 3968; i++) {
        if (degrees[i] > 0) highest = i;
    }
    // print degrees from 0 to highest
    for (int i = 0; i <= highest; i++) {
        cout << "Degree " << i << ":  " << degrees[i] << "\n";
    }
}
// DFS function to set up and call on DFSRecur
void DFS(struct adjList graph[63][63]) {
    // keep track of visited nodes
    bool visited[63][63];
    // keep track of component sizes
    int components[3969];
    for (int i = 0; i < 3968; i++) { // set all to 0
        components[i] = 0;
    }
    // set visited to all false
    for (int x = 0; x < 63; x++) {
        for (int y = 0; y < 63; y++) {
            visited[x][y] = false;
        }
    }
    // iterate thru graph to catch all componenets
    for (int x = 0; x < 63; x++) {
        for (int y = 0; y < 63; y++) {
            comp_size = 0;
            // if visited == false and not a land node, perform DFS recur (new componenet)
            if (visited[x][y] == false && graph[x][y].head != nullptr) {
                DFSRecur(graph, visited, x, y);
                comp_size++;
            }
            if (visited[x][y] == false) {
                visited[x][y] = true;
                comp_size++;
            }
            components[comp_size]++;
        }
    }
    // subtract land nodes
    components[1] -= land_nodes;
    // print component sizes
    for (int i = 1; i < 3968; i++) {
        if (components[i] > 0) {
            cout << "Comp size: " << i << ":  " << components[i] << "\n";
        }
    }
}
// Recursive function that performs the DFS
void DFSRecur(struct adjList graph[63][63], bool visited[63][63], int x, int y) {
    // mark current node as visited and increment comp_size
    visited[x][y] = true;
    // set temp node to head
    struct adjNode* node = graph[x][y].head;
    // first check head
    if (node != nullptr) {
        if (visited[node->x][node->y] == false) {
            DFSRecur(graph, visited, node->x, node->y);
            comp_size++;
        }
        // then iterate thru linked list
        while(node->next != nullptr) {
            node = node->next;
            if (visited[node->x][node->y] == false) {
                DFSRecur(graph, visited, node->x, node->y);
                comp_size++;
            }
        }
    }
}
// maybe I need to go to the (x,y) coordinate in the graph? And then set the node the head of that? And keep doing that ? And then do node->next after?
