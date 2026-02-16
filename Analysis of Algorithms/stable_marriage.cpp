// stable_marriage.cpp
// Glenn G. Chappell
// 2025-11-17
//
// For CS 411 Fall 2025
// Stable Marriage Algorithm

#include <iostream>
using std::cout;
using std::cin;
#include <vector>
using std::vector;
#include <queue>
using std::queue;
#include <utility>
using std::pair;
#include <cstdlib>
using std::size_t;
#include <cassert>
// For assert


// find_smm
// Find stable marriage matching (solve Stable Marriage Problem)
// n is the number of men = number of women.
// Each man is referred to by an index: 0 .. n-1, and similarly for each
// woman.
// mens_prefs, which has size n, gives the preferences of each man.
// mens_prefs[k] is the preferences of man #k. Its value is a vector of
// size n holding women's indices, best to worst. womens_prefs is
// similarly structured.
// Return value is vector of pairs: man, woman.
vector<pair<size_t, size_t>> find_smm(
    size_t n,
    const vector<vector<size_t>> & mens_prefs,
    const vector<vector<size_t>> & womens_prefs)
{
    // *** Verify correctness of parameters ***

    assert(mens_prefs.size() == n);
    for (const auto & pref : mens_prefs)
    {
        assert(pref.size() == n);
        for (size_t w : pref)
            assert(w < n);
        // We do NOT check that each value occurs exactly once in pref.
    }

    assert(womens_prefs.size() == n);
    for (const auto & pref : womens_prefs)
    {
        assert(pref.size() == n);
        for (size_t m : pref)
            assert(m < n);
        // We do NOT check that each value occurs exactly once in pref.
    }

    // *** Preprocessing: set up internal-use data structures ***

    // Set up required info for each man
    vector<queue<size_t>> mens_info(n);
    for (size_t m = 0; m < n; ++m)
    {
        for (size_t w : mens_prefs[m])
        {
            mens_info[m].push(w);
        }
    }

    // Set up required info for each woman
    struct WI {
        vector<size_t> _ratings;  // Lower number is better
        bool           _matched;
        size_t         _mate;     // valid if _matched
    };
    vector<WI> womens_info(n);
    for (size_t w = 0; w < n; ++w)
    {
        womens_info[w]._ratings.resize(n);
        for (size_t r = 0; r < n; ++r)
        {
            size_t m = womens_prefs[w][r];
            womens_info[w]._ratings[m] = r;
        }
        womens_info[w]._matched = false;
    }

    // Make list of free men
    vector<size_t> free_men(n);
    for (size_t m = 0; m < n; ++m)
        free_men[m] = m;

    // *** Do computation ***

    while (!free_men.empty())
    {
        // Choose a free man
        size_t m = free_men.back();

        // Find his favorite never-rejected-me woman
        assert(!mens_info[m].empty());
        size_t w = mens_info[m].front();
        mens_info[m].pop();

        // "Propose"
        if (!womens_info[w]._matched)
        {  // Woman w is free; she says yes
            womens_info[w]._matched = true;
            womens_info[w]._mate = m;
            free_men.pop_back();
         }
         else
         {  // Woman w is matched
             size_t curr_mate = womens_info[w]._mate;
             if (womens_info[w]._ratings[m]
               < womens_info[w]._ratings[curr_mate])
             {  // m is better than w's current mate; she says yes to m
                // & no to current mate (if m is worse, we do nothing)
                 womens_info[w]._mate = m;
                 free_men.back() = curr_mate;
             }
         }
    }

    // *** Return list of pairs ***

    vector<pair<size_t, size_t>> result;
    for (size_t w = 0; w < n; ++w)
    {
        assert(womens_info[w]._matched);
        result.push_back({womens_info[w]._mate, w});
    }
    return result;
}


// userPause
// Wait for user to press ENTER: read all chars through first newline.
void userPause()
{
    std::cout.flush();
    while (std::cin.get() != '\n') ;
}


// Main program
// Use function find_smm to find a stable marriage matching, and print
// results.
int main()
{
    // Set up data
    size_t n = 2;

    vector<vector<size_t>> mens_prefs;
    mens_prefs.push_back({0,1});
    mens_prefs.push_back({0,1});

    vector<vector<size_t>> womens_prefs;
    womens_prefs.push_back({0,1});
    womens_prefs.push_back({0,1});

    // Find stable marriage matching
    auto smm = find_smm(n, mens_prefs, womens_prefs);

    // Print results
    cout << "Stable Marriage Matching:\n";
    for (const auto & p : smm)
    {
        cout << "(" << p.first << ", " << p.second << ")\n";
    }

    // Wait for user
    cout << "Press ENTER to quit ";
    userPause();
}

