#ifndef RECOMENDATION_SYSTEM_H
#define RECOMENDATION_SYSTEM_H

#include "BPlusTree.h"
#include <vector>

float calcular_pearson(const UsuarioData* a, const UsuarioData* b);
void buscarVecinosSimilares(BPlusTreeNode* hojaInicial, UsuarioData* target, int umbral, std::vector<std::pair<float, UsuarioData*>>& vecinos);
void mostrarUsuariosSimilares(const BPlusTree& tree, int usuario_objetivo, int umbral);
void recomendarCanciones(const BPlusTree& tree, int usuario_objetivo, int umbral);

#endif
