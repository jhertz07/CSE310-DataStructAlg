
#define Graph_h

using namespace std;

// make structs for graph adjacency list

// struct for each node in list
struct adjNode {
    int x;
    int y;
    struct adjNode *next;;
};
// pointer to head of list
struct adjList {
    struct adjNode *head;
};
