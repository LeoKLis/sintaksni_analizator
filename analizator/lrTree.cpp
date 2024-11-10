#include "lrTree.h"

LrTree::LrTree(string rootInfo)
{
    this->root->contents = rootInfo;
}

void LrTree::connectNode(Node* to, Node* from)
{
    from->nodes.push_back(to);
}

void LrTree::markAsRoot(Node* node){
    root = node;
}

void LrTree::traverse()
{
    if(root == nullptr) {
        cout << "Desila se neka greska. Root nije NULL!\n";
        return;
    }
    recTraverse(root, 0);
}

void LrTree::recTraverse(Node* node, int depth)
{
    for (int i = 0; i < depth; i++)
        cout << " ";
    if (node->nodes.empty()) {
        cout << node->contents << endl;
        return;
    }
    cout << node->contents << endl;
    for(int i=node->nodes.size() - 1; i >= 0; i--)
        recTraverse(node->nodes[i], depth + 1);
}

void LrTree::delTraverse(Node* node)
{
    if (node->nodes.empty()) {
        delete (node);
        node = nullptr;
        return;
    }
    for (Node* el : node->nodes) {
        delTraverse(el);
    }
    delete (node);
    node = nullptr;
    return;
}

LrTree::~LrTree()
{
    if(root != nullptr)
        delTraverse(root);
}