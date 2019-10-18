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

void initStormYear(int, int);
void initStormEvents(int);
void openStream(int);

using namespace std;

class storm {
public:
    //open streams
    ifstream details;
    ifstream fatalities;
    //declare dyanmic arrays
    annual_storms* year;
    
};

storm s; // call constructor as a global variable
int main(int argc, const char * argv[]) {
    int num_of_years;
    // get num of years thru command line arg
    if ((num_of_years = atoi(argv[1])));
    else return 0;
    // initialize dynamic array to store years from command line args
    s.year = new annual_storms[num_of_years];
    
    // initialize each year of storms
    for (int i = 0; i < num_of_years; i++) {
        initStormYear(atoi(argv[i+2]), i); // match year to arg input
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
    initStormEvents(i); // intialize each storm event
}
// initialize storm events for given year
void initStormEvents(int i_year) {
    string line; string field; istringstream stream;
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
        s.year[i_year].events[i].damage_property = stoi(field);
        getline(stream, field, ',');
        s.year[i_year].events[i].damage_crops = stoi(field);
        getline(stream, field, ',');
        strcpy(s.year[i_year].events[i].tor_f_scale, field.c_str());
        stream.clear();
    }
    
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
