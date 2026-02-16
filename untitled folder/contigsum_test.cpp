// contigsum_test.cpp
// Glenn G. Chappell
// 2025-09-29
//
// For CS 411 Fall 2025
// Test program for function template contigSum
// Used in Assignment 3, Exercise A

// Includes for code to be tested
#include "contigsum.hpp"     // For function template contigsum
#include "contigsum.hpp"     // Double-inclusion check, for testing only

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
    "function template contigSum"
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
    const vector<int> & vc(v);  // Const ref to above
    int expected;               // Expected result

    // Empty sequence
    {
    v.clear();
    expected = 0;
    INFO( "Empty sequence" );
    REQUIRE(contigSum(begin(vc), end(vc)) == expected);
    }

    // Singleton, positive
    {
    v = vector<int> { 5 };
    expected = 5;
    INFO( "Size 1, positive" );
    REQUIRE(contigSum(begin(vc), end(vc)) == expected);
    }

    // Singleton, negative
    {
    v = vector<int> { -3 };
    expected = 0;
    INFO( "Size 1, negative" );
    REQUIRE(contigSum(begin(vc), end(vc)) == expected);
    }

    // Singleton, zero
    {
    v = vector<int> { 0 };
    expected = 0;
    INFO( "Size 1, zero" );
    REQUIRE(contigSum(begin(vc), end(vc)) == expected);
    }

    // Size 2, pos pos
    {
    v = vector<int> { 5, 3 };
    expected = 8;
    INFO( "Size 2, positive positive" );
    REQUIRE(contigSum(begin(vc), end(vc)) == expected);
    }

    // Size 2, pos neg
    {
    v = vector<int> { 6, -4 };
    expected = 6;
    INFO( "Size 2, positive negative" );
    REQUIRE(contigSum(begin(vc), end(vc)) == expected);
    }

    // Size 2, neg pos
    {
    v = vector<int> { -2, 8 };
    expected = 8;
    INFO( "Size 2, negative positive" );
    REQUIRE(contigSum(begin(vc), end(vc)) == expected);
    }

    // Size 2, neg neg
    {
    v = vector<int> { -7, -1 };
    expected = 0;
    INFO( "Size 2, negative negative" );
    REQUIRE(contigSum(begin(vc), end(vc)) == expected);
    }

    // Size 3 #1
    {
    v = vector<int> { 6, -5, 2 };
    expected = 6;
    INFO( "Size 3, test #1" );
    REQUIRE(contigSum(begin(vc), end(vc)) == expected);
    }

    // Size 3 #2
    {
    v = vector<int> { 1, -3, 8 };
    expected = 8;
    INFO( "Size 3, test #2" );
    REQUIRE(contigSum(begin(vc), end(vc)) == expected);
    }

    // Size 3 #3
    {
    v = vector<int> { 6, -5, 7 };
    expected = 8;
    INFO( "Size 3, test #3" );
    REQUIRE(contigSum(begin(vc), end(vc)) == expected);
    }

    // Size 20 #1
    {
    v = vector<int> { 6, -5, 0, 7, 11, -6, -1, 9, 12, -7,
                      5, 8, 2, 4, 10, -4, 3, 1, -2, -3 };
    expected = 55;
    INFO( "Size 20, test #1" );
    REQUIRE(contigSum(begin(vc), end(vc)) == expected);
    }

    // Size 20 #2
    {
    v = vector<int> { 5, -2, 1, 12, 10, -1, 3, 4, 8, 11,
                      6, -5, -7, -3, 9, 7, 0, -4, -6, 2 };
    expected = 58;
    INFO( "Size 20, test #2" );
    REQUIRE(contigSum(begin(vc), end(vc)) == expected);
    }

    // Size 20 #3
    {
    v = vector<int> { 7, -1, 8, 0, 5, 1, 10, 2, -7, 3,
                     -5, 11, 12, -4, 9, 6, 4, -3, -2, -6 };
    expected = 61;
    INFO( "Size 20, test #3" );
    REQUIRE(contigSum(begin(vc), end(vc)) == expected);
    }
}


TEST_CASE( "Large Instances" )
{
    vector<int> v;              // Given sequence
    const vector<int> & vc(v);  // Const ref to above
    int expected;               // Expected return value

    // Large #1
    {
    v.clear();
    const int N = 100000;
    for (int i = 0; i < N; ++i)
    {
        v.push_back(2);
        v.push_back(-3);
    }
    for (int i = 0; i < N; ++i)
    {
        v.push_back(5);
        v.push_back(-4);
        v.push_back(5);
    }
    for (int i = 0; i < N; ++i)
    {
        v.push_back(-2);
        v.push_back(1);
    }
    expected = 6*N;
    INFO( "Large #1" );
    REQUIRE(contigSum(begin(vc), end(vc)) == expected);
    }

    // Large #2
    {
    v.clear();
    const int N = 100000;
    for (int i = 0; i < N; ++i)
    {
        v.push_back(2);
        v.push_back(-3);
    }
    for (int i = 0; i < N; ++i)
    {
        v.push_back(6);
        v.push_back(-4);
    }
    for (int i = 0; i < N; ++i)
    {
        v.push_back(-2);
        v.push_back(1);
    }
    expected = 2*N+4;
    INFO( "Large #2" );
    REQUIRE(contigSum(begin(vc), end(vc)) == expected);
    }

    // Large #3
    {
    v.clear();
    const int N = 100000;
    for (int i = 0; i < N; ++i)
    {
        v.push_back(5);
    }
    for (int i = 0; i < N; ++i)
    {
        v.push_back(-6);
    }
    for (int i = 0; i < N; ++i)
    {
        v.push_back(7);
    }
    expected = 7*N;
    INFO( "Large #3" );
    REQUIRE(contigSum(begin(vc), end(vc)) == expected);
    }

    // Large #4
    {
    v.clear();
    const int N = 100000;
    for (int i = 0; i < N; ++i)
    {
        v.push_back(8);
    }
    for (int i = 0; i < N; ++i)
    {
        v.push_back(-6);
    }
    for (int i = 0; i < N; ++i)
    {
        v.push_back(7);
    }
    expected = 9*N;
    INFO( "Large #4" );
    REQUIRE(contigSum(begin(vc), end(vc)) == expected);
    }
}


TEST_CASE( "Types" )
{
    vector<int> v;              // Given sequence
    const vector<int> & vc(v);  // Const ref to above
    int expected;               // Expected return value

    // Return Type
    {
    v.clear();
    [[maybe_unused]] int && rr = contigSum(begin(vc), end(vc));
    INFO( "Return Type" );
    REQUIRE(TypeCheck<int>::check(contigSum(begin(vc), end(vc))));
    }

    // Fancy Iterators
    {
    std::deque<int> d;              // Given sequence;
                                    //  will be passed reversed
    const std::deque<int> & dc(d);  // Const ref to above
    const int N = 100;
    for (int i = 0; i < N; ++i)
    {
        d.push_back(2);
        d.push_back(-3);
    }
    for (int i = 0; i < N; ++i)
    {
        d.push_back(5);
        d.push_back(-4);
        d.push_back(5);
    }
    for (int i = 0; i < N; ++i)
    {
        d.push_back(-2);
        d.push_back(1);
    }
    expected = 6*N;
    INFO( "Fancy Iterators" );
    REQUIRE(contigSum(begin(dc), end(dc)) == expected);
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

