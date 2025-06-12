#ifndef EXTENDEDBPLUSTREENODE_H
#define EXTENDEDBPLUSTREENODE_H

#include "SongRating.h"
#include <vector>

const int SONG_ORDER = 4;

class ExtendedBPlusTreeNode {
public:
    SongStats songs[SONG_ORDER - 1];
    ExtendedBPlusTreeNode* children[SONG_ORDER];
    ExtendedBPlusTreeNode* next; // para hojas
    int keyCount;
    bool isLeaf;

    ExtendedBPlusTreeNode(bool leaf);
    void insertNonFull(const SongStats& song);
    void splitChild(int index, ExtendedBPlusTreeNode* child);
    void traverse();
    ExtendedBPlusTreeNode* search(int songId);
    SongStats* findSong(int songId);
    void getAllSongs(std::vector<SongStats>& allSongs);

    friend class ExtendedBPlusTree;
};

#endif
