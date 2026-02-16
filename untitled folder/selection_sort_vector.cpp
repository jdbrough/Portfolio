// selection_sort_vector.cpp
// Glenn G. Chappell
// 2025-09-10
//
// For CS 411 Fall 2025
// Selection Sort on a vector

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


// selectionSortVector
// Sort a vector using Selection Sort.
// Requirements on Types:
//     operator< is a total order on type Val.
//     Type Val has a copy ctor & copy assignment.
// Pre:
//     None.
template <typename Val>
void selectionSortVector(vector<Val> & v)
{
    // Get size of vector
    size_t size = v.size();

    for (size_t pass = 0; pass <= size-2; ++pass)  // Passes
        // Passes
        // We deal with items pass .. size-1
        // Note: "pass" is called "i" in the Levitin text
    {
        // Look in v[pass .. size-1]
        //  and find the item that goes in v[pass].

        size_t indexOfSmallest = pass;  // Index of smallest so far
        for (size_t j = pass+1; j <= size-1; ++j)
        {
            if (v[j] < v[indexOfSmallest])
                indexOfSmallest = j;
        }

        // Now v[indexOfSmallgest] goes in v[pass].
        // Put it there, swapping so that the old v[pass] stays around.
        swap(v[indexOfSmallest], v[pass]);
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
// Demonstrates use of function selectionSortVector.
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
    selectionSortVector(data);

    // Print results
    cout << "After:\n";
    for (const auto & val : data)
        cout << val << " ";
    cout << "\n" << endl;

    // Wait for user
    cout << "Press ENTER to quit ";
    userPause();
}

