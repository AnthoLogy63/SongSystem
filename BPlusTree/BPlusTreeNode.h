#ifndef BPLUSTREENODE_H
#define BPLUSTREENODE_H

const int ORDER = 4;

class BPlusTreeNode {
public:
    int keys[ORDER - 1];
    BPlusTreeNode* children[ORDER];
    BPlusTreeNode* next; // para hojas
    int keyCount;
    bool isLeaf;

    BPlusTreeNode(bool leaf);
    void insertNonFull(int key);
    void splitChild(int index, BPlusTreeNode* child);
    void traverse();
    BPlusTreeNode* search(int key);

    friend class BPlusTree;
};

#endif
