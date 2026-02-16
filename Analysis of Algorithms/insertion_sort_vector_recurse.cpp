// insertion_sort_vector_recurse.cpp
// Glenn G. Chappell
// 2025-09-17
//
// For CS 411 Fall 2025
// Insertion Sort on a vector, recursive

#include <iostream>
using std::cout;
using std::endl;
using std::cin;
#include <vector>
using std::vector;
#include <cstddef>
using std::size_t;
#include <utility>
using std::move;
#include <cassert>
// For assert


// insertionSortVectorRecurse
// Sort first *size* items in a vector using Insertion Sort, recursive
// form.
// Requirements on Types:
//     operator< is a total order on type Val.
//     Type Val has a copy ctor & copy assignment.
// Pre:
//     size <= v.size().
template <typename Val>
void insertionSortVectorRecurse(vector<Val> & v,
                                size_t size)
{
    assert (size <= v.size());

    // BASE CASE
    if (size <= 1)
        return;

    // RECURSIVE CASE
    insertionSortVectorRecurse(v, size-1);

    auto save_last_item = std::move(v[size-1]);

    // Find the spot for last item, moving up other items as we go
    size_t k;  // We use k after the loop, so declare it outside
    for (k = size-1; k != 0; --k)
        // Be careful! Backwards loop with unsigned counter
    {
        if (!(save_last_item < v[k-1]))
            break;
        v[k] = std::move(v[k-1]);
    }

    // Last item should be in spot k; put it there
    v[k] = std::move(save_last_item);
}


// userPause
// Wait for user to press ENTER: read all chars through first newline.
void userPause()
{
    std::cout.flush();
    while (std::cin.get() != '\n') ;
}


// Main program
// Demonstrates use of function insertionSortVectorRecurse.
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
    insertionSortVectorRecurse(data, data.size());

    // Print results
    cout << "After:\n";
    for (const auto & val : data)
        cout << val << " ";
    cout << "\n" << endl;

    // Wait for user
    cout << "Press ENTER to quit ";
    userPause();
}

