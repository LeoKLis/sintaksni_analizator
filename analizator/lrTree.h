#ifndef LRTREE_H
#define LRTREE_H

#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

struct Node {
    string contents;
    vector<Node*> nodes;
    Node(string contents)
    {
        this->contents = contents;
        nodes = {};
    }
};

class LrTree {
private:
    void recTraverse(Node* node, int depth);
    void delTraverse(Node* node);
public:
    Node* root = nullptr;
    LrTree(){};
    LrTree(string rootInfo);
    void connectNode(Node* to, Node* from);
    void markAsRoot(Node* node);
    void traverse();
    ~LrTree();
};

#endif