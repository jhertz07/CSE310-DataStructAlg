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
#include <string>

void initStormEvent(int);

using namespace std;

class stormYear {
public:
    //open streams
    ifstream details;
    ifstream fatalities;
    //declare dyanmic arrays
    int* num_of_storms;
    annual_storms* year;
    
    stormYear(string detail_file, string fatality_file) {
        // open csv files for desired year
        details.open(detail_file);
        fatalities.open(fatality_file);
    }
    ~stormYear() {
        // close csv file
        details.close();
        fatalities.close();
        //delete dynamic arrays (dont think I have to do this)
//        delete[] num_of_storms;
//        delete [] year;
    }
};
    stormYear() s; // call constructor as a global variable
int main(int argc, const char * argv[]) {

    int num_of_years;
    // get num of years thru command line arg
    if ((num_of_years = atoi(argv[1])));
    else return 0;
    // initialize dynamic array to store years from command line args
    s.year = new annual_storms[num_of_years];
    s.num_of_storms = new int[num_of_years]; // store the size of each year index
    string line; int num_of_lines = 0; // used for getline in loop
    
    // initialize each year of storms
    for (int i = 0; i < num_of_years; i++) {
        s.year[i].year = atoi(argv[i+2]); // match year to argument input
        // find which file to read from
        num_of_lines = 0;
        if (s.year[i].year == 1950) {
            while(getline(s.details_1950, line)) {
                num_of_lines++;
            }
        }
        else if (s.year[i].year == 1951) {
            while(getline(s.details_1951, line)) {
                num_of_lines++;
            }
        }
        else if (s.year[i].year == 1952) {
            while(getline(s.details_1952, line)) {
                num_of_lines++;
            }
        }
        s.year[i].events = new storm_event[num_of_lines-1]; // make array size of number of storms
        s.num_of_storms[i] = num_of_lines-1; // save the array sizes for future
    }
    // initialize storm events for each year
    for (int i = 0; i < num_of_years; i++) {
        initStormEvent(i);
    }
}
// initialize storm events for given year
void initStormEvent(int index) {
    for (int i = 0; i < s.num_of_storms[index]; i++) {
       // s.year[i].events->event_id = getline(s.details)
    }
}
