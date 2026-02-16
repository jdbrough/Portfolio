// introselect.cpp
// Glenn G. Chappell
// 2025-09-28
//
// For CS 411 Fall 2025
// Introselect with Lomuto Partition
// Based on quickselect.cpp, mom_select.cpp

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
#include <algorithm>
using std::iter_swap;
using std::swap;
#include <cmath>
using std::log2;
#include <chrono>
// Everything from <chrono> is preceded by std::
#include <cassert>
// For assert


// Size of large datasets
const size_t BIGSIZE = 100'000'000;

// Values in datasets range from 0 to MAXVAL
const int MAXVAL = 999'999'999;


// *********************************************************************
// Forward Declarations
// *********************************************************************


// momSelect
// Selection using Median-of-Medians Selection algorithm.
template <typename RAIter>
RAIter momSelect(RAIter first, RAIter last,  // Range: [first, last)
                 size_t index);              // Selection index


// lPartition
// Partitions a sequence about a given pivot. Uses Lomuto Algorithm.
// Returns the new pivot position via a reference parameter.
//
// Requirements on Types:
//     FDIter is a forward iterator type.
//     The value type of FDIter must have copy=, operator<.
//     operator< must be a total order on the value type of FDIter.
// Pre:
//     [first, last) is a valid nonempty range.
//     pivotiter is an iterator in the range [first, last).
template <typename FDIter>
void lPartition(FDIter first, FDIter last,  // Range to partition
                FDIter & pivotiter)         // Iterator to pivot
{
    // Put the pivot at the start of the list
    if (first != pivotiter)
        iter_swap(first, pivotiter);

    // Now "first" points to the pivot

    // Make the "left list": list of items less than pivot
    auto leftFinal = first;  // points to final item in left list
    auto check = first;      // item to check
    for (++check;            // start after pivot, iterate thru list
         check != last;
         ++check)
    {
        if (*check < *first)              // if item < PIVOT
        {
            ++leftFinal;                  // move up end mark
            iter_swap(leftFinal, check);  // and put item in left list
        }
    }

    // Note new pivot position for caller, and put pivot there
    pivotiter = leftFinal;
    if (first != pivotiter)
        iter_swap(first, pivotiter);
}


// medianOf5
// Given 5 iterators, finds the median of the values they reference and
//  returns an iterator to it. Values are not altered; return value is
//  always equal to one of the parameters.
// Performs at most 6 comparisons.
// NOT stable (in the non-mutating selection-algorithm sense).
// Based on http://stackoverflow.com/a/2117018 by user DRBlaise.
template <typename Iter>
Iter medianOf5(Iter ai, Iter bi, Iter ci, Iter di, Iter ei)
{
    auto & a(*ai);
    auto & b(*bi);
    auto & c(*ci);
    auto & d(*di);
    auto & e(*ei);

    return b < a ? d < c ? b < d ? a < e ? a < d ? e < d ? ei : di
                                                 : c < a ? ci : ai
                                         : e < d ? a < d ? ai : di
                                                 : c < e ? ci : ei
                                 : c < e ? b < c ? a < c ? ai : ci
                                                 : e < b ? ei : bi
                                         : b < e ? a < e ? ai : ei
                                                 : c < b ? ci : bi
                         : b < c ? a < e ? a < c ? e < c ? ei : ci
                                                 : d < a ? di : ai
                                         : e < c ? a < c ? ai : ci
                                                 : d < e ? di : ei
                                 : d < e ? b < d ? a < d ? ai : di
                                                 : e < b ? ei : bi
                                         : b < e ? a < e ? ai : ei
                                                 : d < b ? di : bi
                 : d < c ? a < d ? b < e ? b < d ? e < d ? ei : di
                                                 : c < b ? ci : bi
                                         : e < d ? b < d ? bi : di
                                                 : c < e ? ci : ei
                                 : c < e ? a < c ? b < c ? bi : ci
                                                 : e < a ? ei : ai
                                         : a < e ? b < e ? bi : ei
                                                 : c < a ? ci : ai
                         : a < c ? b < e ? b < c ? e < c ? ei : ci
                                                 : d < b ? di : bi
                                         : e < c ? b < c ? bi : ci
                                                 : d < e ? di : ei
                                 : d < e ? a < d ? b < d ? bi : di
                                                 : e < a ? ei : ai
                                         : a < e ? b < e ? bi : ei
                                                 : d < a ? di : ai;
}


