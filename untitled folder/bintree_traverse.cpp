// bintree_traverse.cpp
// Glenn G. Chappell
// 2025-10-03
//
// For CS 411 Fall 2025
// Binary Tree Traversals: Preorder, Inorder, Postorder

#include <iostream>
using std::cout;
using std::endl;
using std::cin;
#include <string>
using std::string;
#include <memory>
using std::shared_ptr;
using std::make_shared;


// We represent a Binary Tree using a BinaryTree, which is a typedef for
// shared_ptr<BTNode>. A struct BTNode represents a node of the tree.
// Member value holds the root value, while members left, right hold the
// left & right subtrees, respectively.
//
// A Binary Tree represented by a BinaryTree object t is *valid* if
//
// Either
// (1) t == nullptr (in which case the tree has no nodes).
// OR
// (2) All of the following hold:
//   (2a) t holds a ptr to a BTNode with ownership handled properly for
//        a shared_ptr.
//   AND
//   (2b) t->left and t->right are valid Binary Trees.
//   AND
//   (2c) No node appears in the tree more than once (this implies that
//        the tree has finite height).

struct BTNode;  // Forward declaration
using BinaryTree = shared_ptr<BTNode>;


// struct BTNode
// Binary Tree Node with string value
// Has convenience ctor.
struct BTNode {

    string value;   // This node's value
    BinaryTree left;   // Left subtree
    BinaryTree right;  // Right subtree

    // Ctor from value & optional left/right trees
    explicit BTNode(const string & theValue,
                    BinaryTree theLeft=nullptr,
                    BinaryTree theRight=nullptr)
        :value(theValue),
         left(theLeft),
         right(theRight)
    {}
};


// createBinaryTree
// Create & return a Binary Tree holding abstract syntax tree of an
// arithmetic expression: (2+8)*(7-4)/2.
// Pre: None.
// Post:
//     Return value is a valid Binary Tree (see above).
BinaryTree createBinaryTree()
{
    auto np1 = make_shared<BTNode>("2");
    auto np2 = make_shared<BTNode>("8");
    auto np3 = make_shared<BTNode>("+", np1, np2);

    auto np4 = make_shared<BTNode>("7");
    auto np5 = make_shared<BTNode>("4");
    auto np6 = make_shared<BTNode>("-", np4, np5);

    auto np7 = make_shared<BTNode>("*", np3, np6);

    auto np8 = make_shared<BTNode>("2");
    auto np9 = make_shared<BTNode>("/", np7, np8);

    return np9;
}


// preorderTraverse
// Outputs a preorder traversal of the given Binary Tree to cout. Value
// of each node is printed. If a node has at least one child, then d1,
// d2, d3, d4 are used as delimiters. There is no terminating newline
// (except as part of delimiters).
void preorderTraverse(BinaryTree t,
                      const string & d1="",
                      const string & d2=" ",
                      const string & d3=" ",
                      const string & d4="")
{
    if (t == nullptr)
        return;
    if (t->left == nullptr && t->right == nullptr)
    {
        // This is just to make the output prettier
        cout << t->value;
        return;
    }
    cout << d1;
    cout << t->value;
    cout << d2;
    preorderTraverse(t->left, d1, d2, d3, d4);
    cout << d3;
    preorderTraverse(t->right, d1, d2, d3, d4);
    cout << d4;
}


// inorderTraverse
// Outputs an inorder traversal of the given Binary Tree to cout.
// Other specs same as preorderTraverse.
void inorderTraverse(BinaryTree t,
                     const string & d1="",
                     const string & d2=" ",
                     const string & d3=" ",
                     const string & d4="")
{
    if (t == nullptr)
        return;
    if (t->left == nullptr && t->right == nullptr)
    {
        // This is just to make the output prettier
        cout << t->value;
        return;
    }
    cout << d1;
    inorderTraverse(t->left, d1, d2, d3, d4);
    cout << d2;
    cout << t->value;
    cout << d3;
    inorderTraverse(t->right, d1, d2, d3, d4);
    cout << d4;
}


// postorderTraverse
// Outputs a postorder traversal of the given Binary Tree to cout.
// Other specs same as preorderTraverse.
void postorderTraverse(BinaryTree t,
                       const string & d1="",
                       const string & d2=" ",
                       const string & d3=" ",
                       const string & d4="")
{
    if (t == nullptr)
        return;
    if (t->left == nullptr && t->right == nullptr)
    {
        // This is just to make the output prettier
        cout << t->value;
        return;
    }
    cout << d1;
    postorderTraverse(t->left, d1, d2, d3, d4);
    cout << d2;
    postorderTraverse(t->right, d1, d2, d3, d4);
    cout << d3;
    cout << t->value;
    cout << d4;
}


// Main program
// Demonstrates Binary Tree traversals.
int main()
{
    // Make a Binary Tree
    auto t = createBinaryTree();

    cout << "Preorder:\n  ";
    preorderTraverse(t);
    cout << endl << endl;

    cout << "Inorder:\n  ";
    inorderTraverse(t);
    cout << endl << endl;

    cout << "Postorder (same as RPN, Forth):\n  ";
    postorderTraverse(t);
    cout << endl << endl;

    cout << "Lisp (preorder):\n  ";
    preorderTraverse(t, "(", " ", " ", ")");
    cout << endl << endl;

    cout << "C++ operator-function calls (preorder):\n  ";
    preorderTraverse(t, "operator", "(", ", ", ")");
    cout << endl << endl;

    cout << "Normal (e.g., C++) infix operators (inorder):\n  ";
    inorderTraverse(t, "(", " ", " ", ")");
    cout << endl << endl;

    // Wait for user
    cout << "Press ENTER to quit ";
    while (cin.get() != '\n') ;

    return 0;
}

