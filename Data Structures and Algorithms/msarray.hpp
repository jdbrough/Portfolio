// msarray.hpp
// Jonathan D. Brough
// Started: 2024-09-16
// Updated: 2024-09-19
//
// For CS 311 Fall 2024
// Header for class MSArray

#ifndef MSARRAY_HPP
#define MSARRAY_HPP

#include <cstddef>   // For std::size_t
#include <utility>   // For std::swap, std::copy, std::fill
#include <algorithm> // For std::equal, std::lexicographical_compare

// Invariants:
//  _length >= 0
template <typename ValType>
class MSArray {
public:
    // Member types
    using size_type = std::size_t;
    using value_type = ValType;

    // Constructors and Destructor
    MSArray()  // Default ctor
        : _data(new value_type[8]), _length(8)
    {}

    // 1-parameter constructor
    // Pre: length >= 0
    explicit MSArray(size_type length)  
        : _data(new value_type[length]), _length(length)
    {}

    // 2-parameter constructor
    // Pre: length >= 0
    MSArray(size_type length, const value_type& init_val)  
        : _data(new value_type[length]), _length(length) 
    {
        std::fill(begin(), end(), init_val);  // Set each element from begin() to end() to init_val
    }

    // Destructor
    ~MSArray() {
        delete[] _data;
    }

    // Copy Constructor
    MSArray(const MSArray& other) 
        : _data(new value_type[other._length]), _length(other._length) 
    {
        std::copy(other.begin(), other.end(), _data);  // Copy elements from other array
    }

    // Move Constructor
    MSArray(MSArray&& other) noexcept 
        : _data(other._data), _length(other._length) 
    {
        other._data = nullptr;
        other._length = 0;
    }

    // Copy Assignment
    MSArray& operator=(const MSArray& other) {
        if (this != &other) {
            delete[] _data;  // Release the existing resource
            _length = other._length;
            _data = new value_type[_length];  // Allocate new resource
            std::copy(other.begin(), other.end(), _data);  // Copy elements
        }
        return *this;
    }

    // Move Assignment
    MSArray& operator=(MSArray&& other) noexcept {
        if (this != &other) {
            swap(other);
        }
        return *this;
    }

    // Member Functions
    // Bracket Operators
    // Pre: index < _length
    value_type& operator[](size_type index) {
        return _data[index];
    }

    // Pre: index < _length
    const value_type& operator[](size_type index) const {
        return _data[index];
    }

    // Returns array size
    size_type size() const {
        return _length;
    }

    // Returns address of item [0]
    value_type* begin() {
        return _data;
    }

    const value_type* begin() const {
        return _data;
    }

    // Returns address of one past the end
    value_type* end() {
        return _data + _length;
    }

    const value_type* end() const {
        return _data + _length;
    }

private:
    // Member variables
    value_type* _data;
    size_type _length;

    // Private member function for swap
    void swap(MSArray& other) noexcept {
        std::swap(_data, other._data);
        std::swap(_length, other._length);
    }
}; // End of class MSArray

// Global Operators

// Pre: arrays are the same type
// Invariant: None
template <typename ValType>
bool operator==(const MSArray<ValType>& lhs, const MSArray<ValType>& rhs)  // Checks if sizes and values are equal
{
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

// Pre: arrays are the same type
// Invariant: None
template <typename ValType>
bool operator!=(const MSArray<ValType>& lhs, const MSArray<ValType>& rhs)  // Checks if not equal
{
    return !(lhs == rhs);
}

// Pre: arrays are the same type
// Invariant: None
template <typename ValType>
bool operator<(const MSArray<ValType>& lhs, const MSArray<ValType>& rhs)  // Checks if lhs < rhs element-wise
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

// Pre: arrays are the same type
// Invariant: None
template <typename ValType>
bool operator<=(const MSArray<ValType>& lhs, const MSArray<ValType>& rhs)  // Checks if lhs <= rhs
{
    return !(rhs < lhs);
}

// Pre: arrays are the same type
// Invariant: None
template <typename ValType>
bool operator>(const MSArray<ValType>& lhs, const MSArray<ValType>& rhs)  // Checks if lhs > rhs
{
    return rhs < lhs;
}

// Pre: arrays are the same type
// Invariant: None
template <typename ValType>
bool operator>=(const MSArray<ValType>& lhs, const MSArray<ValType>& rhs)  // Checks if lhs >= rhs
{
    return !(lhs < rhs);
}

#endif // MSARRAY_HPP
