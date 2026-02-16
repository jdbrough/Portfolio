#ifndef FILE_PLAYER_HPP_INCLUDED
#define FILE_PLAYER_HPP_INCLUDED

#include <ostream>
#include <cassert>
#include <string>

// *********************************************************************
// class Player - Class definition
// *********************************************************************

class Player {
public:
    // Default ctor
    Player();

    // Ctor with Real_Name, Username, Games_Played
    Player(std::string rn, std::string u, int g);

    // getRealName
    std::string getRealName() const;

    // getUsername
    std::string getUsername() const;

    // getGames
    int getGames() const;

    // setRealName
    void setRealName(const std::string& n);

    // setUsername
    void setUsername(const std::string& u);

    // setGames
    void setGames(int g);

    // inactive
    bool inactive() const;

    // toString
    std::string toString() const;

    // Operators
    Player& operator++();
    Player operator++(int);

    Player& operator--();
    Player operator--(int);

private:
    std::string _realName;
    std::string _username;
    int _gamesPlayed;

};  // End class Player

// Global operators
bool operator==(const Player& a, const Player& b);
bool operator!=(const Player& a, const Player& b);

std::ostream& operator<<(std::ostream& os, const Player& p);

#endif // #ifndef FILE_PLAYER_HPP_INCLUDED