// holeydtcount.hpp
// Jonathan Brough
// 2024-10-3
//
// For CS 311 Fall 2024
// Header file for holeydtcount

#ifndef HOLEYDTCOUNT_HPP
#define HOLEYDTCOUNT_HPP

#include <vector>

// Function: holeyDTCount
// Counts the number of holey domino tilings on a board with specified dimensions
// and two specified holes.
//
// Precondition:
// - dim_x > 0, dim_y > 0 (board dimensions must be positive)
// - 0 <= hole1_x < dim_x, 0 <= hole1_y < dim_y (first hole must be within the board)
// - 0 <= hole2_x < dim_x, 0 <= hole2_y < dim_y (second hole must be within the board)
// - The two holes must be distinct.
//
// Parameters:
// - dim_x: Width of the board (number of columns)
// - dim_y: Height of the board (number of rows)
// - hole1_x: X-coordinate of the first hole
// - hole1_y: Y-coordinate of the first hole
// - hole2_x: X-coordinate of the second hole
// - hole2_y: Y-coordinate of the second hole
//
// Returns:
// - The number of distinct ways to tile the board with dominoes,
//   given the holes and board dimensions.
int holeyDTCount(int dim_x, int dim_y,
                 int hole1_x, int hole1_y,
                 int hole2_x, int hole2_y);

#endif // HOLEYDTCOUNT_HPP