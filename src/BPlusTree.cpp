#include "../include/BPlusTree.h"
#include "../include/BPlusTreeNode.h"

BPlusTree::BPlusTree() {
    raiz = new BPlusTreeNode(true);
}

void BPlusTree::insertar(int persona_id, Valoracion v) {
    if (!raiz) {
        raiz = new BPlusTreeNode(true);
    }

    BPlusTreeNode* actual = raiz;
    while (!actual->isLeaf) {
        int i = 0;
        while (i < actual->claves.size() && persona_id >= actual->claves[i]) i++;
        actual = actual->hijos[i];
    }

    actual->insertarEnHoja(persona_id, v);
}

UsuarioData* BPlusTree::buscar(int persona_id) const {
    BPlusTreeNode* actual = raiz;
    while (actual && !actual->isLeaf) {
        int i = 0;
        while (i < actual->claves.size() && persona_id >= actual->claves[i]) i++;
        actual = actual->hijos[i];
    }

    if (!actual) return nullptr;

    for (auto* dato : actual->datos) {
        if (dato->persona_id == persona_id)
            return dato;
    }
    return nullptr;
}