// medianOf3
// Given 3 iterators, finds the median of the values they reference and
// returns an iterator to it. Values are not altered; return value is
// always equal to one of the parameters. Has properties necessary for
// stable sort.
//
// Does the same comparisons as an Insertion Sort of a 3-item list, but
// does not modify the list.
// Requirements on Types:
//     Iter is an iterator type.
// Pre:
//     ai, bi, ci are valid iterators.
template <typename Iter>
Iter medianOf3(Iter ai, Iter bi, Iter ci)
{
    if (*bi < *ai)
    {
        if (*ci < *ai)
            return (*ci < *bi) ? bi : ci;
        else
            return ai;
    }
    else
    {
        if (*ci < *bi)
            return (*ci < *ai) ? ai : ci;
        else
            return bi;
    }
}


// momPivot
// Does Median-of-Medians pivot choice.
// Returns iterator to pivot.
template <typename RAIter>
RAIter momPivot(RAIter first, RAIter last)
{
    assert (last != first);
    if (last-first == 1)
        return first;

    RAIter spot = first;
    for (RAIter five = first; five < last; five += 5)
    {
        RAIter median;
        if (last-five < 5)
        {
            if (last-five >= 3)
                // A median of a,b,c is a median of a,b,c,d
                median = medianOf3(five, five+1, five+2);
            else
                // A median of a or of a,b is just a
                median = five;
        }
        else
            median = medianOf5(five, five+1, five+2, five+3, five+4);

        iter_swap(median, spot);
        ++spot;
    }

    // Pivot is median of our list of medians; find with momSelect
    return momSelect(first, spot, (spot-first)/2);
}


// momSelect
// Selection using Median-of-Medians Selection algorithm. [first, last)
//  specifies a nonempty range. index is an index in this range
//  (0 .. size-1). Return value is an iterator to the item in this range
//  that would be at the given index, if the range were sorted (not
//  necessarily in a stable manner). The range is modified, but not
//  sorted.
// Uses lPartition, momPivot
// Pre:
//     [first, last) is a nonempty valid range.
//     0 <= index < (last-first).
template <typename RAIter>
RAIter momSelect(RAIter first, RAIter last,  // Range: [first, last)
                 size_t index)               // Selection index
{
    assert (index < last-first);
    while (true)  // For tail-recursion elimination
    {
        // Find median-of-medians pivot & point pivotiter at it
        RAIter pivotiter = momPivot(first, last);
                                             // Indirect recursion!

        // Do partition
        lPartition(first, last, pivotiter);
        size_t pivotindex = pivotiter - first;

        // BASE CASE of former recursive function
        // If the pivot is our item, then return it
        if (index == pivotindex)
            return pivotiter;

        // RECURSIVE CASE of former recursive function
        if (index < pivotindex)
        {   // Recurse on range of items preceding the pivot
            last = pivotiter;
        }
        else
        {   // Recurse on range of items following the pivot
            index -= pivotindex+1;
            first = pivotiter+1;
        }
        // Tail call is gone, replaced by loop
        // return momSelect(first, last, index);
    }
}


