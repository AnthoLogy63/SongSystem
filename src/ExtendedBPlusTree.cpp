#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "../include/ExtendedBPlusTree.h"
#include "../include/SongRating.h"

using namespace std;

ExtendedBPlusTree::ExtendedBPlusTree() {
    root = new ExtendedBPlusTreeNode(true);
    globalAverage = 0.0;
    totalRatings = 0;
    confidenceFactor = 5;
}

void ExtendedBPlusTree::insert(const SongStats& song) {
    if (root->keyCount == SONG_ORDER - 1) {
        ExtendedBPlusTreeNode* newRoot = new ExtendedBPlusTreeNode(false);
        newRoot->children[0] = root;
        newRoot->splitChild(0, root);
        int i = (newRoot->songs[0].songId < song.songId) ? 1 : 0;
        newRoot->children[i]->insertNonFull(song);
        root = newRoot;
    } else {
        root->insertNonFull(song);
    }
}

void ExtendedBPlusTree::traverse() {
    if (root != nullptr)
        root->traverse();
    cout << "\n";
}

ExtendedBPlusTreeNode* ExtendedBPlusTree::search(int songId) {
    return (root == nullptr) ? nullptr : root->search(songId);
}

SongStats* ExtendedBPlusTree::findSong(int songId) {
    return (root == nullptr) ? nullptr : root->findSong(songId);
}

void ExtendedBPlusTree::loadFromCSV(const string& filename) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string userId, songId, rating, timestamp;

        if (getline(ss, userId, ',') && 
            getline(ss, songId, ',') && 
            getline(ss, rating, ',') && 
            getline(ss, timestamp, ',')) {

            int id = stoi(songId);
            double rate = stod(rating);

            SongStats* existingSong = findSong(id);
            if (existingSong) {
                existingSong->addRating(rate);
            } else {
                SongStats newSong(id);
                newSong.addRating(rate);
                insert(newSong);
            }
        }
    }
    file.close();

    calculateAllBayesianAverages();
}

void ExtendedBPlusTree::calculateGlobalAverage() {
    vector<SongStats*> allSongs;
    if (root) {
        root->getAllSongs(allSongs);
    }

    double totalSum = 0.0;
    int totalCount = 0;

    for (const auto* song : allSongs) {
        if (song->ratingCount > 0) {
            totalSum += song->totalRating;
            totalCount += song->ratingCount;
        }
    }

    globalAverage = (totalCount > 0) ? totalSum / totalCount : 0.0;
    totalRatings = totalCount;
}

void ExtendedBPlusTree::updateBayesianAverages() {
    vector<SongStats*> allSongs;
    if (root) {
        root->getAllSongs(allSongs);
    }

    for (auto* song : allSongs) {
        song->calculateBayesianAverage(globalAverage, confidenceFactor);
    }
}

void ExtendedBPlusTree::calculateAllBayesianAverages() {
    calculateGlobalAverage();
    updateBayesianAverages();
}

void ExtendedBPlusTree::setConfidenceFactor(int factor) {
    confidenceFactor = factor;
    updateBayesianAverages();
}

vector<SongStats> ExtendedBPlusTree::getTopNSongsBayesian(int n) {
    vector<SongStats*> allSongs;
    if (root) {
        root->getAllSongs(allSongs);
    }

    sort(allSongs.begin(), allSongs.end(),
        [](const SongStats* a, const SongStats* b) {
            return a->bayesianAverage > b->bayesianAverage;
        });

    vector<SongStats> result;
    for (int i = 0; i < min(n, (int)allSongs.size()); ++i) {
        result.push_back(*allSongs[i]);
    }

    return result;
}

vector<SongStats> ExtendedBPlusTree::getBottomNSongsBayesian(int n) {
    vector<SongStats*> allSongs;
    if (root) {
        root->getAllSongs(allSongs);
    }

    sort(allSongs.begin(), allSongs.end(),
        [](const SongStats* a, const SongStats* b) {
            return a->bayesianAverage < b->bayesianAverage;
        });

    vector<SongStats> result;
    for (int i = 0; i < min(n, (int)allSongs.size()); ++i) {
        result.push_back(*allSongs[i]);
    }

    return result;
}

