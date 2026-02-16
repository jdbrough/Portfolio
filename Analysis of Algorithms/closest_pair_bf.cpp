// closest_pair_bf.cpp
// VERSION 2
// Glenn G. Chappell
// 2025-09-12
//
// For CS 411 Fall 2025
// Closest Pair, Brute Force Solution

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
#include <random>
using std::mt19937;
using std::random_device;
using std::uniform_real_distribution;
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
// Generate list of random 2-D pts, find closest pair, print results.
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
        cout << "\n";
        cout << "Number of points must be at least 2\n";
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

    // Compute closest pair & elapsed time
    cout << "Finding closest pair, brute force method\n\n";
    cout << "  Computing ... ";  // Message: before
    cout.flush();
    double starttime = timesec();  // Get starting time
    closepair = closestPair_bf(pts);
    double endtime = timesec(); // Get ending time
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

