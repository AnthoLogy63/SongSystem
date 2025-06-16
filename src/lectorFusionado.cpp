#include "../include/lectorFusionado.h"
#include "../include/SongRating.h"
#include "../include/UsuarioData.h" // ✅ Ya contiene struct Valoracion
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

unordered_map<int, unordered_map<int, float>> userSongRatings;
unordered_map<int, vector<float>> songRatings;

void leerCSVConEstructurasYArbol(const string& ruta, ExtendedBPlusTree& arbolCanciones, BPlusTree& arbolUsuarios) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cerr << "❌ No se pudo abrir el archivo CSV.\n";
        return;
    }

    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        stringstream ss(linea);
        string usuarioStr, cancionStr, puntuacionStr, basura;
        if (getline(ss, usuarioStr, ',') &&
            getline(ss, cancionStr, ',') &&
            getline(ss, puntuacionStr, ',') &&
            getline(ss, basura, ',')) {

            int usuario = stoi(usuarioStr);
            int cancion = stoi(cancionStr);
            float puntuacion = stof(puntuacionStr);

            if (puntuacion > 0.0f) {
                userSongRatings[usuario][cancion] = puntuacion;
                songRatings[cancion].push_back(puntuacion);

                SongStats* existente = arbolCanciones.findSong(cancion);
                if (existente) {
                    existente->addRating(puntuacion);
                } else {
                    SongStats nueva(cancion);
                    nueva.addRating(puntuacion);
                    arbolCanciones.insert(nueva);
                }

                Valoracion v{cancion, puntuacion};

                UsuarioData* user = arbolUsuarios.buscar(usuario);
                if (!user) {
                    arbolUsuarios.insertar(usuario, v);
                } else {
                    user->valoraciones.push_back(v);
                }
            }
        }
    }

    archivo.close();
    arbolCanciones.calculateAllBayesianAverages();
}
