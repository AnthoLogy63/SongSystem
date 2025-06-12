#include <iostream>
#include "BPlusTree.h"
using namespace std;

int main() {
    BPlusTree tree;

    int values[] = {10, 20, 5, 6, 12, 30, 7, 17};
    for (int val : values) {
        tree.insert(val);
    }

    cout << "Recorrido del B+ Tree: ";
    tree.traverse();

    int buscar = 6;
    
    BPlusTreeNode* res = tree.search(buscar);
    if (res) {
        cout << "Clave " << buscar << " encontrada en una hoja.\n";
    } else {
        cout << "Clave " << buscar << " no encontrada.\n";
    }

    tree.printTree();

    return 0;
}