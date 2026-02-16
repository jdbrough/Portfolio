// first_fit.cpp
// Glenn G. Chappell
// 2025-08-25
//
// For CS 411 Fall 2025
// First Fit: approximation algorithm for the Bin Packing Problem

#include <iostream>
using std::cout;
using std::endl;
using std::cin;
#include <vector>
using std::vector;
#include <cassert>
// For assert


// userPause
// Wait for user to press ENTER: read all chars through first newline.
void userPause()
{
    std::cout.flush();
    while (std::cin.get() != '\n') ;
}


// firstFit
// Does First Fit algorithm, an approximation algorithm for the Bin
// Packing Problem. Returns number of bins used.
// Pre:
//     capacity > 0
//     For all legal i, 0 < sizes[i] <= capacity
int firstFit(int capacity,               // Capacity of all bins
             const vector<int> & sizes)  // Size of each object
{
    assert(capacity > 0);

    vector<int> bins;  // Each item holds total of sizes
                       //  of objects contained

    for (auto s : sizes)
    {
        assert (0 < s && s <= capacity);

        bool added = false;     // True if object placed in a bin
        for (auto & b : bins)   // By reference, since we modify
        {
            if (b + s <= capacity)
            {
                b += s;         // Place object in bin
                added = true;   // We have placed object in a bin
                break;
            }
        }
        if (!added)
        {  // Object is not placed
            bins.push_back(s);  // Place object into new empty bin
        }
    }

    return int(bins.size());  // Return number of bins
}


// Main program
// Call firstFit with a simple dataset.
int main()
{
    // Print initial message
    cout << "First Fit Algorithm\n";
    cout << endl;

    // Data to pass to First Fit algorithm
    int cap = 10;                        // Capacity of all bins
    vector<int> objs { 3, 5, 5, 3, 3 };  // Sizes of objects

    // Output passed data
    cout << "Capacity of all bins: " << cap << "\n";
    cout << "Sizes of objects:\n ";
    for (auto s : objs)
    {
        cout << " " << s;
    }
    cout << "\n";
    cout << endl;

    // Run First Fit algorithm
    cout << "Running First Fit algorithm ... ";
    cout.flush();
    auto result = firstFit(cap, objs);
    cout << "DONE\n";
    cout << endl;

    // Print results
    cout << "Result: number of bins used = " << result << "\n";
    cout << endl;

    // Wait for user
    cout << "Press ENTER to quit ";
    userPause();
}

