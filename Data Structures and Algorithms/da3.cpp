// da3.cpp  
// Jonathan Brough
// 2024-09-19
// Updated: 2024-09-26
//
// For CS 311 Fall 2024
// Source for Assignment 3 functions

#include "da3.hpp"     // For Assignment 3 prototypes & templates

#include <functional>
using std::function;
#include <exception>
using std::exception;


// Exercise B — Did It Throw
void didItThrow(const function<void()> & ff,
                bool & threw)
{
    try
    {
        ff();
        threw = false;
    }
    catch (...)
    {
        threw = true;
        throw; // Rethrow the caught exception
    }
}


// Exercise D — Recursive GCD
int gcd(int a, int b)
{
    if (a == 0)
    {
        return b;
    }
    else if (b == 0)
    {
        return a;
    }
    else if (a > b)
    {
        return gcd(a % b, b);
    }
    else
    {
        return gcd(b % a, a);
    }
}