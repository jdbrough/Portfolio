// bridges.hpp
// Jonathan Brough
// 2025-09-24
//
// For CS 411 Fall 2025
// Declaration for max-toll selection 

#ifndef BRIDGES_HPP
#define BRIDGES_HPP

#include <vector>

using Bridge = std::vector<int>;  // {west, east, toll}

int bridges(int w, int e, const std::vector<Bridge>& bridges);

#endif