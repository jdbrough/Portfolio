// unionfind_main.cpp
// Glenn G. Chappell
// 2025-11-03
//
// For CS 411 Fall 2025
// Simple main program for class UnionFind
// Requires unionfind.hpp

#include "unionfind.hpp"  // for class UnionFind
#include <iostream>
using std::cout;
using std::cin;


void find_with_result(UnionFind & uf,
                      size_t vert)
{
    cout << "Find(" << vert << "): " << uf.find(vert) << "\n";
}


// userPause
// Wait for user to press ENTER: read all chars through first newline.
void userPause()
{
    std::cout.flush();
    while (std::cin.get() != '\n') ;
}


// Main program
// Use all operations for class UnionFind.
int main()
{
    UnionFind uf;  // Our Union-Find object

    // Do some operations
    cout << "Doing Make-Set: 0 1 2 3 4 5 6 7 8 9\n";
    uf.makeSet(0);
    uf.makeSet(1);
    uf.makeSet(2);
    uf.makeSet(3);
    uf.makeSet(4);
    uf.makeSet(5);
    uf.makeSet(6);
    uf.makeSet(7);
    uf.makeSet(8);
    uf.makeSet(9);
    cout << "\n";

    cout << "Doing Union: (0,6) (2,7) (5,8) (9,0) (1,3) (5,0) (4,7)\n";
    uf.unionx(0, 6);
    uf.unionx(2, 7);
    uf.unionx(5, 8);
    uf.unionx(9, 0);
    uf.unionx(1, 3);
    uf.unionx(5, 0);
    uf.unionx(4, 7);
    cout << "\n";

    // Check values of find
    cout << "The following should all be equal:\n";
    find_with_result(uf, 0);
    find_with_result(uf, 5);
    find_with_result(uf, 6);
    find_with_result(uf, 8);
    find_with_result(uf, 9);
    cout << "\n";

    cout << "These should also be equal, but different from above:\n";
    find_with_result(uf, 2);
    find_with_result(uf, 4);
    find_with_result(uf, 7);
    cout << "\n";

    cout << "Again, equal, different from each of the above:\n";
    find_with_result(uf, 1);
    find_with_result(uf, 3);
    cout << "\n";

    // Wait for user
    cout << "Press ENTER to quit ";
    userPause();
}

