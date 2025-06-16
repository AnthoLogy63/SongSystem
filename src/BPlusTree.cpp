#include "BPlusTree.h"

BPlusTree::BPlusTree() {
    raiz = new BPlusTreeNode(true);
}

void BPlusTree::insertar(int persona_id, Valoracion v) {
    raiz->insertarEnHoja(persona_id, v);
}

UsuarioData* BPlusTree::buscar(int persona_id) {
    return raiz->buscarEnHoja(persona_id);
}
