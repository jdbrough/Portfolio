// inversions_test.cpp
// Glenn G. Chappell
// 2025-09-29
//
// For CS 411 Fall 2025
// Test program for function template inversions
// Used in Assignment 3, Exercise B

// Includes for code to be tested
#include "inversions.hpp"    // For function template inversions
#include "inversions.hpp"    // Double-inclusion check, for testing only

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
#include <iterator>
using std::begin;
using std::end;
using std::rbegin;
using std::rend;
#include <vector>
using std::vector;
#include <deque>
using std::deque;

// Printable name for this test suite
const string test_suite_name =
    "function template inversions"
    " - CS 411 Assn 3, Ex A";


// *********************************************************************
// Helper Functions/Classes for This Test Program
// *********************************************************************


// class TypeCheck
// This class exists in order to have static member function "check",
// which takes a parameter of a given type, by reference. Objects of
// type TypeCheck<T> cannot be created.
// Usage:
//     TypeCheck<MyType>::check(x)
//      returns true if the type of x is (MyType) or (const MyType),
//      otherwise false.
// Invariants: None.
// Requirements on Types: None.
template<typename T>
class TypeCheck {

private:

    // No default ctor
    TypeCheck() = delete;

    // Uncopyable. Do not define copy/move ctor/assn.
    TypeCheck(const TypeCheck &) = delete;
    TypeCheck(TypeCheck &&) = delete;
    TypeCheck<T> & operator=(const TypeCheck &) = delete;
    TypeCheck<T> & operator=(TypeCheck &&) = delete;

    // Compiler-generated dctor is used (but irrelevant).
    ~TypeCheck() = default;

public:

    // check
    // The function and function template below simulate a single
    // function that takes a single parameter, and returns true iff the
    // parameter has type T or (const T).

    // check (reference-to-const T)
    // Pre: None.
    // Post:
    //     Return is true.
    // Does not throw (No-Throw Guarantee)
    static bool check([[maybe_unused]] const T & param)
    {
        return true;
    }

    // check (reference-to-const non-T)
    // Pre: None.
    // Post:
    //     Return is false.
    // Requirements on types: None.
    // Does not throw (No-Throw Guarantee)
    template <typename OtherType>
    static bool check([[maybe_unused]] const OtherType & param)
    {
        return false;
    }

};  // End class TypeCheck


// *********************************************************************
// Test Cases
// *********************************************************************


TEST_CASE( "Small Instances" )
{
    vector<int> v;              // Given sequence
    size_t expected;            // Expected return value

    // Empty sequence
    {
    v.clear();
    expected = size_t(0);
    INFO( "Empty Sequence" );
    REQUIRE(inversions(begin(v), end(v)) == expected);
    }

    // Singleton
    {
    v = vector<int> { 5 };
    expected = size_t(0);
    INFO( "Singleton" );
    REQUIRE(inversions(begin(v), end(v)) == expected);
    }

    // Size 2, ascending
    {
    v = vector<int> { 3, 7 };
    expected = size_t(0);
    INFO( "Size 2, ascending" );
    REQUIRE(inversions(begin(v), end(v)) == expected);
    }

    // Size 2, descending
    {
    v = vector<int> { 7, 3 };
    expected = size_t(1);
    INFO( "Size 2, decending" );
    REQUIRE(inversions(begin(v), end(v)) == expected);
    }

    // Size 3, constant
    {
    v = vector<int> { 4, 4, 4 };
    expected = size_t(0);
    INFO( "Size 3, constant" );
    REQUIRE(inversions(begin(v), end(v)) == expected);
    }

    // Size 3, ascending
    {
    v = vector<int> { 3, 4, 5 };
    expected = size_t(0);
    INFO( "Size 3, ascending" );
    REQUIRE(inversions(begin(v), end(v)) == expected);
    }

    // Size 3, 1 inversion
    {
    v = vector<int> { 3, 5, 4 };
    expected = size_t(1);
    INFO( "Size 3, 1 inversion" );
    REQUIRE(inversions(begin(v), end(v)) == expected);
    }

    // Size 3, 2 inversions
    {
    v = vector<int> { 5, 3, 4 };
    expected = size_t(2);
    INFO( "Size 3, 2 inversions" );
    REQUIRE(inversions(begin(v), end(v)) == expected);
    }

    // Size 3, 3 inversions
    {
    v = vector<int> { 5, 4, 3 };
    expected = size_t(3);
    INFO( "Size 3, 3 inversions" );
    REQUIRE(inversions(begin(v), end(v)) == expected);
    }
}


TEST_CASE( "Large Instances" )
{
    vector<int> v;              // Given sequence
    size_t expected;            // Expected return value

    // Large, blocks of values
    {
    v.clear();
    const int N = 10000;
    for (int i = 0; i < N; ++i)
    {
        v.push_back(2);
    }
    for (int i = 0; i < N; ++i)
    {
        v.push_back(1);
    }
    expected = size_t(N*N);
    INFO( "Large, blocks of values");
    REQUIRE(inversions(begin(v), end(v)) == expected);
    }

    // Large, ascending
    {
    v.clear();
    const int N = 10000;
    for (int i = 0; i < N; ++i)
    {
        v.push_back(i);
    }
    expected = size_t(0);
    INFO( "Large, ascending");
    REQUIRE(inversions(begin(v), end(v)) == expected);
    }

    // Large, descending
    {
    v.clear();
    const int N = 10000;
    for (int i = 0; i < N; ++i)
    {
        v.push_back(N-i);
    }
    expected = size_t(N*(N-1)/2);
    INFO( "Large, descending");
    REQUIRE(inversions(begin(v), end(v)) == expected);
    }

    // Large, pairs ascending
    {
    v.clear();
    const int N = 10000;
    for (int i = 0; i < N; ++i)
    {
        v.push_back(i);
        v.push_back(i);
    }
    expected = size_t(0);
    INFO( "Large, pairs ascending");
    REQUIRE(inversions(begin(v), end(v)) == expected);
    }

    // Large, pairs descending
    {
    v.clear();
    const int N = 10000;
    for (int i = 0; i < N; ++i)
    {
        v.push_back(N-i);
        v.push_back(N-i);
    }
    expected = size_t(N*(N-1)*2);
    INFO( "Large, pairs descending");
    REQUIRE(inversions(begin(v), end(v)) == expected);
    }

    // Large, ascending & descending
    {
    v.clear();
    const int N = 4;
    for (int i = 0; i < N; ++i)
    {
        v.push_back(i);
        v.push_back(N-i);
    }
    expected = size_t(N*(N-1));
    INFO( "Large, ascending & descending");
    REQUIRE(inversions(begin(v), end(v)) == expected);
    }
}


TEST_CASE( "Types" )
{
    vector<int> v;              // Given sequence
    size_t expected;            // Expected return value

    // Return Type
    {
    v.clear();
    [[maybe_unused]] size_t && rr = inversions(begin(v), end(v));
    INFO( "Return Type" );
    REQUIRE(TypeCheck<size_t>::check(inversions(begin(v), end(v))));
    }

    // Fancy Iterators
    {
    std::deque<int> d;              // Given sequence;
                                    //  will be passed reversed
    const int N = 100;
    for (int i = 0; i < N; ++i)
    {
        d.push_front(i);
        d.push_front(N-i);
    }
    expected = size_t(N*(N-1));
    INFO( "Fancy Iterators" );
    REQUIRE(inversions(rbegin(d), rend(d)) == expected);
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

