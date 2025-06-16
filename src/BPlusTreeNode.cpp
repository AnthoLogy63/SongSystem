#include "../include/BPlusTreeNode.h"

BPlusTreeNode::BPlusTreeNode(bool leaf) {
    isLeaf = leaf;
    siguiente = nullptr;
    anterior = nullptr;
}

void BPlusTreeNode::insertarEnHoja(int persona_id, Valoracion v) {
    for (size_t i = 0; i < claves.size(); ++i) {
        if (claves[i] == persona_id) {
            datos[i]->valoraciones.push_back(v);
            return;
        }
    }
    claves.push_back(persona_id);
    UsuarioData* nuevo = new UsuarioData{persona_id, {v}};
    datos.push_back(nuevo);
}

UsuarioData* BPlusTreeNode::buscarEnHoja(int persona_id) {
    for (size_t i = 0; i < claves.size(); ++i) {
        if (claves[i] == persona_id)
            return datos[i];
    }
    return nullptr;
}
