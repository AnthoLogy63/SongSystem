// SongRating.h
#ifndef SONGRATING_H
#define SONGRATING_H

#include <vector>
#include <algorithm>

struct Rating {
    int userId;
    int songId;
    double rating;
    
    Rating() : userId(0), songId(0), rating(0.0) {}
    Rating(int u, int s, double r) : userId(u), songId(s), rating(r) {}
};

// Estructura para almacenar estadísticas de cada canción
struct SongStats {
    int songId;
    double totalRating;
    int ratingCount;
    double averageRating;
    double bayesianAverage;
    
    SongStats() : songId(0), totalRating(0.0), ratingCount(0), averageRating(0.0), bayesianAverage(0.0) {}
    SongStats(int id) : songId(id), totalRating(0.0), ratingCount(0), averageRating(0.0), bayesianAverage(0.0) {}
    
    void addRating(double rating) {
        if (rating > 0) { // Solo contar valoraciones válidas (no cero)
            totalRating += rating;
            ratingCount++;
            averageRating = totalRating / ratingCount;
        }
    }
    
    // Calcular promedio bayesiano
    void calculateBayesianAverage(double globalAverage, int confidenceFactor) {
        if (ratingCount == 0) {
            bayesianAverage = 0.0;
            return;
        }
        
        // Fórmula del promedio bayesiano
        bayesianAverage = (confidenceFactor * globalAverage + totalRating) / 
                         (confidenceFactor + ratingCount);
    }
    
    bool operator<(const SongStats& other) const {
        return songId < other.songId;
    }
    
    bool operator==(const SongStats& other) const {
        return songId == other.songId;
    }
};

#endif
