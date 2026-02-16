// bridges_test.cpp
// Glenn G. Chappell
// 2025-09-18
//
// For CS 411 Fall 2025
// Test program for function bridges
// Used in Assignment 2, Exercise A

// Includes for code to be tested
#include "bridges.hpp"       // For function bridges
#include "bridges.hpp"       // Double-inclusion check, for testing only

// Includes for the "doctest" unit-testing framework
#define DOCTEST_CONFIG_IMPLEMENT
                             // We write our own main
#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS
                             // Reduce compile time
#include "doctest.h"         // For doctest

// Includes for all test programs
#include <iostream>
using std::cout;
using std::endl;
using std::cin;
#include <string>
using std::string;

// Additional includes for this test program
#include <vector>
using std::vector;

// Printable name for this test suite
const string test_suite_name =
    "function bridges"
    " - CS 411 Assn 2, Ex A";


// *********************************************************************
// Helper Functions/Classes for This Test Program
// *********************************************************************


// Type for bridge description; will always have size 3
using Brg = vector<int>;  // Type for bridge description


// *********************************************************************
// Test Cases
// *********************************************************************


TEST_CASE( "Tiny Instances" )
{
    vector<Brg> bs;               // Bridge descriptions
    const vector<Brg> & cbs(bs);  // Const ref to above
    int w;                        // Number of west cities
    int e;                        // Number of west cities
    int ans;                      // Desired answer

    // Empty list of bridges
    {
        w = 1;
        e = 1;
        bs.clear();
        ans = 0;
        INFO( "No bridges" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // 1 bridge
    {
        w = 5;
        e = 5;
        bs.clear();
        bs.push_back(Brg { 0, 2, 13 });
        ans = 13;
        INFO( "1 bridge" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // 2 bridges, non-crossing
    {
        w = 5;
        e = 5;
        bs.clear();
        bs.push_back(Brg { 2, 0, 6 });
        bs.push_back(Brg { 3, 4, 5 });
        ans = 11;
        INFO( "2 bridges, non-crossing" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // 2 bridges, common west city, N is best
    {
        w = 5;
        e = 5;
        bs.clear();
        bs.push_back(Brg { 2, 0, 8 });
        bs.push_back(Brg { 2, 3, 4 });
        ans = 8;
        INFO( "2 bridges, common west city, N is best" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // 2 bridges, common west city, N is best (reordered)
    {
        w = 5;
        e = 5;
        bs.clear();
        bs.push_back(Brg { 2, 3, 4 });
        bs.push_back(Brg { 2, 0, 8 });
        ans = 8;
        INFO( "2 bridges, common west city, N is best (reordered)" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // 2 bridges, common west city, S is best
    {
        w = 5;
        e = 5;
        bs.clear();
        bs.push_back(Brg { 2, 0, 2 });
        bs.push_back(Brg { 2, 3, 6 });
        ans = 6;
        INFO( "2 bridges, common west city, S is best" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // 2 bridges, common west city, S is best (reordered)
    {
        w = 5;
        e = 5;
        bs.clear();
        bs.push_back(Brg { 2, 3, 6 });
        bs.push_back(Brg { 2, 0, 2 });
        ans = 6;
        INFO( "2 bridges, common west city, S is best (reordered)" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // 2 bridges, common east city, N is best
    {
        w = 5;
        e = 5;
        bs.clear();
        bs.push_back(Brg { 1, 3, 5 });
        bs.push_back(Brg { 2, 3, 2 });
        ans = 5;
        INFO( "2 bridges, common east city, N is best" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // 2 bridges, common east city, N is best (reordered)
    {
        w = 5;
        e = 5;
        bs.clear();
        bs.push_back(Brg { 2, 3, 2 });
        bs.push_back(Brg { 1, 3, 5 });
        ans = 5;
        INFO( "2 bridges, common east city, N is best (reordered)" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // 2 bridges, common east city, S is best
    {
        w = 5;
        e = 5;
        bs.clear();
        bs.push_back(Brg { 0, 3, 1 });
        bs.push_back(Brg { 1, 3, 7 });
        ans = 7;
        INFO( "2 bridges, common east city, S is best" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // 2 bridges, common east city, S is best (reordered)
    {
        w = 5;
        e = 5;
        bs.clear();
        bs.push_back(Brg { 0, 3, 1 });
        bs.push_back(Brg { 1, 3, 7 });
        ans = 7;
        INFO( "2 bridges, common east city, S is best (reordered)" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // 2 bridges, identical
    {
        w = 5;
        e = 5;
        bs.clear();
        bs.push_back(Brg { 0, 2, 5 });
        bs.push_back(Brg { 0, 2, 5 });
        ans = 5;
        INFO( "2 bridges, identical" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // 2 bridges, same cities, different tolls
    {
        w = 5;
        e = 5;
        bs.clear();
        bs.push_back(Brg { 0, 2, 6 });
        bs.push_back(Brg { 0, 2, 5 });
        ans = 6;
        INFO( "2 bridges, same cities, different tolls" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // 2 bridges, same cities, different tolls (reordered)
    {
        w = 5;
        e = 5;
        bs.clear();
        bs.push_back(Brg { 0, 2, 5 });
        bs.push_back(Brg { 0, 2, 6 });
        ans = 6;
        INFO( "2 bridges, same cities, different tolls (reordered)" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // 2 bridges, crossing, NW-SE is best
    {
        w = 5;
        e = 5;
        bs.clear();
        bs.push_back(Brg { 0, 1, 6 });
        bs.push_back(Brg { 2, 0, 5 });
        ans = 6;
        INFO( "2 bridges, crossing, NW-SE is best" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // 2 bridges, crossing, NW-SE is best (reordered)
    {
        w = 5;
        e = 5;
        bs.clear();
        bs.push_back(Brg { 2, 0, 5 });
        bs.push_back(Brg { 0, 1, 6 });
        ans = 6;
        INFO( "2 bridges, crossing, NW-SE is best (reordered)" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // 2 bridges, crossing, SW-NE is best
    {
        w = 5;
        e = 5;
        bs.clear();
        bs.push_back(Brg { 0, 1, 5 });
        bs.push_back(Brg { 2, 0, 7 });
        ans = 7;
        INFO( "2 bridges, crossing, SW-NE is best" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // 2 bridges, crossing, SW-NE is best (reordered)
    {
        w = 5;
        e = 5;
        bs.clear();
        bs.push_back(Brg { 2, 0, 7 });
        bs.push_back(Brg { 0, 1, 5 });
        ans = 7;
        INFO( "2 bridges, crossing, SW-NE is best (reordered)" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }
}


TEST_CASE( "Small Instances" )
{
    vector<Brg> bs;               // Bridge descriptions
    const vector<Brg> & cbs(bs);  // Const ref to above
    int w;                        // Number of west cities
    int e;                        // Number of west cities
    int ans;                      // Desired answer

    // Example #1 from assignment description
    {
        w = 3;
        e = 3;
        bs.clear();
        bs.push_back(Brg { 0, 1, 3 });
        bs.push_back(Brg { 1, 1, 5 });
        bs.push_back(Brg { 1, 2, 4 });
        bs.push_back(Brg { 2, 0, 8 });
        ans = 8;
        INFO( "Example #1 from assignment description" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // Example #2 from assignment description
    {
        w = 3;
        e = 3;
        bs.clear();
        bs.push_back(Brg { 0, 1, 3 });
        bs.push_back(Brg { 1, 1, 5 });
        bs.push_back(Brg { 1, 2, 4 });
        bs.push_back(Brg { 2, 0, 8 });
        bs.push_back(Brg { 2, 2, 6 });
        ans = 11;
        INFO( "Example #2 from assignment description" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // 3 cities each side, not all joinable
    {
        w = 3;
        e = 3;
        bs.clear();
        bs.push_back(Brg { 0, 0, 4 });
        bs.push_back(Brg { 1, 0, 4 });
        bs.push_back(Brg { 2, 0, 4 });
        bs.push_back(Brg { 2, 1, 4 });
        bs.push_back(Brg { 2, 2, 4 });
        ans = 8;
        INFO( "3 cities each side, not all joinable" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // 3 cities each side, all joinable
    {
        w = 3;
        e = 3;
        bs.clear();
        bs.push_back(Brg { 0, 0, 4 });
        bs.push_back(Brg { 1, 0, 4 });
        bs.push_back(Brg { 2, 0, 4 });
        bs.push_back(Brg { 2, 1, 4 });
        bs.push_back(Brg { 2, 2, 4 });
        bs.push_back(Brg { 1, 1, 4 });
        ans = 12;
        INFO( "3 cities each side, all joinable" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }
}


TEST_CASE( "Medium-Sized Instances" )
{
    vector<Brg> bs;               // Bridge descriptions
    const vector<Brg> & cbs(bs);  // Const ref to above
    int w;                        // Number of west cities
    int e;                        // Number of west cities
    int ans;                      // Desired answer

    // All bridges available #1
    {
        w = 4;
        e = w;
        bs.clear();
        for (int i = 0; i < w; ++i)
            for (int j = 0; j < e; ++j)
                bs.push_back(Brg { i, j, 3 });
        ans = w * 3;
        INFO( "All bridges available #1" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // All bridges available #2
    {
        w = 4;
        e = w;
        bs.clear();
        for (int i = 0; i < w; ++i)
            for (int j = 0; j < e; ++j)
                bs.push_back(Brg { i, j, (i == j ? 3 : 5) });
        ans = (w-1) * 5;
        INFO( "All bridges available #2" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // All bridges available #3
    {
        w = 4;
        e = w;
        bs.clear();
        for (int i = w-1; i >= 0; --i)
            for (int j = e-1; j >= 0; --j)
                bs.push_back(Brg { i, j,
                                   (-1 <= i-j && i-j <= 1 ? 3 : 7) });
        ans = (w-2) * 7;
        INFO( "All bridges available #3" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // Random #1
    {
        w = 12;
        e = 12;
        bs.clear();
        bs.push_back(Brg {  2, 10,  5 });
        bs.push_back(Brg {  4,  0,  5 });
        bs.push_back(Brg {  1,  1,  7 });
        bs.push_back(Brg { 11,  2,  7 });
        bs.push_back(Brg {  2,  6,  5 });
        bs.push_back(Brg {  0,  5,  7 });
        bs.push_back(Brg {  7,  8,  5 });
        bs.push_back(Brg { 10,  1,  5 });
        bs.push_back(Brg {  8, 11,  7 });
        bs.push_back(Brg {  9,  3,  5 });
        bs.push_back(Brg {  3,  7,  7 });
        bs.push_back(Brg {  5,  4,  7 });
        bs.push_back(Brg {  8,  9,  7 });
        bs.push_back(Brg {  3,  7,  5 });
        bs.push_back(Brg {  6, 11,  5 });
        bs.push_back(Brg {  9,  4,  7 });
        ans = 31;
        INFO( "Random #1" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // Random #2
    {
        w = 12;
        e = 12;
        bs.clear();
        bs.push_back(Brg {  4,  8,  2 });
        bs.push_back(Brg { 11,  6,  2 });
        bs.push_back(Brg {  2, 11,  4 });
        bs.push_back(Brg {  5,  7,  2 });
        bs.push_back(Brg {  9,  1,  4 });
        bs.push_back(Brg {  6,  4,  2 });
        bs.push_back(Brg {  0,  1,  4 });
        bs.push_back(Brg {  3,  5,  4 });
        bs.push_back(Brg {  4,  9,  4 });
        bs.push_back(Brg {  3, 10,  2 });
        bs.push_back(Brg {  8,  9,  2 });
        bs.push_back(Brg {  3,  2,  4 });
        bs.push_back(Brg {  8, 11,  2 });
        bs.push_back(Brg { 10,  2,  4 });
        bs.push_back(Brg {  1,  0,  2 });
        bs.push_back(Brg {  7,  7,  4 });
        ans = 16;
        INFO( "Random #2" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // Random #3
    {
        w = 12;
        e = 12;
        bs.clear();
        bs.push_back(Brg {  7, 11,  6 });
        bs.push_back(Brg {  9,  4,  7 });
        bs.push_back(Brg {  6,  9,  7 });
        bs.push_back(Brg {  8,  1,  6 });
        bs.push_back(Brg {  0,  7,  6 });
        bs.push_back(Brg {  3,  6,  7 });
        bs.push_back(Brg {  5,  3,  6 });
        bs.push_back(Brg { 11,  2,  7 });
        bs.push_back(Brg { 10,  0,  7 });
        bs.push_back(Brg {  2,  7,  7 });
        bs.push_back(Brg {  1, 10,  7 });
        bs.push_back(Brg {  4,  9,  6 });
        bs.push_back(Brg {  5,  1,  6 });
        bs.push_back(Brg {  4,  2,  7 });
        bs.push_back(Brg {  8, 11,  6 });
        bs.push_back(Brg {  3,  8,  6 });
        ans = 26;
        INFO( "Random #3" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // One bridge crosses many #1
    {
        w = 12;
        e = w;
        bs.clear();
        for (int i = 0; i < w; ++i)
            bs.push_back(Brg { i, i, 1 });
        bs.push_back(Brg { 0, e-1, w+1 });
        bs.push_back(Brg { w-1, 0, w+2 });

        ans = w+2;
        INFO( "One bridge crosses many #1" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }

    // One bridge crosses many #2
    {
        w = 12;
        e = w;
        bs.clear();
        for (int i = 0; i < w; ++i)
            bs.push_back(Brg { i, i, 1 });
        bs.push_back(Brg { 0, e-1, w-1 });
        bs.push_back(Brg { w-1, 0, w-2 });

        ans = w;
        INFO( "One bridge crosses many #2" );
        REQUIRE( ans == bridges(w, e, cbs) );
    }
}


// *********************************************************************
// Main Program
// *********************************************************************


// userPause
// Wait for user to press ENTER: read all chars through first newline.
void userPause()
{
    std::cout.flush();
    while (std::cin.get() != '\n') ;
}


// Main program
// Run all tests. Prompt for ENTER before exiting.
int main(int argc,
         char *argv[])
{
    doctest::Context dtcontext;
                             // Primary doctest object
    int dtresult;            // doctest return code; for return by main

    // Handle command line
    dtcontext.applyCommandLine(argc, argv);
    dtresult = 0;            // doctest flags no command-line errors
                             //  (strange but true)

    if (!dtresult)           // Continue only if no command-line error
    {
        // Run test suites
        cout << "BEGIN tests for " << test_suite_name << "\n" << endl;
        dtresult = dtcontext.run();
        cout << "END tests for " << test_suite_name << "\n" << endl;
    }

    // If we want to do something else here, then we need to check
    // dtcontext.shouldExit() first.

    // Wait for user
    std::cout << "Press ENTER to quit ";
    userPause();

    // Program return value is return code from doctest
    return dtresult;
}

