// topological_sort.cpp
// Glenn G. Chappell
// 2025-09-19
//
// For CS 411 Fall 2025
// Topological Sort
// Uses Find-Source algorithm

#include <iostream>
using std::cout;
using std::endl;
using std::cin;
#include <vector>
using std::vector;
#include <cstddef>
using std::size_t;
#include <algorithm>
using std::lower_bound;
#include <utility>
using std::pair;
#include <queue>
using std::queue;
#include <cassert>
// For assert


// CONVENTIONS

// A DIGRAPH is represented by adjacency lists.
// We use N for the number of vertices in a digraph. Vertices are
// numbered 0 .. N-1.
//
// Adjacency lists are stored in a vector<vector<int>> with size N. Item
// i in this vector is a list of the neighbors of vertex i, in ascending
// order.
//
// To create a digraph represented by adjacency lists, call function
// emptyAdjLists to create a new edgeless digraph. Then call function
// digraphAddEdge to add each edge to the digraph.

// A traversal of a digraph is printed as a list of vertex indices (each
// 0 .. N-1) separated by blanks.


// topSort
// Try to find Topological Sort ordering of vertices of given digraph.
// If no such ordering exists, return a pair with first item false. If
// an ordering exists, return a pair with first item true and second
// item a vector holding vertex indices in Topological-Sort order.
//
// Pre:
//     adjlists holds the adjacency lists of a digraph (as above).
pair<bool, vector<int>> topSort(const vector<vector<int>> & adjlists)
{
    // Get number of vertices in digraph
    const int N = int(adjlists.size());

    vector<int> indegrees(N, 0);  // indegrees[i] counts in-neighbors
                                  //  of vertex i that have NOT been
                                  //  processed yet
    for (int i = 0; i < N; ++i)
    {
        for (int j : adjlists[i])
        {
            ++indegrees[j];
        }
    }

    queue<int> sources;           // Vertices with no unprocessed
                                  //  in-neighbors
    for (int i = 0; i < N; ++i)
    {
        if (indegrees[i] == 0)
            sources.push(i);
    }

    vector<int> order;            // Vertices, in topological sort order
    while (!sources.empty())
    {
        // Get a source
        int s = sources.front();
        sources.pop();

        // Process this source as next vertex in topological sort
        order.push_back(s);

        // Consider s removed from digraph; update indegrees and sources
        for (int v : adjlists[s])
        {
            // edge from s to v, so reduce indegree of v ...
            --indegrees[v];
            // ... and add v to sources if it has become a source
            if (indegrees[v] == 0)
                sources.push(v);
        }
    }

    if (order.size() < size_t(N))         // Ran out of sources early?
        return { false, vector<int>() };  // No top sort
    return { true, order };  // Return top sort
}


// printTopSort
// Given adjacency lists for a digraph, attempt to find a Topological
// Sort. If none exists, print a message. If one exists, print the
// vertex indices in Topological Sort order. Output ends with newline.
//
// Pre:
//     adjlists holds the adjacency lists of a digraph (as above).
void printTopSort(const vector<vector<int>> & adjlists)
{
    // Try to find Topological Sort
    auto info = topSort(adjlists);

    // If no Topological Sort exists, then print message & return
    if (!info.first)
    {
        cout << "Digraph is not a DAG. No topological sort exists.\n";
        return;
    }

    // If Topological Sort exists, then output it
    cout << "Topological sort:";
    for (int v : info.second)
    {
        cout << " " << v;
    }
    cout << "\n";
}


// emptyAdjLists
// Return adjacency lists (see above) for an edgeless digraph with N
// vertices. Edges can then be added using digraphAddEdge.
//
// So, basically, this function is a constructor, except that we are not
// using a class here.
//
// Pre: None.
vector<vector<int>> emptyAdjLists(int N)
{
    return vector<vector<int>>(N);
}


// digraphAddEdge
// Adds edge a - b to digraph described by adjlists (see above).
//
// Pre:
//     adjlists is valid adjacency lists (see above).
void digraphAddEdge(vector<vector<int>> & adjlists,
             int a, int b)
{
    assert (0 <= a);
    assert (size_t(a) < adjlists.size());
    assert (0 <= b);
    assert (size_t(b) < adjlists.size());

    // Insert b in adj list for vertex a; find spot with Binary Search.
    auto pos = lower_bound(adjlists[a].begin(), adjlists[a].end(), b);
    adjlists[a].insert(pos, b);
}


