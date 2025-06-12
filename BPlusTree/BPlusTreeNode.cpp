#include <iostream>
#include "BPlusTreeNode.h"

using namespace std;

BPlusTreeNode::BPlusTreeNode(bool leaf) {
    isLeaf = leaf;
    keyCount = 0;
    next = nullptr;
    for (int i = 0; i < ORDER; ++i)
        children[i] = nullptr;
}

void BPlusTreeNode::traverse() {
    int i;
    for (i = 0; i < keyCount; ++i) {
        if (!isLeaf)
            children[i]->traverse();
        cout << keys[i] << " ";
    }
    if (!isLeaf)
        children[i]->traverse();
}

BPlusTreeNode* BPlusTreeNode::search(int key) {
    int i = 0;
    while (i < keyCount && key > keys[i])
        i++;

    if (isLeaf)
        return (i < keyCount && keys[i] == key) ? this : nullptr;

    return children[i]->search(key);
}

void BPlusTreeNode::insertNonFull(int key) {
    int i = keyCount - 1;

    if (isLeaf) {
        while (i >= 0 && keys[i] > key) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = key;
        keyCount++;
    } else {
        while (i >= 0 && keys[i] > key)
            i--;

        if (children[i + 1]->keyCount == ORDER - 1) {
            splitChild(i + 1, children[i + 1]);
            if (keys[i + 1] < key)
                i++;
        }
        children[i + 1]->insertNonFull(key);
    }
}

void BPlusTreeNode::splitChild(int index, BPlusTreeNode* child) {
    BPlusTreeNode* newNode = new BPlusTreeNode(child->isLeaf);
    newNode->keyCount = (ORDER - 1) / 2;

    for (int j = 0; j < newNode->keyCount; j++)
        newNode->keys[j] = child->keys[j + (ORDER / 2)];

    if (!child->isLeaf) {
        for (int j = 0; j <= newNode->keyCount; j++)
            newNode->children[j] = child->children[j + (ORDER / 2)];
    }

    for (int j = keyCount; j >= index + 1; j--)
        children[j + 1] = children[j];

    children[index + 1] = newNode;

    for (int j = keyCount - 1; j >= index; j--)
        keys[j + 1] = keys[j];

    keys[index] = child->keys[(ORDER - 1) / 2];
    keyCount++;

    child->keyCount = (ORDER - 1) / 2;
}
