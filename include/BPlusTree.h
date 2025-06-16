#pragma once
#include "BPlusTreeNode.h"
#include "UsuarioData.h"

class BPlusTree {
public:
    BPlusTreeNode* raiz;
    BPlusTree();
    void insertar(int persona_id, Valoracion v);
    UsuarioData* buscar(int persona_id) const;
};
