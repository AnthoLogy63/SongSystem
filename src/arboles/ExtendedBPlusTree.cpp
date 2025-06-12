#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "ExtendedBPlusTree.h"

using namespace std;

ExtendedBPlusTree::ExtendedBPlusTree() {
    root = new ExtendedBPlusTreeNode(true);
    globalAverage = 0.0;
    totalRatings = 0;
    confidenceFactor = 5; // Valor por defecto: necesita al menos 5 valoraciones para ser confiable
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
    
    // Después de cargar todos los datos, calcular promedios bayesianos
    calculateAllBayesianAverages();
}

void ExtendedBPlusTree::calculateGlobalAverage() {
    vector<SongStats> allSongs;
    if (root) {
        root->getAllSongs(allSongs);
    }
    
    double totalSum = 0.0;
    int totalCount = 0;
    
    for (const auto& song : allSongs) {
        if (song.ratingCount > 0) {
            totalSum += song.totalRating;
            totalCount += song.ratingCount;
        }
    }
    
    globalAverage = (totalCount > 0) ? totalSum / totalCount : 0.0;
    totalRatings = totalCount;
}

void ExtendedBPlusTree::updateBayesianAverages() {
    vector<SongStats> allSongs;
    if (root) {
        root->getAllSongs(allSongs);
    }
    
    for (auto& song : allSongs) {
        SongStats* originalSong = findSong(song.songId);
        if (originalSong) {
            originalSong->calculateBayesianAverage(globalAverage, confidenceFactor);
        }
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

SongStats ExtendedBPlusTree::getBestSong() {
    vector<SongStats> allSongs;
    if (root) {
        root->getAllSongs(allSongs);
    }
    
    if (allSongs.empty()) {
        return SongStats();
    }
    
    auto maxElement = max_element(allSongs.begin(), allSongs.end(),
        [](const SongStats& a, const SongStats& b) {
            return a.averageRating < b.averageRating;
        });
    
    return *maxElement;
}

SongStats ExtendedBPlusTree::getWorstSong() {
    vector<SongStats> allSongs;
    if (root) {
        root->getAllSongs(allSongs);
    }
    
    if (allSongs.empty()) {
        return SongStats();
    }
    
    auto minElement = min_element(allSongs.begin(), allSongs.end(),
        [](const SongStats& a, const SongStats& b) {
            return a.averageRating < b.averageRating;
        });
    
    return *minElement;
}

vector<SongStats> ExtendedBPlusTree::getTopNSongs(int n) {
    vector<SongStats> allSongs;
    if (root) {
        root->getAllSongs(allSongs);
    }
    
    sort(allSongs.begin(), allSongs.end(),
        [](const SongStats& a, const SongStats& b) {
            return a.averageRating > b.averageRating;
        });
    
    if (allSongs.size() > n) {
        allSongs.resize(n);
    }
    
    return allSongs;
}

vector<SongStats> ExtendedBPlusTree::getBottomNSongs(int n) {
    vector<SongStats> allSongs;
    if (root) {
        root->getAllSongs(allSongs);
    }
    
    sort(allSongs.begin(), allSongs.end(),
        [](const SongStats& a, const SongStats& b) {
            return a.averageRating < b.averageRating;
        });
    
    if (allSongs.size() > n) {
        allSongs.resize(n);
    }
    
    return allSongs;
}

SongStats ExtendedBPlusTree::getBestSongBayesian() {
    vector<SongStats> allSongs;
    if (root) {
        root->getAllSongs(allSongs);
    }
    
    if (allSongs.empty()) {
        return SongStats();
    }
    
    auto maxElement = max_element(allSongs.begin(), allSongs.end(),
        [](const SongStats& a, const SongStats& b) {
            return a.bayesianAverage < b.bayesianAverage;
        });
    
    return *maxElement;
}

SongStats ExtendedBPlusTree::getWorstSongBayesian() {
    vector<SongStats> allSongs;
    if (root) {
        root->getAllSongs(allSongs);
    }
    
    if (allSongs.empty()) {
        return SongStats();
    }
    
    auto minElement = min_element(allSongs.begin(), allSongs.end(),
        [](const SongStats& a, const SongStats& b) {
            return a.bayesianAverage < b.bayesianAverage;
        });
    
    return *minElement;
}

vector<SongStats> ExtendedBPlusTree::getTopNSongsBayesian(int n) {
    vector<SongStats> allSongs;
    if (root) {
        root->getAllSongs(allSongs);
    }
    
    sort(allSongs.begin(), allSongs.end(),
        [](const SongStats& a, const SongStats& b) {
            return a.bayesianAverage > b.bayesianAverage;
        });
    
    if (allSongs.size() > n) {
        allSongs.resize(n);
    }
    
    return allSongs;
}

vector<SongStats> ExtendedBPlusTree::getBottomNSongsBayesian(int n) {
    vector<SongStats> allSongs;
    if (root) {
        root->getAllSongs(allSongs);
    }
    
    sort(allSongs.begin(), allSongs.end(),
        [](const SongStats& a, const SongStats& b) {
            return a.bayesianAverage < b.bayesianAverage;
        });
    
    if (allSongs.size() > n) {
        allSongs.resize(n);
    }
    
    return allSongs;
}

void ExtendedBPlusTree::printSongStats(int songId) {
    SongStats* song = findSong(songId);
    if (song && song->ratingCount > 0) {
        cout << "Canción ID: " << song->songId << endl;
        cout << "Número de valoraciones: " << song->ratingCount << endl;
        cout << "Valoración promedio: " << song->averageRating << endl;
        cout << "Valoración bayesiana: " << song->bayesianAverage << endl;
        cout << "Suma total de valoraciones: " << song->totalRating << endl;
    } else {
        cout << "Canción " << songId << " no encontrada o sin valoraciones." << endl;
    }
}

void ExtendedBPlusTree::printAllSongs() {
    vector<SongStats> allSongs;
    if (root) {
        root->getAllSongs(allSongs);
    }
    
    cout << "\n=== TODAS LAS CANCIONES (Promedio Simple) ===" << endl;
    for (const auto& song : allSongs) {
        cout << "Canción " << song.songId 
            << " - Promedio: " << song.averageRating 
            << " (" << song.ratingCount << " valoraciones)" << endl;
    }
}

void ExtendedBPlusTree::printAllSongsBayesian() {
    vector<SongStats> allSongs;
    if (root) {
        root->getAllSongs(allSongs);
    }
    
    cout << "\n=== TODAS LAS CANCIONES (Promedio Bayesiano) ===" << endl;
    cout << "Promedio global: " << globalAverage << " | Factor de confianza: " << confidenceFactor << endl;
    for (const auto& song : allSongs) {
        cout << "Canción " << song.songId 
            << " - Bayesiano: " << song.bayesianAverage 
            << " | Simple: " << song.averageRating 
            << " (" << song.ratingCount << " valoraciones)" << endl;
    }
}
