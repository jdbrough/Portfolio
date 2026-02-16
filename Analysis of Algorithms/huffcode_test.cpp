// huffcode_test.cpp
// Glenn G. Chappell
// 2025-11-10
//
// For CS 411 Fall 2025
// Test program for class HuffCode
// Used in Assignment 6, Exercise A

// Includes for code to be tested
#include "huffcode.hpp"      // For class HuffCode
#include "huffcode.hpp"      // Double-inclusion check, for testing only

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
#include <unordered_map>
using std::unordered_map;
#include <vector>
using std::vector;
#include <cstddef>
using std::size_t;

// Printable name for this test suite
const string test_suite_name =
    "class HuffCode"
    " - CS 411 Assn 6, Ex A";


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


// matchPattern
// Pattern matching for Huffman-coded strings
bool matchPattern(const string & pattern,
                  const string & coded)
{
    size_t len = pattern.size();
    if (len != coded.size())
        return false;

    unordered_map<char, char> m;
    for (size_t i = 0; i != len; ++i)
    {
        char tc = coded[i];
        if (tc != '0' && tc != '1')
            return false;

        char pc = pattern[i];
        bool islower =  (pc >= 'a');
        char pcupper = islower ? char(pc - 'a' + 'A') : pc;

        char tcbase = islower ? char('1'-(tc-'0')) : tc;

        if (m.find(pcupper) != m.end())
        {
            if (tcbase != m[pcupper])
                return false;
        }
        else
        {
            m[pcupper] = tcbase;
        }
    }
    return true;
}


// *********************************************************************
// Test Cases
// *********************************************************************


TEST_CASE( "class HuffCode - Types" )
{
    HuffCode h;
    unordered_map<char, int> w;
    w['a'] = 1;
    w['b'] = 2;
    const unordered_map<char, int> & cw(w);
    h.setWeights(cw);
    const HuffCode & ch(h);
    const string s = "";

    // Return type of encode
    {
    [[maybe_unused]] string && dummy = ch.encode(s);
    INFO( "Return type of encode is string" );
    REQUIRE( TypeCheck<string>::check(ch.encode(s)) );
    }

    // Return type of decode
    {
    [[maybe_unused]] string && dummy = ch.decode(s);
    INFO( "Return type of decode is string" );
    REQUIRE( TypeCheck<string>::check(ch.decode(s)) );
    }
}


TEST_CASE( "class HuffCode - simple code" )
{
    HuffCode h;
    unordered_map<char, int> w;
    w['a'] = 3;
    w['b'] = 2;
    w['c'] = 1;
    const unordered_map<char, int> & cw(w);
    h.setWeights(cw);

    string text, result, expected, pat;

    // Encode empty text
    {
    text = "";
    expected = "";
    result = h.encode(text);
    INFO( "Encode empty text" );
    REQUIRE( result == expected );
    }

    // Encode all characters
    {
    text = "abc";
    result = h.encode(text);
    pat = "aAbAB";
    INFO( "Encode all characters" );
    REQUIRE( matchPattern(pat, result) );
    }

    vector<string> cwd;
    cwd.push_back(result.substr(0, 1));
    cwd.push_back(result.substr(1, 2));
    cwd.push_back(result.substr(3, 2));

    // Encode single characters
    for (size_t i = 0; i != cwd.size(); ++i)
    {
        text = string(1, 'a'+char(i));
        expected = cwd[i];
        result = h.encode(text);
        INFO( "Encode single character: '" << text << "'");
        REQUIRE( result == expected );
    }

    // Encode longer text
    {
    text = "aaabacbabbbccacbcc";
    expected = cwd[0]+cwd[0]+cwd[0]+cwd[1]+cwd[0]+cwd[2]
              +cwd[1]+cwd[0]+cwd[1]+cwd[1]+cwd[1]+cwd[2]
              +cwd[2]+cwd[0]+cwd[2]+cwd[1]+cwd[2]+cwd[2];
    result = h.encode(text);
    INFO( "Encode longer text" );
    REQUIRE( result == expected );
    }

    // Decode empty text
    {
    text = "";
    expected = "";
    result = h.decode(text);
    INFO( "Decode empty text" );
    REQUIRE( result == expected );
    }

    // Decode single characters
    for (size_t i = 0; i != cwd.size(); ++i)
    {
        text = cwd[i];
        expected = string(1, 'a'+char(i));
        result = h.decode(text);
        INFO( "Decode codeword for single char: '" << text << "'" );
        REQUIRE( result == expected );
    }

    // Decode longer text
    {
    text = cwd[0]+cwd[1]+cwd[2]+cwd[0]+cwd[2]+cwd[1]
          +cwd[1]+cwd[0]+cwd[2]+cwd[1]+cwd[2]+cwd[0]
          +cwd[2]+cwd[0]+cwd[1]+cwd[2]+cwd[1]+cwd[0]
          +cwd[0]+cwd[0]+cwd[0]+cwd[1]+cwd[1]+cwd[1]
          +cwd[2]+cwd[2]+cwd[2];
    expected = "abcacbbacbcacabcbaaaabbbccc";
    result = h.decode(text);
    INFO( "Decode longer text" );
    REQUIRE( result == expected );
    }
}

