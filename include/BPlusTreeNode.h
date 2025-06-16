#pragma once
#include "UsuarioData.h"

struct BPlusTreeNode {
    bool isLeaf;
    std::vector<int> claves;
    std::vector<BPlusTreeNode*> hijos;
    std::vector<UsuarioData*> datos;
    BPlusTreeNode* siguiente;
    BPlusTreeNode* anterior;

    BPlusTreeNode(bool leaf);
    void insertarEnHoja(int persona_id, Valoracion v);
    UsuarioData* buscarEnHoja(int persona_id);
};
