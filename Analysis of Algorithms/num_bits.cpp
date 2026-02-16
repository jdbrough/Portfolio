// num_bits.cpp
// Glenn G. Chappell
// 2025-09-05
//
// For CS 411 Fall 2025
// Count Bits in Binary Representation

#include <iostream>
using std::cout;
using std::endl;
using std::cin;
#include <cstdlib>
using std::exit;
#include <string>
using std::string;
using std::getline;
#include <sstream>
using std::istringstream;


// numBits
// Count number of bits in binary representation of a positive integer.
// Based on algorithm BinRec from Levitin p. 75.
// Recursive.
// Pre:
//     n >= 1.
// Does not throw.
int numBits(int n)
{
    // BASE CASE
    if (n == 1)
        return 1;

    // RECURSIVE CASE
    return numBits(n/2) + 1;  // Integer division
}


// getNum
// Prints prompt to cout and then inputs a number of type int on a line
// from cin. Repeats until valid number obtained; returns it to caller
// in reference argument. Return is false if number could not be
// obtained.
bool getNum(const string & prompt,
            int & num)
{
    while (true)
    {
        cout << prompt;
        string line;
        getline(cin, line);
        if (!cin)
        {
            if (cin.eof())
                return false;
            continue;
        }

        istringstream istr(line);
        istr >> num;
        if (!istr)
            continue;
        return true;
    }
}


// Main program
// Repeatedly inputs a number n and prints the number of bits in the
// binary representation of n. Terminates on n < 0.
// Uses getNum, numBits.
int main()
{
    // Print header
    cout << "Counting Bits" << endl;
    cout << endl;

    while (true)
    {
        // Prompt & input n
        int n;
        bool good = getNum("n (0 or negative to quit)? ", n);
        if (!good)
            exit(1);

        if (n <= 0)
            exit(0);

        // Print number and how many bits are in it
        cout << endl;
        int bits = numBits(n);
        cout << n << " has " << bits
             << (bits == 1 ? " bit" : " bits") << endl;
        cout << endl;
    }
}

