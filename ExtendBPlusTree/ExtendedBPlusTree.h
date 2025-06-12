#ifndef EXTENDEDBPLUSTREE_H
#define EXTENDEDBPLUSTREE_H

#include "ExtendedBPlusTreeNode.h"
#include <vector>
#include <string>

class ExtendedBPlusTree {
private:
    ExtendedBPlusTreeNode* root;
    double globalAverage;
    int totalRatings;
    int confidenceFactor; 

public:
    ExtendedBPlusTree();
    void insert(const SongStats& song);
    void traverse();
    ExtendedBPlusTreeNode* search(int songId);
    SongStats* findSong(int songId);
    void loadFromCSV(const std::string& filename);
    
    SongStats getBestSong();
    SongStats getWorstSong();
    std::vector<SongStats> getTopNSongs(int n);
    std::vector<SongStats> getBottomNSongs(int n);
    
    // Nuevos métodos con Bayesian Average
    SongStats getBestSongBayesian();
    SongStats getWorstSongBayesian();
    std::vector<SongStats> getTopNSongsBayesian(int n);
    std::vector<SongStats> getBottomNSongsBayesian(int n);
    
    void calculateAllBayesianAverages();
    void setConfidenceFactor(int factor);
    void printSongStats(int songId);
    void printAllSongs();
    void printAllSongsBayesian();
    
private:
    void calculateGlobalAverage();
    void updateBayesianAverages();
};

#endif
