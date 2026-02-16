// kruskal.cpp
// Glenn G. Chappell
// 2025-11-05
//
// For CS 411 Fall 2025
// Kruskal's Algorithm
// Based on prim.cpp
// Requires unionfind.hpp

#include "unionfind.hpp"
// For class UnionFind
#include <iostream>
using std::cout;
using std::cin;
#include <iomanip>
using std::setw;
#include <vector>
using std::vector;
#include <cstddef>
using std::size_t;
#include <utility>
using std::pair;
#include <iterator>
using std::begin;
using std::end;
#include <algorithm>
using std::sort;
#include <cassert>
// For assert


// CONVENTIONS

// A GRAPH is represented by adjacency lists.
// We use N for the number of vertices in a graph. Vertices are numbered
// 0 .. N-1. A vertex index is stored as a size_t.
//
// Adjacency lists are stored in a vector<vector<size_t>> with size N.
// Item i in this vector is a list of the neighbors of vertex i, in
// ascending order.

// A WEIGHTED GRAPH (weights on edges) is represented by a matrix of
// weights, possibly along with adjacency lists. The matrix is stored as
// a vector<vector<size_t>> with size N, in which all items have size N.
// Entry [i][j] holds the weight of the edge with endpoints i, j, if
// this edge exists. Otherwise it holds the special value NO_EDGE.
//
// To create a weighed graph represented as above, first set a constant
// N to the number of vertices. Call emptyWeightMatrix, passing N, to
// get a weight matrix for an N-vertex graph with all weights set to
// NO_EDGE. Make zero or more calls to setWeight to set the weights of
// various edges. Lastly, call makeAdjLists to get adjacency lists for
// the graph.


const int NO_EDGE = -1;             // In weight matrix, means no edge

using Edge = pair<size_t, size_t>;  // Type for edge of graph
                                    //  size_t's are indices of endpts


// kruskal
// Compute minimum spanning tree using Kruskal's Algorithm.
// Given weighted graph (represented as discussed above), order of
// graph, and adjacency lists.
vector<Edge> kruskal(
    const vector<vector<size_t>> & adjlists,  // adjacency lists
    const vector<vector<int>> & wmatrix)      // weight matrix
{
    const size_t N = adjlists.size();
                                  // Number of vertices in graph
    assert (wmatrix.size() == N);
    for (size_t i = 0; i < N; ++i)
    {
        assert (wmatrix[i].size() == N);
    }

    // Make list of edges
    vector<Edge> edges;
    for (size_t i = 0; i < N; ++i)
    {
        for (auto v : adjlists[i])
        {
            if (i < v)
                edges.push_back(Edge(i, v));
        }
    }

    // List of edges -> sorted list of edges
    sort(begin(edges), end(edges),
         [&](const Edge & a, const Edge & b)
         { return wmatrix[a.first][a.second]
                < wmatrix[b.first][b.second]; });

    // Make union-find structure with one blob for each vertex
    UnionFind uf;
    for (size_t i = 0; i < N; ++i)
    {
        uf.makeSet(i);
    }

    // Make tree from cheapest N-1 edges that do not create cycle
    vector<Edge> tree;          // Edges in tree
    for (auto e : edges)
    {
        if (uf.find(e.first) == uf.find(e.second))
            continue;

        // Add new edge to tree & do union
        tree.push_back(e);
        if (tree.size()+1 == N)  // Easy optimization
            break;
        uf.unionx(e.first, e.second);
    }

    // Done
    return tree;
}


// emptyWeightMatrix
// Return weight matrix (see above) for an edgeless graph with N
// vertices. Edge weights can then be added using setWeight.
//
// Basically, this function is a constructor, except that we are not
// using a class here.
vector<vector<int>> emptyWeightMatrix(size_t N)
{
    return vector<vector<int>>(N, vector<int>(N, NO_EDGE));
}


// setWeight
// Sets weight of a,b edge in weighted graph described by
//  wmatrix (see above).
void setWeight(vector<vector<int>> & wmatrix,
               size_t a, size_t b,
               int wt)
{
    wmatrix[a][b] = wt;
    wmatrix[b][a] = wt;
}


