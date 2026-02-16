// factorial_recurse.cpp
// Glenn G. Chappell
// 2025-09-05
//
// For CS 411 Fall 2025
// Recursive Computation of a Factorial

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
#include <cstdint>
using std::uint_fast64_t;


// Type unum
// Type of values used for large-ish integer computation.
// Should be an unsigned integer type.
using unum = uint_fast64_t;  // >= 64 bits, fast, unsigned


// factorial
// Recursive.
// Pre:
//     n >= 0.
//     n! is a value unum value.
// For 64-bit unsigned, preconditions hold if 0 <= n <= 20.
// Does not throw.
unum factorial(int n)
{
    // BASE CASE
    if (n == 0)
        return static_cast<unum>(1);

    // RECURSIVE CASE
    return static_cast<unum>(n) * factorial(n-1);
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
// Repeatedly inputs a number n and prints n! (factorial).
// Terminates on n < 0.
// Uses getNum, factorial.
int main()
{
    // Print header
    cout << "Factorials" << endl;
    cout << endl;

    while (true)
    {
        // Prompt & input n
        int n;
        while (true)
        {
            bool good = getNum("n (negative to quit)? ", n);
            if (!good)
                exit(1);

            if (n < 0)
                exit(0);

            if (n > 20)
            {
                cout << "TOO LARGE!" << endl;
                cout << endl;
                continue;
            }

            break;
        }

        // Print number and its factorial
        cout << endl;
        unum fact = factorial(n);
        cout << n << "! = " << fact << endl;
        cout << endl;
    }
}

