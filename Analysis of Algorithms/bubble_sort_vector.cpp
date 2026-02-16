// bubble_sort_vector.cpp
// Glenn G. Chappell
// 2025-09-10
//
// For CS 411 Fall 2025
// Bubble Sort on a vector

#include <iostream>
using std::cout;
using std::endl;
using std::cin;
#include <vector>
using std::vector;
#include <cstddef>
using std::size_t;
#include <algorithm>
using std::swap;


// bubbleSortVector
// Sort a vector using Bubble Sort.
// Requirements on Types:
//     operator< is a total order on type Val.
//     Type Val has a copy ctor & copy assignment.
// Pre:
//     None.
template <typename Val>
void bubbleSortVector(vector<Val> & v)
{
    // Get size of vector
    size_t size = v.size();

    for (size_t pass = 0; pass <= size-2; ++pass)
        // Passes; exit when sorted
        // (size-pass) is number of items dealt with on the pass
        // That is, we deal with items 0 .. size-pass-1
        // Note: "pass" is called "i" in the Levitin text
    {
        // Do one Bubble Sort pass on v[0 .. size-pass-1],
        //  that is, on pairs from 0,1 up to size-pass-2,size-pass-1
        bool swapped = false;  // True if any swaps during this pass

        for (size_t j = 0; j <= size-pass-2; ++j)
            // v[j], v[j+1] are current pair
        {
            // Compare & maybe swap current pair
            if (v[j+1] < v[j])  // Out of order?
            {
                swap(v[j], v[j+1]);
                swapped = true;
            }
        }

        // If no swaps, then vector is sorted; we're done
        if (!swapped)
            break;
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
// Demonstrates use of function bubbleSortVector.
int main()
{
    // Set up data to sort
    vector<int> data { 123456, 34, 0, 56, 2, 654321, 123, 1, 0, 99 };

    // Print unaltered data
    cout << "Before:\n";
    for (const auto & val : data)
        cout << val << " ";
    cout << "\n\n";

    // Do a sort
    bubbleSortVector(data);

    // Print results
    cout << "After:\n";
    for (const auto & val : data)
        cout << val << " ";
    cout << "\n" << endl;

    // Wait for user
    cout << "Press ENTER to quit ";
    userPause();
}