// printWeightMatrix
// Given a matrix stored in a vector, with the (i,j) entry stored in
// matrix[i*numcols+j], print the matrix, entries separated by blanks,
// rows ending with newline. Each entry is printed as its numerical
// value, or "-" if it is NO_EDGE. Column labels are printed above the
// matrix; row labels are printed to its left. All numbers are printed
// with a field width of 3, padded on the left with blanks.
void printWeightMatrix(const vector<vector<int>> & wmatrix)
{
    const size_t N = wmatrix.size();

    cout << "    | ";
    for (size_t col = 0; col < N; ++col)
    {
        cout << setw(3) << col << " ";
    }
    cout << "\n";

    cout << " ---+-";
    for (size_t col = 0; col < N; ++col)
    {
        cout << "----";
    }
    cout << "\n";

    for (size_t row = 0; row < N; ++row)
    {
        cout << setw(3) << row << " | ";
        for (size_t col = 0; col < N; ++col)
        {
            int value = wmatrix[row][col];
            if (value == NO_EDGE)
                cout << "  -";
            else
                cout << setw(3) << value;
            cout << " ";
        }
        cout << "\n";
    }
}


// makeAdjLists
// Given matrix representation of weighted graph (see above), return
// adjacency lists.
vector<vector<size_t>> makeAdjLists(const vector<vector<int>> & wmatrix)
{
    const size_t N = wmatrix.size();

    vector<vector<size_t>> adjlists(N, vector<size_t>());
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            if (wmatrix[i][j] != NO_EDGE)
                adjlists[i].push_back(j);
        }
    }
    return adjlists;
}


// printAdjacencyLists
// Given adjacency lists for a graph, print them, each lists on a
// separate line, preceded by the index of the vertex whose lists this
// is, entries separated by commas.
void printAdjacencyLists(const vector<vector<size_t>> & adjlists)
{
    const size_t N = adjlists.size();
    for (size_t i = 0; i < N; ++i)
    {
        cout << setw(3) << i << ": ";
        for (size_t j = 0; j < adjlists[i].size(); ++j)
        {
            if (j != 0)
                cout << ", ";
            cout << adjlists[i][j];
        }
        cout << "\n";
    }
}


// userPause
// Wait for user to press ENTER: read all chars through first newline.
void userPause()
{
    std::cout.flush();
    while (std::cin.get() != '\n') ;
}


// Main program
// Compute a minimum spanning tree using function kruskal. Print graph
// and result.
int main()
{
    const size_t N = 5;  // Number of vertices of graph

    // Set up weight matrix for graph;
    auto wmatrix = emptyWeightMatrix(N);
    setWeight(wmatrix, 0, 1, 7);
    setWeight(wmatrix, 0, 2, 6);
    setWeight(wmatrix, 0, 3, 2);
    setWeight(wmatrix, 0, 4, 8);
    setWeight(wmatrix, 1, 2, 3);
    setWeight(wmatrix, 1, 4, 4);
    setWeight(wmatrix, 2, 3, 5);
    setWeight(wmatrix, 2, 4, 1);
    setWeight(wmatrix, 3, 4, 9);

    // Print weights
    cout << "Weight matrix:\n";
    printWeightMatrix(wmatrix);
    cout << "\n";

    // Compute & print adjacency lists
    auto adjlists = makeAdjLists(wmatrix);
    cout << "Adjacency lists:\n";
    printAdjacencyLists(adjlists);
    cout << "\n";

    // Pause before running algorithm
    cout << "Press ENTER to run Kruskal's Algorithm ";
    userPause();

    // Run Kruskal's Algorithm & print result
    cout << "Running Kruskal's Algorithm ... ";
    cout.flush();
    vector<Edge> tree = kruskal(adjlists, wmatrix);
    cout << "DONE\n\n";

    cout << "Edges in minimum spanning tree:\n";
    if (tree.size() == 0)
        cout << "[NONE]\n";
    for (size_t i = 0; i < tree.size(); ++i)
    {
        cout << setw(3) << tree[i].first << ", " << tree[i].second
             << "\n";
    }
    cout << "\n";

    // Wait for user
    cout << "Press ENTER to quit ";
    userPause();
}

