/*ILplates.cpp*/

//
// Hashing functions to store (license plate, amount) pairs
// using linear probing.
//
//

#include <iostream>
#include <string>

#include <math.h>

#include "ILplates.h"

using namespace std;


//
// Hash:
//
// Given a specialized Illinois license plate, returns an index into
// the underyling hash table.  If the given plate does not follow the
// formatting rules given below, -1 is returned.
//
// Personalized:
//   letters and numbers, with a space between the letters
//   and numbers.  Format: 1-5 letters plus 1..99 *OR*
//   6 letters plus 1..9
//
//   Examples: A 1, B 99, ZZZZZ 1, ABCDEF 3
//
// Vanity:
//   Format: 1-3 numbers *OR* 1-7 letters
//
//   Examples: 007, 1, 42, X, AAA, ZZZEFGH
//
int ILplates::Hash(string plate)
{
    long long index = 0;
    
    size_t plateSize = plate.size();
    size_t i;
    
    
    // split string into alphabets and digits
    for ( i = 0; i < plateSize; ++i) {
        if ( !isupper( plate[i])) {
            break;
        }
    }
    string alphabets = plate.substr( 0, i);
    string digits = ( i > 0) ? plate.erase( 0, i+1) : plate;
    
    size_t alphabetsSize = i;
    size_t digitsSize = digits.size();
    
    
    // check if plate is valid
    for ( i = 0; i < alphabetsSize; ++i) {
        if ( isdigit( alphabets[i]) || isspace( alphabets[i]))
            return -1;
    }
    
    for ( i = 0; i < digitsSize; ++i) {
        if ( isalpha( digits[i]) || isspace( digits[i]))
            return -1;
    }
    
    if ( !( alphabetsSize > 0 && alphabetsSize < 6 && digitsSize > 0 && digitsSize < 3) &&  // 1-5 letters plus 1..99
         !( alphabetsSize == 6 && digitsSize == 1) &&                                       // 6 letters plus 1..9
         !( digitsSize > 0 && digitsSize < 4 && alphabetsSize == 0) &&                      // 1-3 numbers
         !( digitsSize == 0 && alphabetsSize > 0 && alphabetsSize < 8)) {                   // 1-7 letters
        return -1;
    }
    
    
    // begin hashing
    vector<int> numbers;
    
    for ( i = 0; i < alphabetsSize; ++i)
        numbers.push_back( alphabets[i] - 'A' + 1);
    for ( i = 0; i < digitsSize; ++i)
        numbers.push_back( digits[i] - '0' + 'A' + 1);
    
    int power = 1;
    for ( int i = numbers.size() - 1; i >= 0; --i) {
        index += numbers[i] * pow( numbers[i], power);
        power++;
    }

    return index % HT.Size();
}


//
// Search
//
// Hashes and searches for the given license plate; returns the
// associated value for this plate if found, or -1 if not found.
//
int ILplates::Search(string plate)
{
    
    int index = Hash( plate);
    
    int size = HT.Size();
    
    bool isEmpty = false;
    string tempPlate = "";
    int tempValue = -1;
    
    int count = 0;
    while ( count < size) {
        
        if ( index >= size) {
            index = 0;
        }
        
        HT.Get( index, isEmpty, tempPlate, tempValue);
        
        if ( isEmpty) {
            return -1;
        }
        
        if ( plate == tempPlate) {
            //cout << "search index = " << index << endl;
            return tempValue;
        }
        
        count++;
        index = ( index + 1) % size;
        
        
    }
    
    
    return -1;
    
}


//
// Insert
//
// Inserts the given (plate, newValue) into the hash table,
// overwriting an existing value if there.
//
void ILplates::Insert(string plate, int newValue)
{
    
    int index = Hash( plate);
    int size = HT.Size();
    
    bool isEmpty = false;
    string tempPlate = "";
    int tempValue = -1;
    
    int count = 0;
    while ( count < size) {
        
        if ( index >= size) {
            index = 0;
        }
        
        HT.Get( index, isEmpty, tempPlate, tempValue);
        
        if ( isEmpty || plate == tempPlate) {
            //cout << "insert index = " << index << endl;
            HT.Set( index, plate, newValue);
            return;
        }
        
        count++;
        index = ( index + 1) % size;
        
    }
    
    cout << "Not enough room in hashtable for " << plate << endl;
    
}
