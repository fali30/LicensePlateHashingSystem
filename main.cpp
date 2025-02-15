/*main.cpp*/

//
// Hashing program for specialized Illinois license plates,
// which processes an input file of license plates and fines.
// The output is the total fines per license plate, in 
// sorted order.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <cassert>

#include "ILplates.h"

using namespace std;


//
// hashInput:
//
void hashInput(string basename, ILplates& hashplates)
{
    string infilename = basename + ".txt";
    ifstream infile(infilename);
    
    if (!infile.good())
    {
        cout << endl;
        cout << "**Error: unable to open input file '" << infilename << "', exiting." << endl;
        cout << endl;
        exit(-1);
    }
    
    //
    // input the plates and fines:
    //
    cout << "Reading '" << infilename << "'..." << endl;
    
    string fine;
    string plate;
    
    getline(infile, fine);
    
    //
    // for each pair (fine, license plate), hash and store/update fine:
    //
    while (!infile.eof())
    {
        getline(infile, plate);
        
        //cout << fine << endl;
        //cout << plate << endl;
        
        //
        // is plate valid?  Only process valid plates:
        //
        if (hashplates.Hash(plate) >= 0)  // yes:
        {
            int amount = hashplates.Search(plate);
            
            if (amount < 0)  // not found:
            {
                hashplates.Insert(plate, stoi(fine));
            }
            else  // we found it, so update total in hash table:
            {
                amount += stoi(fine);
                hashplates.Insert(plate, amount);
            }
            
        }//valid
        
        getline(infile, fine);
    }
}


//
// simple O(N^2) sort:
//
void sortOutput( vector<string>& keys, vector<int>& values) {
    
    size_t size = keys.size();
    
    for ( size_t i = 0; i < size; ++i) {
        for ( size_t j = i; j < size; ++j) {
            if ( keys[j] < keys[i]) {
                string tempKey = keys[j];
                int tempValue = values[j];
                
                keys[j] = keys[i];
                keys[i] = tempKey;
                
                values[j] = values[i];
                values[i] = tempValue;
            }
        }
    }
}


int main()
{
    int    N;        // = 10000;
    string basename; // = "tickets1";
    
    cout << "Enter hashtable size> ";
    cin >> N;
    
    hashtable<string, int>  ht(N);
    ILplates                hashplates(ht);
    
    cout << "Enter base filename> ";
    cin >> basename;
    cout << endl;
    
    //
    // process input file of fines and license plates:
    //
    hashInput(basename, hashplates);
    
    //
    // debugging:
    //
    vector<string> plates = ht.Keys();
    vector<int> amounts = ht.Values();
    
    cout << "Sorting..." << endl;
    sortOutput( plates, amounts);
    
    cout << "Writing '" << basename << "-output.txt'..."<< endl;
    ofstream outfile(basename + "-output.txt");
    
    for (size_t i = 0; i < plates.size(); ++i)
    {
        outfile << "\"" << plates[i] << "\"" << " $" << amounts[i] << endl;
    }
    
    //
    // done:
    //
    return 0;
}
