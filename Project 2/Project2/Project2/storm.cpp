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
#include <math.h>
#include <stdio.h>
#include <cstring>

void initStormYear(int, int);
void initStormEvents(int);
void openStream(int);
void setHashSize(int);
bool testPrime(int);
void addToHash(int, int);
void hashFatality(int);
void processQuery();
bool printEvent(int, int);
void findChainLength();

using namespace std;

class storm {
public:
    //open streams
    ifstream details;
    ifstream fatalities;
    //declare dyanmic arrays
    annual_storms* year;
    hash_table_entry** hash; // hash is an array of pointers to hash_table_entry
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
        processQuery();
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
void processQuery() {
    string query; string s_value; string digit; int value; istringstream stream;
    getline(cin, query);
    stream.str(query);
    for (int i = 0; i < query.size()-1; i++) {
        if (isdigit(query[i])) {
            digit = query[i]; // keep delimiter to append after
            getline(stream, query, query[i]);
            getline(stream, s_value);
            break;
        }
    }
    query.pop_back(); // remove last letter of string
    value = stoi(digit+s_value); // event id in this case
    int index = value % s.hash_size;
    hash_table_entry* hash = s.hash[index];
    bool found_id = false; bool found_fat = false;
    // find event query
    if (query == "find event") {
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
    for (int i = 0; i < 100; i++) {
        if (s.chain_l[i] > -1) {
            cout << i << ": " << s.chain_l[i] << "\n";
        }
    }
    float l_factor;
    l_factor = (s.hash_size - s.chain_l[0]) / (float)(s.hash_size);
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
