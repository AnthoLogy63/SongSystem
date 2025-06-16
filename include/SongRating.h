#ifndef SONGRATING_H
#define SONGRATING_H

struct SongStats {
    int songId;
    double totalRating = 0;
    int ratingCount = 0;
    double averageRating = 0;
    double bayesianAverage = 0;

    SongStats() = default;

    SongStats(int id)
        : songId(id), totalRating(0.0), ratingCount(0),
          averageRating(0.0), bayesianAverage(0.0) {}

    void addRating(double rating) {
        totalRating += rating;
        ratingCount++;
        averageRating = (ratingCount > 0) ? totalRating / ratingCount : 0.0;
    }

    void calculateBayesianAverage(double globalAvg, int confidence) {
        if (ratingCount > 0) {
            bayesianAverage = (ratingCount * averageRating + confidence * globalAvg) /
                              (ratingCount + confidence);
        } else {
            bayesianAverage = globalAvg; // o 0 si prefieres castigar la falta de votos
        }
    }
};

#endif
