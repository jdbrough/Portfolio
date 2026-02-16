// closest_pair_dc.cpp
// VERSION 2
// Glenn G. Chappell
// 2025-10-06
//
// For CS 411 Fall 2025
// Closest Pair, Divide and Conquer Solution

#include <iostream>
using std::cout;
using std::endl;
using std::cin;
using std::fixed;
#include <iomanip>
using std::setprecision;
#include <vector>
using std::vector;
#include <utility>
using std::pair;
#include <cmath>
using std::sqrt;
#include <cstddef>
using std::size_t;
#include <cstdlib>
using std::exit;
#include <string>
using std::string;
using std::getline;
#include <sstream>
using std::istringstream;
#include <algorithm>
using std::swap;
using std::sort;
#include <random>
using std::mt19937;
using std::random_device;
using std::uniform_real_distribution;
#include <numeric>
using std::iota;
#include <chrono>
// Everything from <chrono> is preceded by std::
#include <cassert>  // for assert


// getNum
// Prints prompt to cout and then inputs a number of type int on a line
// from cin. Repeats until valid number obtained; returns it to caller
// in reference argument. Return is false if number could not be
// obtained.
bool getNum(const string & prompt,
            int & num)
{
    while (true)
    {
        cout << prompt;
        string line;
        getline(cin, line);
        if (!cin)
        {
            if (cin.eof())
                return false;
            continue;
        }

        istringstream istr(line);
        istr >> num;
        if (!istr)
            continue;
        return true;
    }
}


// struct Pt2
// Holds a 2-D point in the obvious way
struct Pt2 {
    Pt2(double theX, double theY)
        :x(theX),
         y(theY)
    {}

    double x;
    double y;
};


// distsqr
// Given two Pt2 objects, returns square of distance between them.
double distsqr(const Pt2 & p1,
               const Pt2 & p2)
{
    double xd = p1.x - p2.x;
    double yd = p1.y - p2.y;
    return xd*xd + yd*yd;
}


// randPt2
// Returns a random Pt2 object, both coordinates in range [min, max],
//  uniformly distributed, based on given PRNG.
Pt2 randPt2(mt19937 & r)
{
    static uniform_real_distribution<> dis(-100., 100.);

    double x = dis(r);
    double y = dis(r);
    return Pt2(x, y);
}


// printPoint
// Given a list of Pt2 objects and an index in this list,
// output the point with the given index.
void printPoint(const vector<Pt2> & pts,
                size_t index)
{
    cout << "  Index " << index
         << ": (" << pts[index].x << ", " << pts[index].y << ")\n";
}


// printPair
// Given a list of Pt2 objects and a pair of indices in this list,
// output the points with the given indices, in order of increasing
// index, along with the distance between the two points.
void printPair(const vector<Pt2> & pts,
               pair<size_t, size_t> p)
{
    size_t a = p.first;
    size_t b = p.second;
    if (b < a)
        swap(a, b);
    printPoint(pts, a);
    printPoint(pts, b);
    cout << "Distance: " << sqrt(distsqr(pts[a], pts[b])) << "\n";
}


// closestPair_bf
// Given vector of Pt2 objects, returns a std::pair holding the indices
//  of a closest pair of distinct points.
// Uses Brute Force method.
// Pre:
//     pts.size() >= 2.
pair<size_t, size_t> closestPair_bf(const vector<Pt2> & pts)
{
    size_t n = pts.size();  // Number of pts in dataset
    assert (n >= 2);

    // Variables holding best pair so far & squared distances to compare
    // Best pair so far is initialized to first 2 pts given
    pair<size_t, size_t> closepair(0, 1);
                             // Indices of best pair so far
    double closedist = distsqr(pts[0], pts[1]);
                             // Distance squared of best pair so far
    double newdist;          // Distance squared of new pair

    // Loop through all pairs of indices i,j with 0 <= i < j < n
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = i+1; j < n; ++j)
        {
            // Find distance squared of current pair
            newdist = distsqr(pts[i], pts[j]);

            // Is this pair closer than best so far?
            if (newdist < closedist)
            {
                // Found closer pair; save it
                closepair.first = i;
                closepair.second = j;
                closedist = newdist;
            }
        }
    }

    // Return indices of closest pair
    return closepair;
}


