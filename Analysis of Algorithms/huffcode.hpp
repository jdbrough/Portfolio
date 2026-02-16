// huffcode.hpp
// Jonathan Brough
// 2025-11-20
//
// For CS 411 Fall 2025
// Assignment 6, Exercise A
// Header for class HuffCode

#ifndef FILE_HUFFCODE_HPP_INCLUDED
#define FILE_HUFFCODE_HPP_INCLUDED

#include <string>
#include <unordered_map>
#include <memory>

// Class HuffCode
// Encoding & decoding using a Huffman code
class HuffCode {

private:
    // Huffman tree node
    struct HuffNode {
        std::string data;
        int weight;
        std::shared_ptr<HuffNode> left;
        std::shared_ptr<HuffNode> right;

        HuffNode(const std::string & d,
                 int w,
                 std::shared_ptr<HuffNode> l = nullptr,
                 std::shared_ptr<HuffNode> r = nullptr)
            : data(d), weight(w), left(l), right(r)
        {}
    };

public:
    // Compiler-generated ctor, copy ctor, copy assignment, dctor are fine

    void setWeights(const std::unordered_map<char, int> & theweights);

    std::string encode(const std::string & text) const;

    std::string decode(const std::string & codestr) const;

private:
    // Map from character to its Huffman code
    std::unordered_map<char, std::string> code;

    // Pointer to root of Huffman tree
    std::shared_ptr<HuffNode> head = nullptr;

};  // end class HuffCode

#endif