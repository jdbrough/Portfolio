// da4.hpp
// Jonathan Brough
// 2024-09-19
// Updated: 2024-09-26
//
// For CS 311 Fall 2024
// Header for Assignment 3 functions

#ifndef FILE_DA3_HPP_INCLUDED
#define FILE_DA3_HPP_INCLUDED

#include "llnode.hpp"  // For LLNode
#include <cstddef>     // For std::size_t
#include <functional>  // For std::function
#include <stdexcept>   // For std::out_of_range


// Exercise A — Linked List Lookup
template <typename ValueType>
ValueType lookup(const LLNode<ValueType> *head, std::size_t index)
{
    const LLNode<ValueType> *current = head;
    std::size_t count = 0;

    while (current != nullptr)
    {
        if (count == index)
        {
            return current->_data;
        }
        current = current->_next;
        ++count;
    }

    // If index is out of range
    throw std::out_of_range("index value is out of the list's range");
}


// Exercise B — Did It Throw (Implementation in source file)
void didItThrow(const std::function<void()> & ff,
                bool & threw);


// Exercise C — Check Sorted
template <typename FDIter>
bool checkSorted(FDIter first, FDIter last)
{
    if (first == last) return true; // An empty range is sorted by default

    FDIter next = first;
    ++next;

    while (next != last)
    {
        if (*next < *first)
        {
            return false;
        }
        ++first;
        ++next;
    }
    return true;
}


// Exercise D — Recursive GCD (Implementation in source file)
int gcd(int a,
        int b);

#endif  //#ifndef FILE_DA3_HPP_INCLUDED