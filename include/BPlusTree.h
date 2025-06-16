#pragma once
#include "BPlusTreeNode.h"

class BPlusTree {
private:
    BPlusTreeNode* raiz;

public:
    BPlusTree();
    void insertar(int persona_id, Valoracion v);
    UsuarioData* buscar(int persona_id);
};
