#include "../include/lectorFusionado.h"
#include "../include/SongRating.h"
#include "../include/UsuarioData.h"
#include "../include/Valoracion.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// Variables auxiliares globales (útiles para análisis posterior si quieres)
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
                // Guardar en estructuras auxiliares
                userSongRatings[usuario][cancion] = puntuacion;
                songRatings[cancion].push_back(puntuacion);

                // Insertar o actualizar canción en el árbol B+ extendido
                SongStats* existente = arbolCanciones.findSong(cancion);
                if (existente) {
                    existente->addRating(puntuacion);
                } else {
                    SongStats nueva(cancion);
                    nueva.addRating(puntuacion);
                    arbolCanciones.insert(nueva);
                }

                // Insertar o actualizar usuario en el árbol de usuarios
                Valoracion v;
                v.cancion_id = cancion;
                v.valoracion = puntuacion;

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

    // Calcular promedios bayesianos al final
    arbolCanciones.calculateAllBayesianAverages();
}
