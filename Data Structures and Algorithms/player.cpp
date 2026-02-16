// player.cpp
// Jonathan Daniel Brough
// Started: 2024-09-08
// Updated: 2024-09-09
//
// For CS 311 Fall 2024
// Source for class Player
// Player: Real_Name, Username, Games_Played

#include "player.hpp"
#include <cassert>
#include <string>  // For std::to_string

// *********************************************************************
// class Player - Ctors
// *********************************************************************

// Default ctor
Player::Player()
    : _realName("UNKNOWN"),
      _username("UNKNOWN"),
      _gamesPlayed(0)
{
}

// Ctor with Real_Name, Username, Games_Played
Player::Player(std::string rn, std::string u, int g)
    : _realName(rn),
      _username(u),
      _gamesPlayed(g)
{
    assert(g >= 0);
}

// *********************************************************************
// class Player - Member functions
// *********************************************************************

// Player::getRealName
std::string Player::getRealName() const {
    return _realName;
}

// Player::getUsername
std::string Player::getUsername() const {
    return _username;
}

// Player::getGames
int Player::getGames() const {
    return _gamesPlayed;
}

// Player::setRealName
void Player::setRealName(const std::string& rn) {
    _realName = rn;
}

// Player::setUsername
void Player::setUsername(const std::string& u) {
    _username = u;
}

// Player::setGames
void Player::setGames(int g) {
    assert(g >= 0);
    _gamesPlayed = g;
}

// Player::inactive
bool Player::inactive() const {
    assert(_gamesPlayed >= 0);
    return _gamesPlayed == 0;
}

// Player::toString
std::string Player::toString() const {
    assert(_gamesPlayed >= 0);
    return _realName + " (" + _username + "): " + std::to_string(_gamesPlayed);
}

// *********************************************************************
// class Player - Operators
// *********************************************************************

// Equality operator (global)
bool operator==(const Player& a, const Player& b) {
    return a.getRealName() == b.getRealName() &&
           a.getUsername() == b.getUsername() &&
           a.getGames() == b.getGames();
}

// Inequality operator (global)
bool operator!=(const Player& a, const Player& b) {
    return !(a == b);
}

// Pre-increment operator
Player& Player::operator++() {
    ++_gamesPlayed;
    return *this;
}

// Post-increment operator
Player Player::operator++(int) {
    Player temp = *this;
    ++(*this);
    return temp;
}

// Pre-decrement operator
Player& Player::operator--() {
    if (_gamesPlayed > 0) {
        --_gamesPlayed;
    }
    return *this;
}

// Post-decrement operator
Player Player::operator--(int) {
    Player temp = *this;
    --(*this);
    return temp;
}

// Stream insertion operator (global)
std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << player.toString();
    return os;
}