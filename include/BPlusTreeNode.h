#pragma once
#include <vector>
#include "UsuarioData.h"

using namespace std;

class BPlusTreeNode {
public:
    bool isLeaf;
    vector<int> claves;
    vector<BPlusTreeNode*> hijos;
    vector<UsuarioData*> datos;
    BPlusTreeNode* siguiente;

    BPlusTreeNode(bool leaf);
    void insertarEnHoja(int persona_id, Valoracion v);
    UsuarioData* buscarEnHoja(int persona_id);
};
