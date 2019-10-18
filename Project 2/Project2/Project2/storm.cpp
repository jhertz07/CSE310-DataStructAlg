//
//  main.cpp
//  Project2
//
//  Created by Justin Hertzberg on 10/16/19.
//  Copyright © 2019 ASU. All rights reserved.
//

#include <iostream>
#include "defn.h"
#include <stdlib.h>
#include <fstream>
#include <string>

void init_storm(storm_event);

using namespace std;

class storm {
public:
    //open streams - remember to close streams (deconstructor?)
    ifstream details_1950;
    ifstream fatalities_1950;
    ifstream details_1951;
    ifstream fatalities_1951;
    ifstream details_1952;
    ifstream fatalities_1952;
    
    storm() {
    // open csv files
    details_1950.open("details-1950.csv");
    fatalities_1950.open("fatalities-1950.csv");
    details_1951.open("details-1951.csv");
    fatalities_1951.open("fatalities-1951.csv");
    details_1952.open("details-1952.csv");
    fatalities_1952.open("fatalities-1952.csv");
    }
};

int main(int argc, const char * argv[]) {
    storm s; // call constructor
    int num_of_years;
    // get num of years thru command line arg
    if ((num_of_years = atoi(argv[1])));
    else return 0;
    // initialize dynamic array to store years from command line args
    annual_storms *year = new annual_storms[num_of_years];
    string line; int num_of_lines = 0; // used for getline in loop
    
    // initialize each year of storms
    for (int i = 0; i < num_of_years; i++) {
        year[i].year = atoi(argv[i+2]); // match year to argument input
        // find which file to read from
        num_of_lines = 0;
        if (year[i].year == 1950) {
            while(getline(s.details_1950, line)) {
                num_of_lines++;
            }
        }
        else if (year[i].year == 1951) {
            while(getline(s.details_1951, line)) {
                num_of_lines++;
            }
        }
        else if (year[i].year == 1952) {
            while(getline(s.details_1952, line)) {
                num_of_lines++;
            }
        }
        year[i].events = new storm_event[num_of_lines-1];
    }
}
void init_storm(storm_event storm) {
    
}
