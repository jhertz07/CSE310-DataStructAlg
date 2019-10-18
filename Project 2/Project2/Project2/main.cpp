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

void initialize_storm(storm_event);

using namespace std;

int main(int argc, const char * argv[]) {
    // open csv files
    ifstream details_1950;
    details_1950.open("details-1950.csv");
    ifstream fatalities_1950;
    fatalities_1950.open("fatalities-1950.csv");
    ifstream details_1951;
    details_1951.open("details-1951.csv");
    ifstream fatalities_1951;
    fatalities_1951.open("fatalities-1951.csv");
    ifstream details_1952;
    details_1952.open("details-1952.csv");
    ifstream fatalities_1952;
    fatalities_1952.open("fatalities-1952.csv");
    
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
            while(getline(details_1950, line)) {
                num_of_lines++;
            }
        }
        else if (year[i].year == 1951) {
            while(getline(details_1951, line)) {
                num_of_lines++;
            }
        }
        else if (year[i].year == 1952) {
            while(getline(details_1952, line)) {
                num_of_lines++;
            }
        }
    }
    cout << num_of_lines << "\n";
}

void initialize_storm(storm_event storm) {
    
}
