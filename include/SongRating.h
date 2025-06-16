#ifndef SONGRATING_H
#define SONGRATING_H

struct SongStats {
    int songId;
    double totalRating = 0;
    int ratingCount = 0;
    double averageRating = 0;
    double bayesianAverage = 0;

    SongStats() {}
    SongStats(int id) : songId(id) {}

    void addRating(double rating) {
        totalRating += rating;
        ratingCount++;
        averageRating = (ratingCount > 0) ? totalRating / ratingCount : 0.0;
    }

    void calculateBayesianAverage(double globalAvg, int confidence) {
        bayesianAverage = (ratingCount * averageRating + confidence * globalAvg) / (ratingCount + confidence);
    }
};

#endif