//
//  main.cpp
//  Project2
//
//  Created by Justin Hertzberg on 10/16/19.
// CAN RESTRUCTURE THIS TO WORK SEPARATELY FROM A MAIN.CPP FILE WHICH CALLS
// THIS CLASS FOR EACH YEAR. ie pass which year csv files to open by string
//

#include <iostream>
#include "defn.h"
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>

void initStormYear(int, int);
void initStormEvents(int);
void openStream(int);
void setHashSize(int);
bool testPrime(int);
void addToHash(int, int);

using namespace std;

class storm {
public:
    //open streams
    ifstream details;
    ifstream fatalities;
    //declare dyanmic arrays
    annual_storms* year;
    hash_table_entry** hash; // hash is an array of pointers to hash_table_entry
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
    // initialize events for each year of storms and hash
    for (int i = 0; i < num_of_years; i++) {
        initStormEvents(i); // intialize each storm event for i year
    }
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
        s.year[i_year].events->year = stoi(field);
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
    hash_i = s.year[year_i].events[storm_i].event_id % s.hash_size;
    // if the hash index is empty, allocate new pointer and set it
    if (s.hash[hash_i] == NULL) {
        s.hash[hash_i] = new hash_table_entry;
        s.hash[hash_i]->event_id = s.year[year_i].events[storm_i].event_id;
        s.hash[hash_i]->event_index = storm_i;
        s.hash[hash_i]->year = s.year[year_i].year;
        s.hash[hash_i]->next = NULL;
        cout << "test";
    }
    // else use separate chaining, create new pointer and assign it as head
    else {
        
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
    s.hash = new hash_table_entry*[s.hash_size](); // initalize with null pointers ()
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
    if (storm_year == 1950) {
        s.details.open("details-1950.csv");
        s.fatalities.open("fatalities-1950.csv");
    }
    else if (storm_year == 1951) {
        s.details.open("details-1951.csv");
        s.fatalities.open("fatalities-1951.csv");
    }
    else if (storm_year == 1952) {
        s.details.open("details-1952.csv");
        s.fatalities.open("fatalities-1952.csv");
    }
}
