// dist_count_sort.cpp
// Glenn G. Chappell
// 2025-10-17
//
// For CS 411 Fall 2025
// Distribution Counting Sort

#include <iostream>
using std::cout;
using std::endl;
using std::cin;
#include <string>
using std::string;
using std::to_string;
#include <vector>
using std::vector;
#include <cstddef>
using std::size_t;
#include <iterator>
using std::begin;
using std::end;
using std::iterator_traits;
#include <algorithm>
using std::move;
using std::swap;
#include <chrono>
// Everything from <chrono> is preceded by std::
#include <cassert>
// For assert


// Size of large datasets
const size_t BIGSIZE = 100'000'000;

// Values in datasets range from 0 to MAXVAL
const int MAXVAL = 999'999;


// distributionCountingSort
// Sort a range using Distribution Counting Sort.
// Requirements on Types:
//     RAIter is a random-access iterator type.
//     The value type of RAIter is int.
// Pre:
//     [first, last) is a valid range.
template <typename RAIter>
void distributionCountingSort(RAIter first, RAIter last)
{
    size_t size = last - first;

    // FInd min, max elements of data
    auto p = minmax_element(first, last);
    auto minelt = *(p.first);
    auto maxelt = *(p.second);

    // Initialize frequencies
    vector<size_t> freqs(maxelt-minelt+1, 0);
    for (size_t i = 0; i < size; ++i)
    {
        ++freqs[first[i]-minelt];
    }

    // freqs[k-minval] is now number of times k occurs in given range

    // Replace freqs[k] with sum of freqs[0]..freqs[k-1]
    size_t runtotal = 0;
    for (size_t k = 0; k < freqs.size(); ++k)
    {
        auto save = freqs[k];
        freqs[k] = runtotal;
        runtotal += save;
    }

    // freqs[k-minval] is now the index, in sorted version of given
    //  range, of 1st item equal to k (if any).

    // Make sorted list
    vector<int> buffer(size);
    for (size_t i = 0; i < size; ++i)
    {
        buffer[freqs[first[i]-minelt]] = first[i];
        ++freqs[first[i]-minelt];
            // Update index for next w/ same value
    }

    // Copy back to original storage
    move(begin(buffer), end(buffer), first);
}


// *********************************************************************
// Code to Call Sorting Function
//
// Place sorting call in function doSort!!!
// *********************************************************************


// timesec
// Returns time in seconds since the epoch. Resolution is nanoseconds,
// if the system provides this.
// Requires #include <chrono>.
double timesec()
{
    auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch());
    return static_cast<double>(nanos.count())/1.e9;
}


// doSort
// Wrapper function for our sorting algorithm. Sorts given range with
// messages before & after. Prints elapsed time.
//
// PLACE CALL TO THE PROPER SORTING ALGORITHM IN INDICATED LOCATION!!!
//
// Requirements on Types:
//     Same as sorting function.
// Pre:
//     Same as sorting function.
// Throws what & when sorting function throws.
template<typename Iter>
void doSort(Iter first,
            Iter last)
{
    // Message: before
    cout << "  Sorting ... ";
    cout.flush();

    // Get starting time
    double starttime = timesec();

    // *********************************************************
    // * THE FOLLOWING MUST BE THE APPROPRIATE SORTING CALL!!! *
    // *********************************************************
    distributionCountingSort(first, last);

    // Get ending time
    double endtime = timesec();

    // Message: after
    cout << "DONE" << endl;
    cout << "  Elapsed time: " << endtime-starttime << " (sec)" << endl;
}


// intWithSep
// Return string form of given integer with a separator (default: ",")
// every 3 digits.
// intWithSep(-123456789) returns "-123,456,789"s.
// intWithSep(1234567, ".") returns "1.234.567"s.
// Requirements on Types:
//     IntType must be an integer type.
template <typename IntType>
string intWithSep(IntType num,               // Integer to stringify
                  const string & sep = ",")  // Separator
{
    const auto SPACING = 3;  // How many digits between separators
    auto result = to_string(num);
    auto pos = result.size();
    while (pos > SPACING + (num < 0))
    {
        pos -= SPACING;
        result.insert(pos, sep);
    }
    return result;
}


// trySort_small
// Call doSort on small dataset. Values range from 0 to MAXVAL (global).
// Pre:
//    Preconditions, if any, for sorting call made by doSort.
void trySort_small()
{
    // Unprocessed version of dataset to sort
    auto rawdata = { 123456, 34, 0, 56, 2, 654321, 123, 1, 0, 99 };

    // Initial message
    cout << "Sorting trial: Small dataset\n";
    cout << "Size = " << intWithSep(rawdata.size()) << "\n";
    cout << "\n";

    // Make dataset
    vector<int> data;
    for (auto x : rawdata)
    {
        assert (x >= 0);
        data.push_back(x % (1+MAXVAL));
    }

    // Print initial data
    cout << "  Before:\n";
    cout << "    ";
    for (const auto & val : data)
        cout << val << " ";
    cout << "\n";

    // Sort
    doSort(begin(data), end(data));

    // Print final data
    cout << "  After:\n";
    cout << "    ";
    for (const auto & val : data)
        cout << val << " ";
    cout << endl;
}


// trySort_messy
// Call doSort on "messy" data. Size is BIGISIZE (global). Values range
// from 0 to MAXVAL (global).
// This should give typical performance.
// Pre:
//    Preconditions, if any, for sorting call made by doSort.
void trySort_messy()
{
    // Initial message
    cout << "Sorting trial: random-ish data\n";
    cout << "Size = " << intWithSep(BIGSIZE) << "\n";
    cout << "\n";

    // Make dataset
    vector<int> data(BIGSIZE);
    const double phi = 1.6180339887498948482;
    for (size_t i = 0; i < BIGSIZE; ++i)
    {
        double x = double(i+1)*phi;
        double fracpart = x - int(x);
        data[i] = int(fracpart * (1+MAXVAL));
    }

    // Sort
    doSort(begin(data), end(data));
}


// userPause
// Wait for user to press ENTER: read all chars through first newline.
void userPause()
{
    std::cout.flush();
    while (std::cin.get() != '\n') ;
}


// Main program
// Sorts a number of datasets, printing results.
int main()
{
    // ********** Dataset spec's **********

    cout << "Size of large datasets: ";
    cout << intWithSep(BIGSIZE) << ".\n";
    cout << "To change this, set BIGSIZE in the source code.\n";
    cout << "Values in datasets range from 0 to ";
    cout << intWithSep(MAXVAL) << ".\n";

    // ********** Sorting **********

    cout << "\n";
    trySort_small();

    cout << "\n";
    trySort_messy();

    // ********** Done **********

    cout << endl;

    // Wait for user
    cout << "Press ENTER to quit ";
    userPause();
}

