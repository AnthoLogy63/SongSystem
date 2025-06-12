#include <iostream>
#include <iomanip>
#include "ExtendedBPlusTree.h"

using namespace std;

int main() {
    ExtendedBPlusTree songTree;
    
    cout << "=== CARGANDO DATOS DESDE CSV ===" << endl;
    songTree.loadFromCSV("ratings.csv");
    
    cout << fixed << setprecision(4);
    
    cout << "\n=== DEMOSTRACIÓN: ¿POR QUÉ EL BAYESIANO ES MÁS CONFIABLE? ===" << endl;
    
    cout << "\n--- RANKING CON PROMEDIO SIMPLE (Problemático) ---" << endl;
    vector<SongStats> topSimple = songTree.getTopNSongs(10);
    for (int i = 0; i < topSimple.size(); i++) {
        cout << (i + 1) << ". Canción " << topSimple[i].songId 
            << " - Promedio: " << topSimple[i].averageRating 
            << " (Valoraciones: " << topSimple[i].ratingCount << ")" << endl;
    }
    
    cout << "\n--- RANKING CON PROMEDIO BAYESIANO (Más Confiable) ---" << endl;
    songTree.setConfidenceFactor(50);
    vector<SongStats> topBayesian = songTree.getTopNSongsBayesian(10);
    for (int i = 0; i < topBayesian.size(); i++) {
        cout << (i + 1) << ". Canción " << topBayesian[i].songId 
            << " - Bayesiano: " << topBayesian[i].bayesianAverage
            << " (Valoraciones: " << topBayesian[i].ratingCount << ")" << endl;
    }
    
    return 0;
}
