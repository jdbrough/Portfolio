// fibo_dp_bottomup.cpp
// Glenn G. Chappell
// 2025-10-24
//
// For CS 411 Fall 2025
// Computing Fibonacci Numbers
// Dynamic Programming (Bottom-Up)

#include <iostream>
using std::cout;
using std::endl;
using std::cin;
#include <cstdint>
using std::uint_fast64_t;
#include <vector>
using std::vector;
#include <cstddef>
using std::size_t;
#include <cassert>
// For assert


// Type bignum
// Type of values used for large-ish integer computation.
// Should be an unsigned integer type.
using bignum = uint_fast64_t;  // fastest unsigned w/ >= 64 bits


// fibo
// Given nonnegative n, returns F(n) (the nth Fibonacci number).
// F(0) = 0. F(1) = 1. For n >= 2, F(n) = F(n-2) + F(n-1).
// Pre:
//     n >= 0.
//     F(n) is a valid bignum value.
// For bignum 64-bit unsigned, preconditions hold if 0 <= n <= 93.
// May throw std::bad_alloc.
bignum fibo(int n)
{
    vector<bignum> fibotable;  // fibotable[i] will be Fibonacci #i

    for (int i = 0; i <= n; ++i)
    {
        assert(fibotable.size() == size_t(i));
        if (i <= 1)
            fibotable.push_back(bignum(i));  // fibotable[i]
        else
            fibotable.push_back(fibotable[i-1] + fibotable[i-2]);
    }

    assert(fibotable.size() == size_t(n+1));
    return fibotable[n];
}


// userPause
// Wait for user to press ENTER: read all chars through first newline.
void userPause()
{
    std::cout.flush();
    while (std::cin.get() != '\n') ;
}


// Main program
// Prints F(i) for i = 0, 1, 2, ... through the greatest Fibonacci
// number that a 64-bit unsigned integer can represent.
// Uses fibo.
int main()
{
    const int MAXINDEX = 93;  // Greatest i for which we compute F(i)

    // Print Fibonacci numbers
    cout << "Fibonacci Numbers" << "\n";
    cout << "\n";
    for (int i = 0; i <= MAXINDEX; ++i)
    {
        // Compute Fibonacci number
        auto ff = fibo(i);

        // Print it nicely
        cout << "F(" << i << ")";
        if (i < 10)
            cout << " ";  // For aligned output
        cout << " = " << ff << "\n";
    }
    cout << endl;

    // Wait for user
    cout << "Press ENTER to quit ";
    userPause();
}

