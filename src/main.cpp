#include <iostream>
#include "../include/ExtendedBPlusTree.h"
#include "../include/lectorFusionado.h"
#include "../include/BPlusTree.h"
#include "../include/RecomendationSystem.h"

using namespace std;

int main() {
    ExtendedBPlusTree arbolCanciones;
    BPlusTree arbolUsuarios;

    string rutaCSV = "data/ratings_s.csv";
    leerCSVConEstructurasYArbol(rutaCSV, arbolCanciones, arbolUsuarios);
    arbolCanciones.calculateAllBayesianAverages();

    int opcion;
    do {
        cout << "\n========= MENÚ PRINCIPAL =========\n";
        cout << "1. Top N canciones (promedio bayesiano)\n";
        cout << "2. Peores N canciones (promedio bayesiano)\n";
        cout << "3. Mostrar usuarios similares\n";
        cout << "4. Recomendaciones para un usuario\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        if (opcion == 1) {
            int n;
            cout << "¿Cuántas canciones deseas ver?: ";
            cin >> n;
            vector<SongStats> top = arbolCanciones.getTopNSongsBayesian(n);
            for (const auto& s : top) {
                cout << "🎵 Canción " << s.songId 
                     << " | Bayesiano: " << s.bayesianAverage 
                     << " | Promedio: " << s.averageRating 
                     << " | Votos: " << s.ratingCount << "\n";
            }
        } else if (opcion == 2) {
            int n;
            cout << "¿Cuántas canciones peores deseas ver?: ";
            cin >> n;
            vector<SongStats> bottom = arbolCanciones.getBottomNSongsBayesian(n);
            for (const auto& s : bottom) {
                cout << "🔻 Canción " << s.songId 
                     << " | Bayesiano: " << s.bayesianAverage 
                     << " | Promedio: " << s.averageRating 
                     << " | Votos: " << s.ratingCount << "\n";
            }
        } else if (opcion == 3) {
            int id;
            cout << "Ingrese ID del usuario: ";
            cin >> id;
            mostrarUsuariosSimilares(arbolUsuarios, id);
        } else if (opcion == 4) {
            int id;
            cout << "Ingrese ID del usuario: ";
            cin >> id;
            recomendarCanciones(arbolUsuarios, id);
        } else if (opcion == 0) {
            cout << "Saliendo del sistema...\n";
        } else {
            cout << "❌ Opción no válida.\n";
        }

    } while (opcion != 0);

    return 0;
}
