#include <iostream>
#include <queue>
#include "BPlusTree.h"

using namespace std;

BPlusTree::BPlusTree() {
    root = new BPlusTreeNode(true);
}

void BPlusTree::insert(int key) {
    if (root->keyCount == ORDER - 1) {
        BPlusTreeNode* newRoot = new BPlusTreeNode(false);
        newRoot->children[0] = root;
        newRoot->splitChild(0, root);
        int i = (newRoot->keys[0] < key) ? 1 : 0;
        newRoot->children[i]->insertNonFull(key);
        root = newRoot;
    } else {
        root->insertNonFull(key);
    }
}

void BPlusTree::traverse() {
    if (root != nullptr)
        root->traverse();
    cout << "\n";
}

BPlusTreeNode* BPlusTree::search(int key) {
    return (root == nullptr) ? nullptr : root->search(key);
}

void BPlusTree::printTree() {
    if (!root) return;
    queue<BPlusTreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int count = q.size();
        for (int i = 0; i < count; ++i) {
            BPlusTreeNode* node = q.front();
            q.pop();

            cout << "[ ";
            for (int j = 0; j < node->keyCount; ++j) {
                cout << node->keys[j];
                if (j != node->keyCount - 1) cout << ", ";
            }
            cout << " ] ";

            if (!node->isLeaf) {
                for (int j = 0; j <= node->keyCount; ++j) {
                    if (node->children[j])
                        q.push(node->children[j]);
                }
            }
        }
        cout << endl;
    }
}