// closestPair_dc_recurse
// Given vector of Pt2 objects, and 2 ranges of indices into this list:
//  one x-sorted, one y-sorted, returns a std::pair holding the indices
//  of a closest pair of distinct points.
// Recursive helper function for closestPair_dc.
// Uses Divide-and-Conquer method.
// Requirements on Types:
//     RAIter is a random-access iterator type.
//     The value type of RAIter is Pt2.
// Pre:
//     pts.size() >= 2.
//     No two items in pts have the same x-coordinate.
//     [xsort_f, xsort_l) is a valid range.
//     xsort_l-xsort_f >= 2.
//     Values in [xsort_f, xsort_l) are pairwise distinct indices into
//      pts. These are sorted by the x-coordinate of the indexed Pt2,
//      ascending.
//     [ysort_f, ysort_l) is a valid range.
//     ysort_l-ysort_f >= 2.
//     Values in [ysort_f, ysort_l) are pairwise distinct indices into
//      pts. These are sorted by the y-coordinate of the indexed Pt2,
//      ascending.
//     Ranges [xsort_f, xsort_l), [ysort_f, ysort_l) contain the same
//      values, possibly in different orders.
// Post:
//     Return is std::pair holding indices, in pts, of a closest pair of
//      distinct points whose indices lie in [xsort_f, xsort_l).
template<typename RAIter>
pair<size_t, size_t> closestPair_dc_recurse(
    const vector<Pt2> & pts,
    RAIter xsort_f, RAIter xsort_l,
    RAIter ysort_f, RAIter ysort_l)
{
    size_t n = xsort_l - xsort_f;
    assert (n >= 2);
    assert (n == size_t(ysort_l - ysort_f));
    assert (n <= pts.size());

    // Variables holding best pair so far & distances to compare
    pair<size_t, size_t> closepair;  // Indices of best pair
    double closedist;                // Distance of best pair
    double newdist;                  // Distance of new pair

    // BASE CASE

    const size_t SMALL_SIZE = 50;  // Max base case size (must be >= 3)
    assert (SMALL_SIZE >= 3);
    if (n <= SMALL_SIZE)
    {
        closepair.first = xsort_f[0];
        closepair.second = xsort_f[1];
                                   // Indices of first 2 pts
        closedist = distsqr(pts[closepair.first],
                            pts[closepair.second]);
                                   // Distance of first 2 pts

        // Loop through all pairs of indices i,j with 0 <= i < j < n
        for (size_t i = 0; i < n; ++i)
        {
            size_t a = xsort_f[i];
            for (size_t j = i+1; j < n; ++j)
            {
                size_t b = xsort_f[j];

                // Find distance of current pair
                newdist = distsqr(pts[a], pts[b]);

                // Is this pair closer than best so far?
                if (newdist < closedist)
                {
                    // Found closer pair; save it
                    closepair.first = a;
                    closepair.second = b;
                    closedist = newdist;
                }
            }
        }

        // Return indices of closest pair
        return closepair;
    }

    // RECURSIVE CASE

    // Find median of x-coords of pts: xsplit
    size_t halfn = n/2;
    assert (halfn >= 2);
    assert (n-halfn >= 2);
    double x1 = pts[xsort_f[halfn-1]].x;
    double x2 = pts[xsort_f[halfn]].x;
    assert (x1 != x2);
    double xsplit = (x1+x2)/2.;

    // Create y-sorted list of indices of pts on lo, hi side of x split
    vector<size_t> lo_x_ysort;
    lo_x_ysort.reserve(halfn);
    vector<size_t> hi_x_ysort;
    hi_x_ysort.reserve(n-halfn);
    for (size_t i = 0; i < n; ++i)
    {
        size_t b = ysort_f[i];
        if (pts[b].x < xsplit)
            lo_x_ysort.push_back(b);
        else
            hi_x_ysort.push_back(b);
    }
    assert (lo_x_ysort.size() == halfn);
    assert (hi_x_ysort.size() == n-halfn);

    // Do 1st recursive call
    closepair = closestPair_dc_recurse(
        pts,
        xsort_f, xsort_f+halfn,
        lo_x_ysort.begin(), lo_x_ysort.end());
    closedist = distsqr(pts[closepair.first], pts[closepair.second]);

    // Do 2nd recursive call
    auto newpair = closestPair_dc_recurse(
        pts,
        xsort_f+halfn, xsort_l,
        hi_x_ysort.begin(), hi_x_ysort.end());
    newdist = distsqr(pts[newpair.first], pts[newpair.second]);

    // Did 2nd recursive call give better result?
    if (newdist <= closedist)
    {
        closepair = newpair;
        closedist = newdist;
    }

    // Deal with center strip: pts whose x-coordinates are within the
    // current minimum distance (closedist) of the x-coordinate split
    // point (xsplit).

    // Create y-sorted list of indices of pts in center strip
    vector<size_t> strip_ysort;
    strip_ysort.reserve(n);
    for (size_t i = 0; i < n; ++i)
    {
        size_t b = ysort_f[i];
        if (xsplit-closedist < pts[b].x && pts[b].x < xsplit+closedist)
            strip_ysort.push_back(b);
    }

    // Iterate through center strip
    // For each point, we consider the pairs consisting of this point
    // and each of the next K points.
    const size_t K = 3;  // How many upcoming points to look at
    for (size_t i = 0; i < strip_ysort.size(); ++i)
    {
        size_t a = strip_ysort[i];
        for (size_t j = i+1; j < i+K && j < strip_ysort.size(); ++j)
        {
            size_t b = strip_ysort[j];

            // Find distance of current pair
            newdist = distsqr(pts[a], pts[b]);

            // Is this pair closer than best so far?
            if (newdist < closedist)
            {
                // Found closer pair; save it
                closepair.first = a;
                closepair.second = b;
                closedist = newdist;
            }
        }
    }

    // Return indices of closest pair
    return closepair;
}


