#include "../include/RecomendationSystem.h"
#include <iostream>
#include <cmath>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

float calcular_pearson(const UsuarioData* a, const UsuarioData* b) {
    map<int, float> mapaA, mapaB;
    for (const auto& val : a->valoraciones) mapaA[val.cancion_id] = val.valoracion;
    for (const auto& val : b->valoraciones) mapaB[val.cancion_id] = val.valoracion;

    vector<int> comunes;
    for (const auto& [id, _] : mapaA)
        if (mapaB.count(id)) comunes.push_back(id);

    if (comunes.size() < 2) return 0.0;

    float sumA = 0, sumB = 0, sumA2 = 0, sumB2 = 0, sumAB = 0;
    for (int id : comunes) {
        float x = mapaA[id], y = mapaB[id];
        sumA += x; sumB += y;
        sumA2 += x * x; sumB2 += y * y;
        sumAB += x * y;
    }

    int n = comunes.size();
    float num = sumAB - (sumA * sumB / n);
    float den = sqrt((sumA2 - (sumA * sumA / n)) * (sumB2 - (sumB * sumB / n)));
    return den == 0 ? 0 : num / den;
}

void recolectarVecinosCercanos(BPlusTreeNode* hoja, UsuarioData* target, set<int>& visitados, int& restantes, vector<pair<float, UsuarioData*>>& vecinos, bool haciaAdelante) {
    if (!hoja || restantes <= 0) return;

    if (haciaAdelante) {
        for (int i = 0; i < hoja->datos.size() && restantes > 0; ++i) {
            UsuarioData* actual = hoja->datos[i];
            if (!visitados.count(actual->persona_id)) {
                float sim = calcular_pearson(target, actual);
                vecinos.emplace_back(sim, actual);
                visitados.insert(actual->persona_id);
                --restantes;
            }
        }
        recolectarVecinosCercanos(hoja->siguiente, target, visitados, restantes, vecinos, true);
    } else {
        for (int i = hoja->datos.size() - 1; i >= 0 && restantes > 0; --i) {
            UsuarioData* actual = hoja->datos[i];
            if (!visitados.count(actual->persona_id)) {
                float sim = calcular_pearson(target, actual);
                vecinos.emplace_back(sim, actual);
                visitados.insert(actual->persona_id);
                --restantes;
            }
        }
        recolectarVecinosCercanos(hoja->anterior, target, visitados, restantes, vecinos, false);
    }
}

void buscarVecinosSimilares(BPlusTreeNode* hojaInicial, UsuarioData* target, int umbral, vector<pair<float, UsuarioData*>>& vecinos) {
    set<int> visitados;
    visitados.insert(target->persona_id);
    int restantes = umbral;

    recolectarVecinosCercanos(hojaInicial, target, visitados, restantes, vecinos, false);
    recolectarVecinosCercanos(hojaInicial->siguiente, target, visitados, restantes, vecinos, true);
}

void mostrarUsuariosSimilares(const BPlusTree& tree, int usuario_objetivo, int umbral) {
    UsuarioData* target = tree.buscar(usuario_objetivo);
    if (!target) {
        cout << "Usuario no encontrado.\n";
        return;
    }

    BPlusTreeNode* hoja = tree.raiz;
    while (!hoja->isLeaf) {
        int i = 0;
        while (i < hoja->claves.size() && usuario_objetivo >= hoja->claves[i]) i++;
        hoja = hoja->hijos[i];
    }

    vector<pair<float, UsuarioData*>> vecinos;
    buscarVecinosSimilares(hoja, target, umbral, vecinos);

    sort(vecinos.begin(), vecinos.end(), [](auto& a, auto& b) {
        return a.first > b.first;
    });

    cout << "\nUsuarios mas similares al usuario " << usuario_objetivo << ":\n";

    //Se cambia el número 10 aquí para mostrar más o menos usuarios similares
    for (int i = 0; i < min(10, (int)vecinos.size()); ++i) {
        cout << "Usuario " << vecinos[i].second->persona_id
             << " con similitud " << vecinos[i].first << "\n";
    }
}

void recomendarCanciones(const BPlusTree& tree, int usuario_objetivo, int umbral) {
    UsuarioData* target = tree.buscar(usuario_objetivo);
    if (!target) {
        cout << "Usuario no encontrado.\n";
        return;
    }

    BPlusTreeNode* hoja = tree.raiz;
    while (!hoja->isLeaf) {
        int i = 0;
        while (i < hoja->claves.size() && usuario_objetivo >= hoja->claves[i]) i++;
        hoja = hoja->hijos[i];
    }

    vector<pair<float, UsuarioData*>> vecinos;
    buscarVecinosSimilares(hoja, target, umbral, vecinos);

    sort(vecinos.begin(), vecinos.end(), [](auto& a, auto& b) {
        return a.first > b.first;
    });

    map<int, float> recomendaciones;
    set<int> cancionesEscuchadas;
    for (const auto& val : target->valoraciones)
        cancionesEscuchadas.insert(val.cancion_id);

    //Se cambia aqui el número 10 para usar más o menos vecinos en la recomendación
    for (int i = 0; i < min(10, (int)vecinos.size()); ++i) {
        float peso = vecinos[i].first;
        for (const auto& val : vecinos[i].second->valoraciones) {
            if (!cancionesEscuchadas.count(val.cancion_id))
                recomendaciones[val.cancion_id] += peso * val.valoracion;
        }
    }

    priority_queue<pair<float, int>> pq;
    for (const auto& [id, score] : recomendaciones)
        pq.emplace(score, id);

    cout << "\nRecomendaciones para el usuario " << usuario_objetivo << ":\n";

    //Cambia el número 5 aquí para mostrar más o menos canciones recomendadas
    for (int i = 0; i < 5 && !pq.empty(); ++i) {
        auto [score, id] = pq.top(); pq.pop();
        cout << "Cancion " << id << " con score " << score << "\n";
    }
}