TEST_CASE( "class HuffCode - complex code" )
{
    HuffCode h;
    unordered_map<char, int> w;
    w['a'] = 2;
    w['b'] = 20;
    w['c'] = 3;
    w['d'] = 8;
    w['e'] = 12;
    w['f'] = 3;
    w['g'] = 8;
    w['h'] = 2;
    w['i'] = 7;
    w['j'] = 5;
    const unordered_map<char, int> & cw(w);
    h.setWeights(cw);

    string text, result, expected, pat;

    // Encode empty text
    {
    text = "";
    expected = "";
    result = h.encode(text);
    INFO( "Encode empty text" );
    REQUIRE( result == expected );
    }

    // Encode all characters
    {
    text = "abcdefghij";
    result = h.encode(text);
    pat = "ihFcAiHIGDbIgEihfIGDBIgeihFcaIGdihFC";
    INFO( "Encode all characters" );
    REQUIRE( matchPattern(pat, result) );
    }

    vector<string> cwd;
    cwd.push_back(result.substr(0, 5));
    cwd.push_back(result.substr(5, 2));
    cwd.push_back(result.substr(7, 4));
    cwd.push_back(result.substr(11, 3));
    cwd.push_back(result.substr(14, 3));
    cwd.push_back(result.substr(17, 4));
    cwd.push_back(result.substr(21, 3));
    cwd.push_back(result.substr(24, 5));
    cwd.push_back(result.substr(29, 3));
    cwd.push_back(result.substr(32, 4));

    // Encode single characters
    for (size_t i = 0; i != cwd.size(); ++i)
    {
        text = string(1, 'a'+char(i));
        expected = cwd[i];
        result = h.encode(text);
        INFO( "Encode single character: '" << text << "'");
        REQUIRE (result == expected );
    }

    // Encode longer text
    {
    text = "ajabcffhcdihgjihgfedcba";
    expected = cwd[0]+cwd[9]+cwd[0]+cwd[1]+cwd[2]+cwd[5]+cwd[5]+cwd[7]
              +cwd[2]+cwd[3]+cwd[8]+cwd[7]+cwd[6]+cwd[9]+cwd[8]+cwd[7]
              +cwd[6]+cwd[5]+cwd[4]+cwd[3]+cwd[2]+cwd[1]+cwd[0];
    result = h.encode(text);
    INFO( "Encode longer text" );
    REQUIRE( result == expected );
    }

    // Decode empty text
    {
    text = "";
    expected = "";
    result = h.decode(text);
    INFO( "Decode empty text" );
    REQUIRE( result == expected );
    }

    // Decode single characters
    for (size_t i = 0; i != cwd.size(); ++i)
    {
        text = cwd[i];
        expected = string(1, 'a'+char(i));
        result = h.decode(text);
        INFO( "Decode codeword for single char: '" << text << "'" );
        REQUIRE( result == expected );
    }

    // Decode longer text
    {
    text = cwd[6]+cwd[9]+cwd[0]+cwd[8]+cwd[9]+cwd[9]+cwd[3]+cwd[3]
          +cwd[3]+cwd[4]+cwd[8]+cwd[9]+cwd[2]+cwd[6]+cwd[1]+cwd[7]
          +cwd[4]+cwd[0]+cwd[5]+cwd[1]+cwd[5]+cwd[0]+cwd[2]+cwd[8]
          +cwd[8]+cwd[8];
    expected = "gjaijjdddeijcgbheafbfaciii";
    result = h.decode(text);
    INFO( "Decode longer text" );
    REQUIRE( result == expected );
    }
}


TEST_CASE( "class Huffcode - copying HuffCode object" )
{
    HuffCode h;
    unordered_map<char, int> w;
    w['a'] = 3;
    w['b'] = 2;
    w['c'] = 1;
    const unordered_map<char, int> & cw(w);
    h.setWeights(cw);
    const HuffCode ch(h);

    string text, result, expected, pat;

    // Encode all characters
    {
    text = "abc";
    result = ch.encode(text);
    pat = "aAbAB";
    INFO( "Copy, encode all characters" );
    REQUIRE( matchPattern(pat, result) );
    }

    vector<string> cwd;
    cwd.push_back(result.substr(0, 1));
    cwd.push_back(result.substr(1, 2));
    cwd.push_back(result.substr(3, 2));

    // Encode text
    {
    text = "aaabacbabbbccacbcc";
    expected = cwd[0]+cwd[0]+cwd[0]+cwd[1]+cwd[0]+cwd[2]
              +cwd[1]+cwd[0]+cwd[1]+cwd[1]+cwd[1]+cwd[2]
              +cwd[2]+cwd[0]+cwd[2]+cwd[1]+cwd[2]+cwd[2];
    result = ch.encode(text);
    INFO( "Copy, encode text" );
    REQUIRE( result == expected );
    }

    // Decode text
    {
    text = cwd[0]+cwd[1]+cwd[2]+cwd[0]+cwd[2]+cwd[1]
          +cwd[1]+cwd[0]+cwd[2]+cwd[1]+cwd[2]+cwd[0]
          +cwd[2]+cwd[0]+cwd[1]+cwd[2]+cwd[1]+cwd[0]
          +cwd[0]+cwd[0]+cwd[0]+cwd[1]+cwd[1]+cwd[1]
          +cwd[2]+cwd[2]+cwd[2];
    expected = "abcacbbacbcacabcbaaaabbbccc";
    result = ch.decode(text);
    INFO( "Copy, decode text" );
    REQUIRE( result == expected );
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