// closestPair_dc
// Given vector of Pt2 objects, returns a std::pair holding the indices
//  of a closest pair of distinct points.
// Calls closestPair_dc_recurse to do most of the work.
// Uses Divide-and-Conquer method.
// Pre:
//     pts.size() >= 2.
//     No two items in pts have the same x-coordinate.
// Post:
//     Return is std::pair holding indices, in pts, of a closest pair of
//      distinct points.
pair<size_t, size_t> closestPair_dc(const vector<Pt2> & pts)
{
    size_t n = pts.size();  // No. of pts in dataset
    assert (n >= 2);

    // Make x-sorted & y-sorted list of indices into pts
    vector<size_t> xsort(n);
    iota(xsort.begin(), xsort.end(), 0);
    sort(xsort.begin(), xsort.end(),
      [&pts](size_t a, size_t b) { return pts[a].x < pts[b].x; }
    );
    vector<size_t> ysort(n);
    iota(ysort.begin(), ysort.end(), 0);
    sort(ysort.begin(), ysort.end(),
      [&pts](size_t a, size_t b) { return pts[a].y < pts[b].y; }
    );

   return closestPair_dc_recurse(
       pts,
       xsort.begin(), xsort.end(),
       ysort.begin(), ysort.end());
}


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


// userPause
// Wait for user to press ENTER: read all chars through first newline.
void userPause()
{
    std::cout.flush();
    while (std::cin.get() != '\n') ;
}


// Main program
// Generate list of random 2-D pts, find closest pair using both Divide
// and Conquer and Brute Force methods, print results.
int main()
{
    random_device rd;   // For nondeterministic seeding
    mt19937 rng(rd());  // Pseudorandom number generator

    cout << fixed << setprecision(8);  // Floating-point output format

    // Get number of points
    int numpts;
    while (true)
    {
        bool success =
            getNum("Number of points (at least 2)? ", numpts);
        if (!success)
            exit(1);
        if (numpts >= 2)
            break;
        cout << endl;
        cout << "Number of points must be at least 2" << endl;
    }
    cout << "\n";

    // Generate list of points
    vector<Pt2> pts;
    for (int i = 0; i < numpts; ++i)
    {
        pts.push_back(randPt2(rng));
    }

    // Print coordinates, if not many points; print how many points
    cout << numpts << " points generated";
    if (numpts <= 10)
    {
        cout << ":\n";
        for (int i = 0; i < numpts; ++i)
        {
            printPoint(pts, i);
        }
    }
    else
    {
        cout << "\n";
    }
    cout << "\n";

    // Variables for results
    pair <size_t, size_t> closepair;  // Indices of closest pair

    // Compute closest pair (Divide and Conquer) & elapsed time
    cout << "Finding closest pair, divide and conquer method\n\n";
    cout << "  Computing ... ";  // Message: before
    cout.flush();
    double starttime = timesec();  // Get starting time
    closepair = closestPair_dc(pts);
    double endtime = timesec(); // Get ending time
    cout << "DONE" << endl;  // Message: after
    cout << "  Elapsed time: " << endtime-starttime << " sec\n\n";

    // Print closest pair
    cout << "Closest pair:\n";
    printPair(pts, closepair);
    cout << "\n";

    // Compute closest pair (Brute Force) & elapsed time
    cout << "Finding closest pair, brute force method\n\n";
    cout << "  Computing ... ";  // Message: before
    cout.flush();
    starttime = timesec();  // Get starting time
    closepair = closestPair_bf(pts);
    endtime = timesec(); // Get ending time
    cout << "DONE" << endl;  // Message: after
    cout << "  Elapsed time: " << endtime-starttime << " sec\n\n";

    // Print closest pair
    cout << "Closest pair:\n";
    printPair(pts, closepair);
    cout << "\n";

    // Wait for user
    cout << "Press ENTER to quit ";
    userPause();
}

