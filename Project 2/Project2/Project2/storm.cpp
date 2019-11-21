//
//  main.cpp
//  Project2
//
//  Created by Justin Hertzberg on 10/16/19.

#include <iostream>
#include "defn.h"
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <math.h>
#include <stdio.h>

void initStormYear(int, int);
void initStormEvents(int);
void openStream(int);
void setHashSize(int);
bool testPrime(int);
void addToHash(int, int);
void hashFatality(int);
void processQuery(int);
bool printEvent(int, int);
void findChainLength();
void buildMaxHeap(int, string);
void maxHeapify(int, int, string);
storm_event getMax(int, string, storm_event*);
bst* newNode(int, string);
bst* insertBST(bst*, int, string);
void inorder(bst*, string, string, int);
void printRange(bst*, int);
void deleteBST(bst*);
int inorderCount(bst*);
int heightBST(bst*);

class storm {
public:
    //open streams
    ifstream details;
    ifstream fatalities;
    //declare dyanmic arrays
    annual_storms* year;
    hash_table_entry** hash; // hash is an array of pointers to hash_table_entry
    storm_event* heap; // used for max-heap
    bst* bst; // used for binary search tree
    int chain_l[100]; // stores the number of chains in hash ie for index 0, number of indexes where l=0
    int hash_size;
    
};

storm s; // call constructor as a global variable

int main(int argc, const char * argv[]) {
    int num_of_years; // globally define number of years
    // get num of years thru command line arg
    if ((num_of_years = atoi(argv[1])));
    else return 0;
    // initialize dynamic array to store years from command line args
    s.year = new annual_storms[num_of_years];
    
    // initialize each year of storms
    for (int i = 0; i < num_of_years; i++) {
        initStormYear(atoi(argv[i+2]), i); // match year to arg input
    }
    // set up hash
    setHashSize(num_of_years);
    // initialize events for each year of storms and set up hash
    for (int i = 0; i < num_of_years; i++) {
        initStormEvents(i); // intialize each storm event for i year
    }
    // hash fatalities for each year
    for (int i = 0; i < num_of_years; i++) {
        hashFatality(i);
    }
    // Process Queries
    int qnum; string nonsense;
    cout << "Process Queries:\n";
    cin >> qnum;
    getline(cin, nonsense); // for some reason I need to clear cin
    for (int i = 0; i < qnum; i++) {
        processQuery(num_of_years);
    }
    findChainLength();
}

