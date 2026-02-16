// insertion_sort_vector.cpp
// Glenn G. Chappell
// 2025-09-03
//
// For CS 411 Fall 2025
// Insertion Sort on a vector

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


// insertionSortVector
// Sort a vector using Insertion Sort.
// Requirements on Types:
//     operator< is a total order on type Val.
//     Type Val has a copy ctor & copy assignment.
// Pre:
//     None.
template <typename Val>
void insertionSortVector(vector<Val> & v)
{
    // Get size of vector
    size_t size = v.size();
    if (size <= 1)
        return;

    // Iterate through items, inserting each into earlier items
    for (size_t i = 1; i != size; ++i)
    {
        // We need to insert item i into sorted list of items 0 .. i-1

        auto save_item_i = std::move(v[i]);

        // Find the spot for item i, moving up other items as we go
        size_t k;  // We use k after the loop, so declare it outside
        for (k = i; k != 0; --k)
            // Be careful! Backwards loop with unsigned counter
        {
            if (!(save_item_i < v[k-1]))
                break;
            v[k] = std::move(v[k-1]);
        }

        // Item i should be in spot k; put it there
        v[k] = std::move(save_item_i);
        // NOTE. I had "if (k != i)" above, but that would leave
        //  v[i] in a moved-from state when k == i.
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
// Demonstrates use of function insertionSortVector.
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
    insertionSortVector(data);

    // Print results
    cout << "After:\n";
    for (const auto & val : data)
        cout << val << " ";
    cout << "\n" << endl;

    // Wait for user
    cout << "Press ENTER to quit ";
    userPause();
}

