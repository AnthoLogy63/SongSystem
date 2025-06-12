#include <iostream>
#include "ExtendedBPlusTreeNode.h"

using namespace std;

ExtendedBPlusTreeNode::ExtendedBPlusTreeNode(bool leaf) {
    isLeaf = leaf;
    keyCount = 0;
    next = nullptr;
    for (int i = 0; i < SONG_ORDER; ++i)
        children[i] = nullptr;
}

void ExtendedBPlusTreeNode::traverse() {
    int i;
    for (i = 0; i < keyCount; ++i) {
        if (!isLeaf)
            children[i]->traverse();
        cout << "Canción " << songs[i].songId 
             << " (Promedio: " << songs[i].averageRating << ") ";
    }
    if (!isLeaf)
        children[i]->traverse();
}

ExtendedBPlusTreeNode* ExtendedBPlusTreeNode::search(int songId) {
    int i = 0;
    while (i < keyCount && songId > songs[i].songId)
        i++;

    if (isLeaf)
        return (i < keyCount && songs[i].songId == songId) ? this : nullptr;

    return children[i]->search(songId);
}

SongStats* ExtendedBPlusTreeNode::findSong(int songId) {
    int i = 0;
    while (i < keyCount && songId > songs[i].songId)
        i++;

    if (isLeaf) {
        return (i < keyCount && songs[i].songId == songId) ? &songs[i] : nullptr;
    }

    return children[i]->findSong(songId);
}

void ExtendedBPlusTreeNode::getAllSongs(std::vector<SongStats>& allSongs) {
    if (isLeaf) {
        // En las hojas, agregar todas las canciones
        for (int i = 0; i < keyCount; i++) {
            if (songs[i].ratingCount > 0) { // Solo canciones con valoraciones
                allSongs.push_back(songs[i]);
            }
        }
    } else {
        // En nodos internos, recorrer TODOS los hijos
        for (int i = 0; i <= keyCount; i++) {
            if (children[i] != nullptr) {
                children[i]->getAllSongs(allSongs);
            }
        }
    }
}

void ExtendedBPlusTreeNode::insertNonFull(const SongStats& song) {
    int i = keyCount - 1;

    if (isLeaf) {
        // Verificar si la canción ya existe
        for (int j = 0; j < keyCount; j++) {
            if (songs[j].songId == song.songId) {
                // Actualizar estadísticas existentes
                songs[j].totalRating += song.totalRating;
                songs[j].ratingCount += song.ratingCount;
                if (songs[j].ratingCount > 0) {
                    songs[j].averageRating = songs[j].totalRating / songs[j].ratingCount;
                }
                return;
            }
        }
        
        // Insertar nueva canción
        while (i >= 0 && songs[i].songId > song.songId) {
            songs[i + 1] = songs[i];
            i--;
        }
        songs[i + 1] = song;
        keyCount++;
    } else {
        while (i >= 0 && songs[i].songId > song.songId)
            i--;

        if (children[i + 1]->keyCount == SONG_ORDER - 1) {
            splitChild(i + 1, children[i + 1]);
            if (songs[i + 1].songId < song.songId)
                i++;
        }
        children[i + 1]->insertNonFull(song);
    }
}

void ExtendedBPlusTreeNode::splitChild(int index, ExtendedBPlusTreeNode* child) {
    ExtendedBPlusTreeNode* newNode = new ExtendedBPlusTreeNode(child->isLeaf);
    newNode->keyCount = (SONG_ORDER - 1) / 2;

    for (int j = 0; j < newNode->keyCount; j++)
        newNode->songs[j] = child->songs[j + (SONG_ORDER / 2)];

    if (!child->isLeaf) {
        for (int j = 0; j <= newNode->keyCount; j++)
            newNode->children[j] = child->children[j + (SONG_ORDER / 2)];
    }

    for (int j = keyCount; j >= index + 1; j--)
        children[j + 1] = children[j];

    children[index + 1] = newNode;

    for (int j = keyCount - 1; j >= index; j--)
        songs[j + 1] = songs[j];

    songs[index] = child->songs[(SONG_ORDER - 1) / 2];
    keyCount++;

    child->keyCount = (SONG_ORDER - 1) / 2;
}
