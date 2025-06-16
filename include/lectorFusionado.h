#ifndef LECTORFUSIONADO_H
#define LECTORFUSIONADO_H

#include "ExtendedBPlusTree.h"
#include "BPlusTree.h"
#include <unordered_map>
#include <vector>

extern std::unordered_map<int, std::unordered_map<int, float>> userSongRatings;
extern std::unordered_map<int, std::vector<float>> songRatings;

void leerCSVConEstructurasYArbol(const std::string& ruta, ExtendedBPlusTree& arbolCanciones, BPlusTree& arbolUsuarios);

#endif