// introselect
// Selection using Introselect algorithm. [first, last) specifies a
//  nonempty range. index is an index in this range (0 .. size-1).
//  Return value is an iterator to the item in this range that would be
//  at the given index, if the range were sorted (not necessarily in a
//  stable manner). The range is modified, but not sorted.
// Recursion depth limit for Median-of-Medians Selection is 2 log_2(n),
//  as recommended by David Musser in "Introspective Sorting and
//  Selection Algorithms", 1997.
// Uses lPartition, medianOf3, momSelect
// Pre:
//     [first, last) is a nonempty valid range.
//     0 <= index < (last-first).
template <typename RAIter>
RAIter introselect(RAIter first, RAIter last,  // Range: [first, last)
                   size_t index)               // Selection index
{
    size_t size = last - first;
    assert (index < size);
        // Invariant: size > 0
    int depth = int(2.0 * log2(double(size)));
        // 2 log_2(n)
        // This decreases with each recursive call (recursion
        //  eliminated); when it reaches zero, we switch to
        //  Median-of-Medians Selection.

    while (true)  // For tail-recursion elimination
    {
        // Find median-of-three pivot & point pivotiter at it
        RAIter pivotiter = medianOf3(first, first+size/2, last-1);

        // Do partition
        lPartition(first, last, pivotiter);
        size_t pivotindex = pivotiter - first;

        // BASE CASE of former recursive function
        // If the pivot is our item, then return it
        if (index == pivotindex)
            return pivotiter;

        // RECURSIVE CASE of former recursive function
        if (index < pivotindex)
        {   // Recurse on range of items preceding the pivot
            last = pivotiter;
        }
        else
        {   // Recurse on range of items following the pivot
            index -= pivotindex+1;
            first = pivotiter+1;
        }
        if (depth == 0)
            return momSelect(first, last, index);
                // Recursion (eliminated) depth reached;
                //  switch to Median-of-Medians Selection
        --depth;
        size = last - first;  // Set size to new value
        // Tail call is gone, replaced by loop
        // return introselect(first, last, index);
    }
}


// *********************************************************************
// Code to Call Selection Function
//
// Place selection call in function doSelect!!!
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


