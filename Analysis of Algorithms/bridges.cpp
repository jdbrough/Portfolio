// bridges.cpp
// Jonathan Brough
// 2025-09-24
//
// For CS 411 Fall 2025
// Brute-force search for max toll with non-crossing bridges

#include "bridges.hpp"
#include <vector>

// check if two bridges cross
bool crosses(const Bridge& a, const Bridge& b) {
    return (a[0] < b[0] && a[1] > b[1]) ||
           (a[0] > b[0] && a[1] < b[1]);
}

// recursive search (include / skip each bridge)
int dfs(int idx,
        const std::vector<Bridge>& br,
        std::vector<bool>& usedW,
        std::vector<bool>& usedE,
        std::vector<int>& chosen)
{
    if (idx == (int) br.size()) return 0;        // base case

    // option 1: skip this bridge
    int best = dfs(idx + 1, br, usedW, usedE, chosen);

    // option 2: try to include this bridge if legal
    const Bridge& b = br[idx];
    int w = b[0], e = b[1], toll = b[2];

    bool can_take = true;
    if (w < 0 || e < 0 || w >= (int)usedW.size() || e >= (int)usedE.size())
        can_take = false;
    else if (usedW[w] || usedE[e])
        can_take = false;

    for (int j : chosen) {
        if (crosses(b, br[j])) { can_take = false; break; }
    }

    if (can_take) {
        usedW[w] = true; usedE[e] = true;
        chosen.push_back(idx);

        int with = toll + dfs(idx + 1, br, usedW, usedE, chosen);
        if (with > best) best = with;

        chosen.pop_back();
        usedW[w] = false; usedE[e] = false;
    }

    return best;
}

int bridges(int w, int e, const std::vector<Bridge>& br)
{
    std::vector<bool> usedW(w, false);
    std::vector<bool> usedE(e, false);
    std::vector<int> chosen;
    return dfs(0, br, usedW, usedE, chosen);
}