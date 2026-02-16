// unionfind.hpp
// VERSION 2
// Glenn G. Chappell
// Started: 2025-11-03
// Updated: 2025-11-05
//
// For CS 411 Fall 2025
// Header for class UnionFind
// Union-Find Structure
// There is no associated source file
//
// History:
// - v1: All functionality working.
// - v2: Added optimizations: union by rank, path compression.

#ifndef FILE_UNIONFIND_HPP_INCLUDED
#define FILE_UNIONFIND_HPP_INCLUDED

#include <vector>
using std::vector;
#include <cstddef>
using std::size_t;
#include <cassert>
// for assert


// class UnionFind
// Union-Find structure.
// "Quick union" / Disjoint Path Forest implementation, with union by
// rank and path-compression optimizations.
//
// Items are size_t values.
// Member functions:
// - makeSet (Make-Set operation)
// - find    (Find operation)
// - unionx  (Union operation)
class UnionFind {

// ***** UnionFind: ctors, dctor, op= *****
public:

    // Compiler generated default ctor, copy ctor, copy =, dctor used

// ***** UnionFind: general public functions *****
public:

    // makeSet
    // Make-Set operation.
    // Pre:
    //     x >= 0.
    //     makeSet must not have been previously called on x.
    void makeSet(size_t x)
    {
        if (x >= _data.size())
        {
            _data.resize(x+1, Info(false, 0, 0));
        }

        assert(!_data[x]._inited);
        _data[x] = Info(true, x, 0);
    }

    // find
    // Find operation.
    // Pre:
    //     x >= 0.
    //     makeSet must have been previously called on x.
    size_t find(size_t x)
    {
        assert(x < _data.size());
        assert(_data[x]._inited);

        // Do path compression
        if (_data[x]._parent != x)
        {
            _data[x]._parent = find(_data[x]._parent);
        }
        return _data[x]._parent;
    }

    // unionx
    // Union operation ("union" is a reserved word in C++).
    // Pre:
    //    x >= 0.
    //    y >= 0.
    //    makeSet must have been previously called on each of x, y.
    void unionx(size_t x,
                size_t y)
    {
        assert(x < _data.size());
        assert(_data[x]._inited);
        assert(y < _data.size());
        assert(_data[y]._inited);

        size_t xroot = find(x);
        assert(_data[xroot]._parent == xroot);
        size_t yroot = find(y);
        assert(_data[yroot]._parent == yroot);
        if (xroot == yroot)
        {
            return;
        }

        // Do union by rank
        if (_data[xroot]._rank < _data[yroot]._rank)
        {
            _data[xroot]._parent = yroot;
        }
        else if (_data[yroot]._rank < _data[xroot]._rank)
        {
            _data[yroot]._parent = xroot;
        }
        else
        {
            _data[yroot]._parent = xroot;
            ++_data[xroot]._rank;
        }
    }

// ***** UnionFind: internal-use types *****
private:

    // struct Info
    // Info on single item in union-find structure
    struct Info {
        bool _inited;     // Has Make-Set been called on this item?
                          //  _parent, _rank fields invalid if !_inited
        size_t  _parent;  // Parent of item
        size_t  _rank;    // Tree rank (height); only valid for root

        // Ctor from field values, for convenience
        Info(bool theInited,
             size_t theParent,
             size_t theRank)
           :_inited(theInited),
            _parent(theParent),
            _rank(theRank)
        {}
    };

// ***** UnionFind: data members *****
private:

    vector<Info> _data;  // Info for each item

};  // End class UnionFind


#endif //#ifndef FILE_UNIONFIND_HPP_INCLUDED

