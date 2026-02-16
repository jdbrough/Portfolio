// holeydtcount.cpp
// Jonathan Brough
// 2024-10-3
//
// For CS 311 Fall 2024
// Source file for holeydtcount

#include "holeydtcount.hpp"
#include <vector>

using std::vector;

// Function to convert 2D coordinates (x, y) to a 1D index
int index(int x, int y, int height) {
    return x * height + y;
}

// Check if the square at (x, y) is available
bool checkSquare(const vector<int> &board, int x, int y, int width, int height) {
    if (x >= 0 && x < width && y >= 0 && y < height)
        return (board[size_t(index(x, y, height))] == 0);
    return false;
}

// Recursive function to count valid tilings
int holeyDTCount_recurse(vector<int> &board, int width, int height, int squaresLeft, int currentIndex) {
    // Base case: if no squares left, we found a valid tiling
    if (squaresLeft == 0)
        return 1;

    int total = 0;
    int totalSize = width * height;

    // Find the first uncovered square starting from currentIndex
    while (currentIndex < totalSize && board[currentIndex] != 0) {
        ++currentIndex;
    }

    // If we've covered all squares, return 0 as there are no more empty squares to process
    if (currentIndex >= totalSize) {
        return 0;
    }

    // Calculate the 2D coordinates from the currentIndex
    int x = currentIndex / height;
    int y = currentIndex % height;

    // Try placing a horizontal domino
    if (checkSquare(board, x + 1, y, width, height)) {
        // Place the domino
        board[size_t(index(x, y, height))] = 1;
        board[size_t(index(x + 1, y, height))] = 1;

        // Recurse
        total += holeyDTCount_recurse(board, width, height, squaresLeft - 2, currentIndex + 1);

        // Backtrack
        board[size_t(index(x, y, height))] = 0;
        board[size_t(index(x + 1, y, height))] = 0;
    }

    // Try placing a vertical domino
    if (checkSquare(board, x, y + 1, width, height)) {
        // Place the domino
        board[size_t(index(x, y, height))] = 1;
        board[size_t(index(x, y + 1, height))] = 1;

        // Recurse
        total += holeyDTCount_recurse(board, width, height, squaresLeft - 2, currentIndex + 1);

        // Backtrack
        board[size_t(index(x, y, height))] = 0;
        board[size_t(index(x, y + 1, height))] = 0;
    }

    return total;
}

// Main function to set up the board and start the recursive process
int holeyDTCount(int width, int height, int hole1_x, int hole1_y, int hole2_x, int hole2_y) {
    // Create the board of size (width * height), initialized with 0
    vector<int> board(size_t(width * height), 0);

    // Set the holes in the vector
    board[size_t(index(hole1_x, hole1_y, height))] = 1;
    board[size_t(index(hole2_x, hole2_y, height))] = 1;

    // Total squares minus the two holes
    int totalSquares = (width * height) - 2;

    // Shortcut: If the number of available squares is odd, return 0 (cannot be fully tiled)
    if (totalSquares % 2 != 0) {
        return 0;
    }

    // Call the recursive function starting from index 0
    return holeyDTCount_recurse(board, width, height, totalSquares, 0);
}