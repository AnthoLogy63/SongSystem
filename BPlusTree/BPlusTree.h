#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include "BPlusTreeNode.h"

class BPlusTree {
private:
    BPlusTreeNode* root;

public:
    BPlusTree();
    void insert(int key);
    void traverse();
    void printTree();
    BPlusTreeNode* search(int key);
};

#endif
