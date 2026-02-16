// knapsack.cpp
// Glenn G. Chappell
// 2025-10-29
//
// For CS 411 Fall 2025
// Knapsack Problem

#include <iostream>
using std::cout;
using std::cin;
#include <string>
using std::string;
using std::getline;
#include <cstdlib>
using std::size_t;
#include <vector>
using std::vector;
#include <algorithm>
using std::max;
#include <cassert>
// for assert


// knapsack_bf_recurse
// Given lists of object weights & values, knapsack capacity, and n,
// returns greatest possible value in knapsack problem using only
// objects from first n.
// Brute-force method.
// Recursive helper for function knapsack_bf.
int knapsack_bf_recurse(const vector<int> & weights,
                        const vector<int> & values,
                        int capacity,
                        size_t n)
{
    assert(weights.size() == values.size());
    assert(n <= weights.size());
    int result;  // For our final result

    if (n == 0)  // BASE CASE
    {
        result = 0;
    }
    else         // RECURSIVE CASE
    {
        int max_without_last =
            knapsack_bf_recurse(weights, values,
                                capacity, n-1);
        if (weights[n-1] > capacity)
            result = max_without_last;
        else
        {
            int max_with_last = values[n-1] +
                knapsack_bf_recurse(weights, values,
                                    capacity-weights[n-1], n-1);
            result = max(max_with_last, max_without_last);
        }
    }

    return result;
}


// knapsack_bf
// Given lists of object weights & values, and knapsack capacity,
// returns greatest possible value in knapsack problem.
// Brute-force method.
// Calls recursive helper function knapsack_bf_recurse.
int knapsack_bf(const vector<int> & weights,
                const vector<int> & values,
                int capacity)
{
    assert(weights.size() == values.size());
    size_t n = weights.size();

    return knapsack_bf_recurse(weights, values,
                               capacity, n);
}


// Marker for unknown values in array
const int UNKNOWN = -1;


// knapsack_mem_recurse
// Given lists of object weights & values, knapsack capacity, and n,
// returns greatest possible value in knapsack problem using only
// objects from first n.
// Recursive helper for function knapsack_mem.
// Values stored in vector knapsacktable, initialized to all UNKNOWN.
// Value for capacity, n is knapsacktable[capacity*dim+n].
int knapsack_mem_recurse(const vector<int> & weights,
                         const vector<int> & values,
                         int capacity,
                         size_t n,
                         vector<int> & knapsacktable,
                         size_t dim)
{
    assert(weights.size() == values.size());
    assert(n <= weights.size());
    int result;  // For our final result

    // Check if we have already computed this; if so, do not recompute
    if (knapsacktable[capacity*dim+n] != UNKNOWN)
        return knapsacktable[capacity*dim+n];

    if (n == 0)  // BASE CASE
    {
        result = 0;
    }
    else         // RECURSIVE CASE
    {
        int max_without_last =
            knapsack_mem_recurse(weights, values,
                                 capacity, n-1,
                                 knapsacktable, dim);
        if (weights[n-1] > capacity)
            result = max_without_last;
        else
        {
            int max_with_last = values[n-1] +
                knapsack_mem_recurse(weights, values,
                                     capacity-weights[n-1], n-1,
                                     knapsacktable, dim);
            result = max(max_with_last, max_without_last);
        }
    }

    // Save our result and return it
    knapsacktable[capacity*dim+n] = result;
    return result;
}


// knapsack_mem
// Given lists of object weights & values, and knapsack capacity,
// returns greatest possible value in knapsack problem.
// Memory-function version.
// Calls recursive helper function knapsack_mem_recurse.
int knapsack_mem(const vector<int> & weights,
                 const vector<int> & values,
                 int capacity)
{
    assert(weights.size() == values.size());
    size_t n = weights.size();

    // Create space for memoized values.
    // Items start at UNKNOWN, meaning not valid. We store values for
    // 0..capacity and 0..n; we need dimensions (capacity+1) and (n+1).
    size_t dim = n+1;
    vector<int> knapsacktable((capacity+1)*dim, UNKNOWN);
    return knapsack_mem_recurse(weights, values,
                                capacity, n,
                                knapsacktable, dim);
}


// try_knapsack
// Calls functions on given number of objects; prints results.
void try_knapsack(size_t n,
                  bool dobf)  // True if we do brute-force method
{
    // Sizes & values to push
    vector<int> weightpattern { 1, 10, 1, 10, 10, 1, 10, 1 };
    vector<int> valuepattern { 1, 10, 4, 6, 4, 6, 1, 10 };
    assert(weightpattern.size() == valuepattern.size());

    // Make dataset
    vector<int> weights;
    vector<int> values;
    for (size_t i = 0; i < n; ++i)
    {
        weights.push_back(weightpattern[i%weightpattern.size()]);
        values.push_back(valuepattern[i%weightpattern.size()]);
    }
    int cap = int(4*n);

    // Call knapsack function(s)
    cout << "Number of objects: " << n << "\n";
    cout << "Result (memory-function version): \n";
    cout << "    " << knapsack_mem(weights, values, cap) << "\n";
    if (dobf)
    {
        cout << "Result (brute-force version): \n";
        cout << "    " << knapsack_bf(weights, values, cap) << "\n";
    }
    cout << "\n";
}


// getYN
// Prompt user with given string, repeatedly input until user types a
// string beginning with 'y', 'Y', 'n', or 'N'. Return yes/no to caller
// as bool (true means yes). Exit on EOF.
bool getYN(const string & prompt)
{
    while (true)
    {
        cout << prompt;
        string line;
        getline(cin, line);
        if (!cin)
        {
            cin.clear();
            cin.ignore();
            continue;
        }

        if (line.size() > 0 && (line[0] == 'y' || line[0] == 'Y'))
            return true;
        if (line.size() > 0 && (line[0] == 'n' || line[0] == 'N'))
            return false;

        cout << "\n";
        cout << "Please type \"y\" or \"n\"\n";
    }
}


// userPause
// Wait for user to press ENTER: read all chars through first newline.
void userPause()
{
    std::cout.flush();
    while (std::cin.get() != '\n') ;
}


// Main program
// Try functions for increasing input sizes.
int main()
{
    // Ask user whether to do brute-force computation
    cout << "Knapsack Problem, Memory-Function Method\n\n";
    string prompt =
        "Also use brute-force method (for comparison)? [y/n] ";
    bool do_brute_force = getYN(prompt);
    cout << "\n";

    // Loop through problem sizes
    for (size_t i = 0; i < 1000000; ++i)
        try_knapsack(i, do_brute_force);

    // Wait for user
    cout << "Press ENTER to quit ";
    userPause();
}