// initialize each year
void initStormYear(int year, int i) {
    s.year[i].year = year; // set year
    // used for getline in loop
    string line; int num_of_lines = 0;
    // open file for desired year
    openStream(s.year[i].year);
    // read number of lines from opened file
    while(getline(s.details, line)) {
        num_of_lines++;
    }
    s.year[i].events = new storm_event[num_of_lines-1]; // make array size of number of storms
    s.year[i].num_of_storms = num_of_lines-1; // save number of storms for given year
}
// initialize storm events for given year
void initStormEvents(int i_year) {
    string line; string field; istringstream stream; float f_dmg;
    openStream(s.year[i_year].year); //reset stream
    getline(s.details, line); // move past first line
    // iterate thru the number of lines in csv file
    for (int i = 0; i < s.year[i_year].num_of_storms; i++) {
        // take in entire line
        getline(s.details, line);
        stream.str(line);
        //take in each comma separated field in line
        getline(stream, field, ',');
        s.year[i_year].events[i].event_id = stoi(field);
        getline(stream, field, ',');
        strcpy(s.year[i_year].events[i].state, field.c_str());
        getline(stream, field, ',');
        s.year[i_year].events[i].year = stoi(field);
        getline(stream, field, ',');
        strcpy(s.year[i_year].events[i].month_name, field.c_str());
        getline(stream, field, ',');
        strcpy(s.year[i_year].events[i].event_type, field.c_str());
        getline(stream, field, ',');
        s.year[i_year].events[i].cz_type = field[0];
        getline(stream, field, ',');
        strcpy(s.year[i_year].events[i].cz_name, field.c_str());
        getline(stream, field, ',');
        s.year[i_year].events[i].injuries_direct = stoi(field);
        getline(stream, field, ',');
        s.year[i_year].events[i].injuries_indirect = stoi(field);
        getline(stream, field, ',');
        s.year[i_year].events[i].deaths_direct = stoi(field);
        getline(stream, field, ',');
        s.year[i_year].events[i].deaths_indirect = stoi(field);
        getline(stream, field, ',');
        if (field[field.length()-1] == 'K') { // if string ends in K process as so
            f_dmg = stof(field);
            f_dmg = f_dmg*1000;
            s.year[i_year].events[i].damage_property = int(f_dmg);
        }
        else { // otherwise process as M
            f_dmg = stof(field);
            f_dmg = f_dmg*1000000;
            s.year[i_year].events[i].damage_property = int(f_dmg);
        }
        getline(stream, field, ',');
        s.year[i_year].events[i].damage_crops = stoi(field);
        getline(stream, field, ',');
        strcpy(s.year[i_year].events[i].tor_f_scale, field.c_str());
        s.year[i_year].events[i].fatality_size = 0;
        stream.clear();
        // add storm event to hash
        addToHash(i_year, i);
    }
}
void addToHash(int year_i, int storm_i) {
    int hash_i; // index to place in hash table
    hash_table_entry *temp = NULL; // temp pointer for swapping
    hash_i = s.year[year_i].events[storm_i].event_id % s.hash_size;
    // if the hash index is empty, allocate new pointer and set it
    if (s.hash[hash_i] == NULL) {
        s.hash[hash_i] = new hash_table_entry;
        s.hash[hash_i]->event_id = s.year[year_i].events[storm_i].event_id;
        s.hash[hash_i]->event_index = storm_i;
        s.hash[hash_i]->year = s.year[year_i].year;
        s.hash[hash_i]->next = NULL;
        
    }
    // else use separate chaining, create new pointer and assign it as head
    else {
        temp = s.hash[hash_i]; // point temp towards old head
        s.hash[hash_i] = new hash_table_entry; // point to new memory
        s.hash[hash_i]->event_id = s.year[year_i].events[storm_i].event_id;
        s.hash[hash_i]->event_index = storm_i;
        s.hash[hash_i]->year = s.year[year_i].year;
        s.hash[hash_i]->next = temp; // link temp pointer
    }
    
}
void setHashSize (int num_of_years) {
    // declare variable for total storms across all years
    int total_storms = 0; int closest_prime;
    for (int i = 0; i < num_of_years; i++) {
        total_storms += s.year[i].num_of_storms;
    }
    closest_prime = 2 * total_storms;
    while (!testPrime(closest_prime)) {
        closest_prime++;
    }
    s.hash_size = closest_prime;
    s.hash = new hash_table_entry*[s.hash_size];
    for (int i = 0; i < s.hash_size; i++) { // set all to null
        s.hash[i] = NULL;
    }
}
void hashFatality(int year_i) {
    string line; string field; istringstream stream; int index; int e_id; int f_size = -1; // -1 to account for header line
    hash_table_entry *h_index; fatality_event *f_event; int f_id;
    openStream(s.year[year_i].year);
    // find number of lines in fatality csv;
    while (getline(s.fatalities, line)) {
        f_size++;
    }
    openStream(s.year[year_i].year); // reset stream
    getline(s.fatalities, line); // move past first line
    // iterate thru fatalities file to and match event id
    for (int i = 0; i < f_size; i++) {
        getline(s.fatalities, line);
        stream.str(line);
        // go to second field
        getline(stream, field, ',');
        f_id = stoi(field); // save for later
        getline(stream, field, ',');
        // find hash index
        e_id = stoi(field);
        index = e_id % s.hash_size;
        h_index = s.hash[index];
        // match event id with possible linked list in hash table
        while(h_index != NULL) {
            if (h_index->event_id == e_id) {
                index = h_index->event_index;
                break;
            }
            else h_index = h_index->next;
        }
        // if there is already a fatality event, replace as head
        if (s.year[year_i].events[index].f != NULL) {
            f_event = s.year[year_i].events[index].f;
            s.year[year_i].events[index].f = new fatality_event;
            s.year[year_i].events[index].f->next = f_event;
        }
        else {
            s.year[year_i].events[index].f = new fatality_event;
            s.year[year_i].events[index].f->next = NULL;
        }
        // iterate thru fatality file
        s.year[year_i].events[index].fatality_size += 1; // increment fatality size
        s.year[year_i].events[index].f->event_id = e_id;
        s.year[year_i].events[index].f->fatality_id = f_id;
        getline(stream, field, ',');
        if (field == "") s.year[year_i].events[index].f->fatality_type = '0';
        s.year[year_i].events[index].f->fatality_type = field[0];
        getline(stream, field, ',');
        strcpy(s.year[year_i].events[index].f->fatality_date, field.c_str());
        getline(stream, field, ',');
        if (field == "") s.year[year_i].events[index].f->fatality_age = 0;
        else s.year[year_i].events[index].f->fatality_age = stoi(field);
        getline(stream, field, ',');
        if (field == "" ) s.year[year_i].events[index].f->fatality_sex = '0';
        else s.year[year_i].events[index].f->fatality_sex = field[0];
        getline(stream, field, ',');
        strcpy(s.year[year_i].events[index].f->fatality_location, field.c_str());
        stream.clear();
    }
}
void processQuery(int num_of_years) {
    string query; string s_value; string digit; istringstream stream;
    getline(cin, query);
    stream.str(query);
    if (query.substr(0, 9) == "range all") {
        getline(stream, query, ' ');
        getline(stream, s_value);
    }
    else {
    for (int i = 0; i < query.size()-1; i++) {
        if (isdigit(query[i])) {
            digit = query[i]; // keep delimiter to append after
            getline(stream, query, query[i]);
            getline(stream, s_value);
            break;
        }
    }
        query.pop_back(); // remove last letter of string
        s_value = digit + s_value; // convert to desired format depending on query
    }
    stream.clear();
    bool found_id = false; bool found_fat = false;
    // QUERY: FIND EVENT
    if (query == "find event") {
        int value = stoi(s_value);
        int index = value % s.hash_size;
        hash_table_entry* hash = s.hash[index];
        // check through hash index for event id
        while (hash != NULL) {
            if (hash->event_id == value) {
                found_id = true;
                break;
            }
            else hash = hash->next;
        }
        if (found_id) found_fat = printEvent(hash->year, hash->event_index);
        // if event id was not found
        else if (!found_id) cout << "Storm event <" << value << "> not found.\n";
        // if no fatalities were found
        if (!found_fat) cout << "No fatalities\n";
    }
    // QUERY: FIND MAX
    else if (query == "find max" || query == "find max fatality") {
        stream.str(s_value); int num; string year; string dmg_type; string temp; int heap_size = 0;
        // get num
        getline(stream, temp, ' ');
        try { num = stoi(temp); }
        catch (const invalid_argument& e) { cout << "Invalid input\n"; return; }
        // create array to hold max values
        storm_event *max_events = new storm_event[num];
        // get year
        getline(stream, year, ' ');
        // get dmg_type
        getline (stream, dmg_type, ' ');
        //construct heap array based off years
        if (year == "all") { // create a heap array composed of all years
            // get heap size by adding all years together
            for (int i = 0; i < num_of_years; i++) {
                heap_size += s.year[i].num_of_storms;
            }
            // allocate heap array
            s.heap = new storm_event[heap_size];
            // add all storm events to heap array
            int heap_i = 0;
            for (int i = 0; i < num_of_years; i++) {
                for (int k = 0; k < s.year[i].num_of_storms; k++) {
                    s.heap[heap_i] = s.year[i].events[k];
                    heap_i++;
                }
            }
        }
        else { // if year != all, error check and iterate thru years in annualstorms array
            int iyear; bool year_found = false;
            try { iyear = stoi(year); }
            catch (const invalid_argument& e) { cout << "Invalid input\n"; return; }
            for (int i = 0; i < num_of_years; i++) {
                if (s.year[i].year == iyear) {
                    year_found = true;
                    heap_size = s.year[i].num_of_storms;
                    s.heap = new storm_event[heap_size];
                    for (int k = 0; k < s.year[i].num_of_storms; k++) {
                        s.heap[k] = s.year[i].events[k];
                    }
                    break;
                }
            }
            if (!year_found) {
                cout << "invalid year\n";
                return;
            }
        }
        if (query == "find max") {
            // error check dmg_type
            if ((dmg_type != "damage_property") && (dmg_type != "damage_crops")) {
                cout << "invalid input\n";
                delete[] s.heap;
                delete[] max_events;
                return;
            }
            // create max heap and store max values in array
            else {
                buildMaxHeap(heap_size, dmg_type);
                // create new variable for heap size
                int temp_size = heap_size-1;
                // get max 'num' amount of times
                for (int i = 0; i < num; i++) {
                    max_events[i] = getMax(temp_size, dmg_type, max_events);
                    temp_size--;
                }
            }
            // print out max events
            cout << "Max " << dmg_type << ":\n";
            for (int i = 0; i < num; i++) {
                cout << max_events[i].event_id << ", " << max_events[i].event_type << ", ";
                if (dmg_type == "damage_property") cout << max_events[i].damage_property << "\n";
                else cout << max_events[i].damage_crops << "\n";
            }
        }
        // find fatality
        else {
            buildMaxHeap(heap_size, "fatality");
            // create new variable for heap size
            int temp_size = heap_size-1;
            // get max 'num' amount of times
            for (int i = 0; i < num; i++) {
                max_events[i] = getMax(temp_size, "fatality", max_events);
                temp_size--;
            }
            // print out storm fatalities
            cout << "Max fatalities: \n";
            for (int i = 0; i < num; i++) {
                cout << "storm event " << max_events[i].event_id << ":\n";
                fatality_event* fat_event = max_events[i].f;
                // print out all fatality events in linked list
                while (fat_event != NULL) {
                    cout << "Fatality_ID: " << fat_event->fatality_id << "\n";
                    if (fat_event->fatality_type == '0') cout << "Fatality-type: Not Available\n";
                    else cout << "Fatality-type: " << fat_event->fatality_type << "\n";
                    if (fat_event->fatality_date[0] == '\0') cout << "Fatality-date: Not Available\n";
                    else cout << "Fatality-date: " << fat_event->fatality_date << "\n";
                    if (fat_event->fatality_age == 0) cout << "Fatality-age: Not Available\n";
                    else cout << "Fatality-age " << fat_event->fatality_age << "\n";
                    if (fat_event->fatality_sex == '0') cout << "Fatality-sex: Not Available\n";
                    else cout << "Fatality-sex " << fat_event->fatality_sex << "\n";
                    if (fat_event->fatality_location[0] == '\0') cout << "Fatality-location: Not Available\n";
                    else cout << "Fatality-location " << fat_event->fatality_location << "\n";
                    
                    // move to next pointer
                    fat_event = fat_event->next;
                }
                cout << "\n";
            }
        }
        // print characteristics
        cout << "Total number nodes in heap: " << heap_size << "\n";
        double max_height = floor(log(heap_size));
        double left_height = max_height - 1;
        double right_height = floor(log(heap_size-2)) - 1;
        cout << "max height: " << max_height;
        cout << " left height: " << left_height;
        cout << " right height: " << right_height << "\n";
        // delete dyanmic arrays
        delete[] s.heap;
        delete[] max_events;
    }
    // QUERY: RANGE
    else if (query == "range") {
        stream.str(s_value); string year, field_name, low, high; int i_year; bst* node = NULL;
        getline(stream, year, ' '); // get year value
        getline(stream, field_name, ' '); // get field name
        if (field_name != "state" && field_name != "month_name") {
            cout << "invalid field_name\n";
            return;
        }
        getline(stream, high, ' '); // get low
        getline(stream, low, ' '); // get high
        
        if (year == "all") {
            if (field_name == "state") {
                node = insertBST(node, s.year[0].events[0].event_id, s.year[0].events[0].state);
            }
            else {
                node = insertBST(node, s.year[0].events[0].event_id, s.year[0].events[0].month_name);
            }
            for (int i = 0; i < num_of_years; i++) {
                for (int k = 0; k < s.year[i].num_of_storms; k++) {
                    if ((i==0) && (k==0)) {} // do nothing on first
                    else if (field_name == "state") {
                        insertBST(node, s.year[i].events[k].event_id, s.year[i].events[k].state);
                    }
                    else {
                        insertBST(node, s.year[i].events[k].event_id, s.year[i].events[k].month_name);
                    }
                }
            }
        }
        else { // input besides 'all'
            bool year_found = false;
            try {i_year = stoi(year);}
            catch (const invalid_argument& e) { cout << "Invalid input\n"; return; }
            // loop to find matching year
            for (int i = 0; i < num_of_years; i++) {
                if (s.year[i].year == i_year) {
                    year_found = true;
                    if (field_name == "state") {
                        node = insertBST(node, s.year[i].events[0].event_id, s.year[i].events[0].state);
                        for (int k = 1; k < s.year[i].num_of_storms; k++) {
                            insertBST(node, s.year[i].events[k].event_id, s.year[i].events[k].state);
                        }
                    }
                    else {
                        node = insertBST(node, s.year[i].events[0].event_id, s.year[i].events[0].month_name);
                        for (int k = 1; k < s.year[i].num_of_storms; k++) {
                            insertBST(node, s.year[i].events[k].event_id, s.year[i].events[k].month_name);
                        }
                    }
                    break;
                }
            }
            if (!year_found) {
                cout << "invalid year\n";
                return;
            }
        }
        inorder(node, low, high, num_of_years);
        int node_count = inorderCount(node);
        cout << "Total number of nodes in BST: " << node_count << "\n";
        int l_height = heightBST(node->left);
        int r_height = heightBST(node->right);
        int m_height = heightBST(node);
        cout << "left height: " << l_height;
        cout << " right height: " << r_height;
        cout << " max height: " << m_height << "\n";
        deleteBST(node);
    }
    // default case / invalid query
    else {
        cout << "Invalid Query\n";
    }
}
// max heap for 'find max' query
void buildMaxHeap(int n, string dmg_type) {
    // index of last non-leaf node
    int start_i = (n/2)-1;
    // traverse backwards through array and max-heapify each node
    for (int i = start_i; i >= 0; i--) {
        maxHeapify(n, i, dmg_type);
    }
}
void maxHeapify(int n, int i, string dmg_type) {
    int largest = i;
    int l = 2*i+1; // left child
    int r = 2*i+2; // right child
    
    if (dmg_type == "damage_property") {
        if ((l < n) && (s.heap[l].damage_property > s.heap[largest].damage_property)) {
            largest = l;
        }
        if ((r < n) && (s.heap[r].damage_property > s.heap[largest].damage_property)) {
            largest = r;
        }
        // swap and call recursively
        if (largest != i) {
            storm_event temp = s.heap[largest];
            s.heap[largest] = s.heap[i];
            s.heap[i] = temp;
            
            maxHeapify(n, largest, dmg_type);
        }
    }
    else if (dmg_type == "damage_crops") {
        if ((l < n) && (s.heap[l].damage_crops > s.heap[largest].damage_crops)) {
            largest = l;
        }
        if ((r < n) && (s.heap[r].damage_crops > s.heap[largest].damage_crops)) {
            largest = r;
        }
        // swap and call recursively
        if (largest != i) {
            storm_event temp = s.heap[largest];
            s.heap[largest] = s.heap[i];
            s.heap[i] = temp;
            
            maxHeapify(n, largest, dmg_type);
        }
    }
    else if (dmg_type == "fatality") {
        if ((l < n) && (s.heap[l].fatality_size > s.heap[largest].fatality_size)) {
            largest = l;
        }
        if ((r < n) && (s.heap[r].fatality_size > s.heap[largest].fatality_size)) {
            largest = r;
        }
        // swap and call recursively
        if (largest != i) {
            storm_event temp = s.heap[largest];
            s.heap[largest] = s.heap[i];
            s.heap[i] = temp;
            
            maxHeapify(n, largest, dmg_type);
        }
    }
}
storm_event getMax(int n, string dmg_type, storm_event *max_events) {
    // store root and return at end
    storm_event max = s.heap[0];
    //get last element and swap with max
    storm_event last = s.heap[n];
    s.heap[0] = last;
    // decrement n
    n--;
    // call max heapify
    maxHeapify(n, 0, dmg_type);
    return max;
}
// create new node and return
bst* newNode(int event_id, string field_name) {
    bst* node = new bst;
    node->event_id = event_id;
    node->field = field_name;
    node->left = NULL;
    node->right = NULL;
    return node;
}
// create and insert into BST
// remember to delete after use
bst* insertBST(bst* node, int event_id, string field_name) {
    // create new node once at proper position
    if (node == NULL) return newNode(event_id, field_name);
    // otherwise recur down tree
    int compare = field_name.compare(node->field);
    // if equal, use event id
    if (compare == 0) {
        if (event_id > node->event_id) compare = -1;
        else compare = 1;
    }
    // if node->field is higher compare == pos, if field_name is higher compare == neg
    // insert left
    if (compare > 0) {
        node->left = insertBST(node->left, event_id, field_name);
    }
    // insert right
    else if (compare < 0) {
        node->right = insertBST(node->right, event_id, field_name);
    }
    // if this hits the program isn't working right
    else {
        cout << "Something went terribly wrong\n";
    }
    //return the unchanged node pointer
    return node;
}
// inorder traversal of array
void inorder(bst* root, string low, string high, int num_of_years) {
    if (root!=NULL) {
        inorder(root->left, low, high, num_of_years);
        // check if within range and print
        if ((low.compare(root->field) >= 0) && (high.compare(root->field) <= 0)) {
            printRange(root, num_of_years);
        }
        inorder(root->right, low, high, num_of_years);
    }
}
int inorderCount(bst* root) {
    int count = 1;
    if (root->left != NULL) {
        count += inorderCount(root->left);
    }
    if (root->right != NULL) {
        count += inorderCount(root->right);
    }
    return count;
}
void printRange(bst* node, int num_of_years) {
    // print field name
    cout << node->field << ", ";
    // find event by hashing
    int e_id = node->event_id; bool found_id = false;
    int index = e_id % s.hash_size;
    hash_table_entry* hash = s.hash[index];
    // check through hash index for event id
    while (hash != NULL) {
        if (hash->event_id == e_id) {
            found_id = true;
            break;
        }
        else hash = hash->next;
    }
    if (!found_id) cout << "something went terribly wrong\n";
    else {
        int i_year = -1;
    // get proper storm event
        for (int i = 0; i < num_of_years; i++) {
            if (s.year[i].year == hash->year) {
                i_year = i;
                break;
            }
        }
        if (i_year==-1) {
            cout << "something went terribly wrong\n";
            return;
        }
    storm_event event = s.year[i_year].events[hash->event_index];
    // output from storm event
    cout << event.event_id << ", " << event.year << ", " << event.event_type << ", " << event.cz_type << ", " << event.cz_name << "\n";
    }
}
int heightBST(bst* root) {
    if (root == NULL) {
        return 0;
    }
    else {
        int lDepth = heightBST(root->left);
        int rDepth = heightBST(root->right);
        
        if (lDepth > rDepth) return (lDepth+1);
        else return (rDepth+1);
    }
}
void deleteBST(bst* node) {
    if (node==NULL) return;
    
    deleteBST(node->left);
    deleteBST(node->right);
    delete node;
}
bool printEvent(int year, int storm_i) {
    int year_i = 0;
    // search for correct year index
    for (int i = 0; i < s.year->num_of_storms; i++) {
        if (s.year[i].year == year) {
            year_i = i;
            break;
        }
    }
    // begin printing storm info
    cout << "STORM INFO:\n";
    cout << "Event_ID: " << s.year[year_i].events[storm_i].event_id << "\n";
    cout << "State: " << s.year[year_i].events[storm_i].state << "\n";
    cout << "Year: " << s.year[year_i].events[storm_i].year << "\n";
    cout << "Month: " << s.year[year_i].events[storm_i].month_name << "\n";
    cout << "Event-type: " << s.year[year_i].events[storm_i].event_type << "\n";
    cout << "CZ-Type: " << s.year[year_i].events[storm_i].cz_type << "\n";
    cout << "CZ-name: " << s.year[year_i].events[storm_i].cz_name << "\n";
    cout << "Injuries-direct: " << s.year[year_i].events[storm_i].injuries_direct << "\n";
    cout << "Injuries-indirect: " << s.year[year_i].events[storm_i].injuries_indirect << "\n";
    cout << "Deaths-direct: " << s.year[year_i].events[storm_i].deaths_direct << "\n";
    cout << "Deaths-indirect: " << s.year[year_i].events[storm_i].deaths_indirect << "\n";
    cout << "Damage-property: " << s.year[year_i].events[storm_i].damage_property << "\n";
    cout << "Damage-crops: " << s.year[year_i].events[storm_i].damage_crops << "\n";
    cout << "TOR F-scale: " << s.year[year_i].events[storm_i].tor_f_scale << "\n";
    // find fatality
    bool found_fat = false;
    fatality_event* fat_event = s.year[year_i].events[storm_i].f;
    // print out all fatality events in linked list
    while (fat_event != NULL) {
        found_fat = true;
        cout << "Fatality_ID: " << fat_event->fatality_id << "\n";
        if (fat_event->fatality_type == '0') cout << "Fatality-type: Not Available\n";
        else cout << "Fatality-type: " << fat_event->fatality_type << "\n";
        if (fat_event->fatality_date[0] == '\0') cout << "Fatality-date: Not Available\n";
        else cout << "Fatality-date: " << fat_event->fatality_date << "\n";
        if (fat_event->fatality_age == 0) cout << "Fatality-age: Not Available\n";
        else cout << "Fatality-age " << fat_event->fatality_age << "\n";
        if (fat_event->fatality_sex == '0') cout << "Fatality-sex: Not Available\n";
        else cout << "Fatality-sex " << fat_event->fatality_sex << "\n";
        if (fat_event->fatality_location[0] == '\0') cout << "Fatality-location: Not Available\n";
        else cout << "Fatality-location " << fat_event->fatality_location << "\n";
        
        // move to next pointer
        fat_event = fat_event->next;
    }
    
    return found_fat;
}
void findChainLength() {
    // initialize chain array to -1
    for (int i = 0; i < 100; i++) {
        s.chain_l[i] = -1;
    }
    int size = 0; hash_table_entry* hash;
    for (int i = 0; i < s.hash_size; i++) {
        size = 0;
        hash = s.hash[i];
        while (hash != NULL) {
            size++;
            hash = hash->next;
        }
        if (s.chain_l[size] == -1) s.chain_l[size] = 1;
        else s.chain_l[size] = s.chain_l[size] + 1;
    }
    cout << "Chain length: \n";
    int chain_length = -1;
    for (int i = 0; i < 100; i++) {
        if (s.chain_l[i] > -1) {
            cout << i << ": " << s.chain_l[i] << "\n";
            chain_length++;
        }
    }
    float l_factor = 0;
    for (int i = 1; i <= chain_length; i++) {
        l_factor += (s.chain_l[i] * i);
    }
    l_factor = l_factor / (float)(s.hash_size);
    //l_factor = (s.hash_size - s.chain_l[0]) / (float)(s.hash_size);
    cout << "Load factor: " << l_factor << "\n";
}
// test if inputted number is prime
bool testPrime( int val )
{
    int limit, factor = 2;
    
    limit = (long)( sqrtf( (float) val ) + 0.5f );
    while( (factor <= limit) && (val % factor) )
        factor++;
    
    return( factor > limit );
}
void openStream(int storm_year) {
    // close streams first
    s.details.close();
    s.fatalities.close();
    // open stream for desired year
    s.details.open("details-"+to_string(storm_year)+".csv");
    s.fatalities.open("fatalities-"+to_string(storm_year)+".csv");
}
