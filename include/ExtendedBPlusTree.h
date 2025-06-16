#ifndef EXTENDED_BPLUS_TREE_H
#define EXTENDED_BPLUS_TREE_H

#include "ExtendedBPlusTreeNode.h"
#include <string>
#include <vector>
using namespace std;

class ExtendedBPlusTree {
private:
    ExtendedBPlusTreeNode* root;
    int confidenceFactor;
    float globalAverage;
    int totalRatings;  // <-- ✅ AGREGA ESTA VARIABLE

public:
    ExtendedBPlusTree();

    void insert(const SongStats& song);
    SongStats* findSong(int songId);

    SongStats getBestSong();
    SongStats getWorstSong();
    vector<SongStats> getTopNSongs(int n);
    vector<SongStats> getBottomNSongs(int n);

    SongStats getBestSongBayesian();
    SongStats getWorstSongBayesian();
    vector<SongStats> getTopNSongsBayesian(int n);
    vector<SongStats> getBottomNSongsBayesian(int n);

    void printSongStats(int songId);
    void printAllSongs();
    void printAllSongsBayesian();

    void loadFromCSV(const string& filename);
    void calculateGlobalAverage();
    void updateBayesianAverages();
    void calculateAllBayesianAverages();
    void setConfidenceFactor(int factor);

    void traverse();  // ✅ NUEVA DECLARACIÓN
    ExtendedBPlusTreeNode* search(int songId);  // ✅ NUEVA DECLARACIÓN
};

#endif