// doSelect
// Wrapper function for our selection algorithm. Calls selection on
// given range with messages before & after. Prints elapsed time.
//
// PLACE CALL TO THE PROPER SELECTION ALGORITHM IN INDICATED LOCATION!!!
//
// Requirements on Types:
//     Same as selection function.
// Pre:
//     Same as selection function.
// Throws what & when selection function throws.
template<typename Iter>
void doSelect(Iter first, Iter last,
              size_t index)
{
    // Message: before
    cout << "  Doing selection ... ";
    cout.flush();

    // Get starting time
    double starttime = timesec();

    // ***********************************************************
    // * THE FOLLOWING MUST BE THE APPROPRIATE SELECTION CALL!!! *
    // ***********************************************************
    introselect(first, last, index);

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


// trySelect_small
// Call doSelect on small dataset. Values range from 0 to MAXVAL
// (global).
// Pre:
//    Preconditions, if any, for selection call made by doSelect.
void trySelect_small()
{
    // Unprocessed version of dataset to select in
    auto rawdata = { 123456, 34, 0, 56, 2, 654321, 123, 1, 0, 99 };

    // Initial message
    cout << "Selection trial: Small dataset\n";
    cout << "Size = " << intWithSep(rawdata.size()) << "\n";
    cout << "\n";

    // Make dataset
    vector<int> data;
    for (auto x : rawdata)
    {
        assert (x >= 0);
        data.push_back(x % (1+MAXVAL));
    }
    size_t index = 3;  // Index for selection

    // Print initial data
    cout << "  Before:\n";
    cout << "    ";
    for (const auto & val : data)
        cout << val << " ";
    cout << "\n";
    cout << "    Index: " << index << "\n";

    // Do selection
    doSelect(begin(data), end(data), index);

    // Print final data
    cout << "  After:\n";
    cout << "    ";
    for (const auto & val : data)
        cout << val << " ";
    cout << "\n";
    cout << "    Item at specied index: " << data[index] << "\n";
}


// trySelect_messy
// Call doSelect on "messy" data. Size is BIGISIZE (global). Values
// range from 0 to MAXVAL (global).
// This should give typical performance.
// Pre:
//    Preconditions, if any, for selection call made by doSelect.
void trySelect_messy()
{
    // Initial message
    cout << "Selection trial: random-ish data\n";
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
    size_t index = data.size()*2/3;  // Index for selection

    // Do selection
    doSelect(begin(data), end(data), index);
}


// trySelect_nearlySorted1
// Call doSelect on nearly sorted data of type 1: all items close to
// their proper spots. Size is BIGSIZE (global). Values range from 0 to
// MAXVAL (global).
// Pre:
//    Preconditions, if any, for selection call made by doSelect.
void trySelect_nearlySorted1()
{
    // Initial message
    cout << "Selection trial: Nearly sorted type 1"
         << " (all items close to proper spots)\n";
    cout << "Size = " << intWithSep(BIGSIZE) << "\n";
    cout << "\n";

    // Make dataset
    vector<int> data(BIGSIZE);
    for (size_t i = 0; i < BIGSIZE; ++i)
    {
        size_t residue = i % 4;
        size_t ii = i + 3 - 2*residue;
        data[i] = int(double(ii) / BIGSIZE * MAXVAL);
    }
    size_t index = data.size()*2/3;  // Index for selection

    // Do selection
    doSelect(begin(data), end(data), index);
}


// trySelect_nearlySorted2
// Call doSelect on nearly sorted data of type 2: few items out of
// order. Size is BIGSIZE (global). Values range from 0 to MAXVAL
// (global).
// Pre:
//    Preconditions, if any, for selection call made by doSelect.
void trySelect_nearlySorted2()
{
    // Initial message
    cout << "Selection trial: Nearly sorted type 2"
         << " (few items out of order)\n";
    cout << "Size = " << intWithSep(BIGSIZE) << "\n";
    cout << "\n";

    // Make dataset
    vector<int> data(BIGSIZE);
    for (size_t i = 0; i < BIGSIZE; ++i)
    {
        data[i] = int(double(i) / BIGSIZE * MAXVAL);
    }
    if (BIGSIZE >= 2)
    {
        swap(data[0], data[BIGSIZE-1]);
    }
    size_t index = data.size()*2/3;  // Index for selection

    // Do selection
    doSelect(begin(data), end(data), index);
}


// trySelect_Mo3Killer
// Call doSelect on Median-of-3 killer data. Size is BIGISIZE (global).
// Values range from 0 to MAXVAL (global).
// This should give worst-case performance is Median-of-3 pivot
// selection is used.
// Pre:
//    Preconditions, if any, for selection call made by doSelect.
void trySelect_Mo3Killer()
{
    // Initial message
    cout << "Selection trial: Median-of-3 killer data\n";
    cout << "Size = " << intWithSep(BIGSIZE) << "\n";
    cout << "\n";

    // Make dataset
    vector<int> data(BIGSIZE);
    for (size_t i = 0; i < BIGSIZE; ++i)
        data[i] = int(i);
    if (BIGSIZE >= 2)
    {
        size_t esize = BIGSIZE - (BIGSIZE % 2);
            // Greatest even integer <= BIGSIZE
        for (size_t i = esize-2; ; i -= 2)
        {
            size_t m = i+(esize-i)/2;
            swap(data[i+1], data[m]);

            if (i == 0)
                break;
        }
    }
    size_t index = data.size()*2/3;  // Index for selection

    // Do selection
    doSelect(begin(data), end(data), index);
}


// userPause
// Wait for user to press ENTER: read all chars through first newline.
void userPause()
{
    std::cout.flush();
    while (std::cin.get() != '\n') ;
}


// Main program
// Does selection in a number of datasets, printing results.
int main()
{
    // ********** Dataset spec's **********

    cout << "Size of large datasets: ";
    cout << intWithSep(BIGSIZE) << ".\n";
    cout << "To change this, set BIGSIZE in the source code.\n";
    cout << "Values in datasets range from 0 to ";
    cout << intWithSep(MAXVAL) << ".\n";

    // ********** Selection **********

    cout << "\n";
    trySelect_small();

    cout << "\n";
    trySelect_messy();

    cout << "\n";
    trySelect_nearlySorted1();

    cout << "\n";
    trySelect_nearlySorted2();

    cout << "\n";
    trySelect_Mo3Killer();

    // ********** Done **********

    cout << endl;

    // Wait for user
    cout << "Press ENTER to quit ";
    userPause();
}