// makeAdjLists_WPDAG
// Return adjacency lists for DAG from Wikipedia.
// From article on "Directed acyclic graph" retrieved 2014-09-29.
// Vertex indices in article:  2  3  5  7  8  9 10 11
// Vertex indices used here:   0  1  2  3  4  5  6  7
//
// Pre: None.
vector<vector<int>> makeAdjLists_WPDAG()
{
    // Digraph: 8-vertex DAG from Wikipedia
    const int N = 8;
    auto adjlists = emptyAdjLists(N);

    digraphAddEdge(adjlists, 1, 4);  // Edge 1 -> 4 (original  3 ->  8)
    digraphAddEdge(adjlists, 1, 7);  // Edge 1 -> 7 (original  3 -> 10)
    digraphAddEdge(adjlists, 2, 7);  // Edge 2 -> 7 (original  5 -> 11)
    digraphAddEdge(adjlists, 3, 7);  // Edge 3 -> 7 (original  7 ->  8)
    digraphAddEdge(adjlists, 3, 4);  // Edge 3 -> 4 (original  7 -> 11)
    digraphAddEdge(adjlists, 4, 5);  // Edge 4 -> 5 (original  8 ->  9)
    digraphAddEdge(adjlists, 7, 0);  // Edge 7 -> 0 (original 11 ->  2)
    digraphAddEdge(adjlists, 7, 5);  // Edge 7 -> 5 (original 11 ->  9)
    digraphAddEdge(adjlists, 7, 6);  // Edge 7 -> 6 (original 11 -> 10)

    return adjlists;
}


// makeAdjLists_cycle
// Return adjacency lists for cycle.
//
// Pre: None.
vector<vector<int>> makeAdjLists_cycle()
{
    // Digraph: directed 8-Cycle
    // 8 vertices: 0 .. 7.
    // Adjacencies: 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 0
    const int N = 8;
    auto adjlists = emptyAdjLists(N);

    for (int i = 0; i < N; ++i)
    {
        digraphAddEdge(adjlists, i, (i+1)%N);
    }

    return adjlists;
}


// makeAdjLists_triWithTail
// Return adjacency lists for triangle with tail.
//
// Pre: None.
vector<vector<int>> makeAdjLists_triWithTail()
{
    // Digraph: triangle with tail
    // 8 vertices: 0 .. 7.
    // Adjacencies: 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 5
    const int N = 8;
    auto adjlists = emptyAdjLists(N);

    for (int i = 0; i < N-1; ++i)
        digraphAddEdge(adjlists, i, i+1);
    digraphAddEdge(adjlists, 7, 5);

    return adjlists;
}


// makeAdjLists_mixPath
// Return adjacency lists for mixed-up path.
//
// Pre: None.
vector<vector<int>> makeAdjLists_mixPath()
{
    // Digraph: mixed-up path
    // 8 vertices: 0 .. 7.
    // Adjacencies: 7 -> 1 -> 3 -> 4 -> 2 -> 5 -> 0 -> 6
    const int N = 8;
    auto adjlists = emptyAdjLists(N);

    digraphAddEdge(adjlists, 7, 1);
    digraphAddEdge(adjlists, 1, 3);
    digraphAddEdge(adjlists, 3, 4);
    digraphAddEdge(adjlists, 4, 2);
    digraphAddEdge(adjlists, 2, 5);
    digraphAddEdge(adjlists, 5, 0);
    digraphAddEdge(adjlists, 0, 6);

    return adjlists;
}


// userPause
// Wait for user to press ENTER: read all chars through first newline.
void userPause()
{
    std::cout.flush();
    while (std::cin.get() != '\n') ;
}


// Main program
// Do DFS & BFS on various graphs, printing results.
int main()
{
    vector<vector<int>> adjlists;  // Holds adjacency lists

    // Print topological sort for digraphs

    // Wikipedia DAG
    cout << "DAG from Wikipedia\n";
    adjlists = makeAdjLists_WPDAG();
    printTopSort(adjlists);
    cout << "\n";

    // Cycle
    cout << "8-Cycle\n";
    adjlists = makeAdjLists_cycle();
    printTopSort(adjlists);
    cout << "\n";

    // Triangle with tail
    cout << "Triangle with Tail\n";
    adjlists = makeAdjLists_triWithTail();
    printTopSort(adjlists);
    cout << "\n";

    // Mixed-up path
    cout << "Mixed-up Path\n";
    adjlists = makeAdjLists_mixPath();
    printTopSort(adjlists);
    cout << "\n";

    // Wait for user
    cout << "Press ENTER to quit ";
    userPause();
}

