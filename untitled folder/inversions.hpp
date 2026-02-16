// inversions.hpp
// Jonathan Brough
// 2025-10-6
//
// For CS 411 Fall 2025
// Stable merge-sort counting inversions
// Assignment 3, Exercise B

#ifndef FILE_INVERSIONS_HPP_INCLUDED
#define FILE_INVERSIONS_HPP_INCLUDED

#include <vector>
#include <iterator>
#include <cstddef>

// inversions
// Sorts [first,last) and returns the number of inversions using
// a merge-sort style recursive count during merge.
template <typename RAIter>
std::size_t inversions(RAIter first, RAIter last)
{
    using diff_t  = typename std::iterator_traits<RAIter>::difference_type;
    using value_t = typename std::iterator_traits<RAIter>::value_type;

    diff_t n = last - first;
    if (n <= 1) return std::size_t(0);

    std::vector<value_t> buf(static_cast<std::size_t>(n));

    // sort_count: sorts [lo,hi) and returns inversion count
    auto sort_count = [&](auto&& self, diff_t lo, diff_t hi) -> std::size_t
    {
        diff_t len = hi - lo;
        if (len <= 1) return std::size_t(0);

        diff_t mid = lo + len/2;
        std::size_t inv = self(self, lo, mid) + self(self, mid, hi);

        diff_t i = lo, j = mid, k = lo;
        while (i < mid && j < hi) {
            if (*(first + i) <= *(first + j)) {
                buf[k++] = *(first + i++);
            } else {
                buf[k++] = *(first + j++);
                inv += static_cast<std::size_t>(mid - i); // cross inversions
            }
        }
        while (i < mid) buf[k++] = *(first + i++);
        while (j < hi)  buf[k++] = *(first + j++);

        for (diff_t t = lo; t < hi; ++t)
            *(first + t) = buf[t];

        return inv;
    };

    return sort_count(sort_count, diff_t(0), n);
}

#endif // FILE_INVERSIONS_HPP_INCLUDED