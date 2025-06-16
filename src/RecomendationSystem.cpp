#include "../include/RecomendationSystem.h"
#include <iostream>
#include <cmath>
#include <map>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

float calcular_pearson(const UsuarioData* a, const UsuarioData* b) {
    map<int, float> mapaA, mapaB;
    for (list<Valoracion>::const_iterator it = a->valoraciones.begin(); it != a->valoraciones.end(); ++it)
        mapaA[it->cancion_id] = it->valoracion;
    for (list<Valoracion>::const_iterator it = b->valoraciones.begin(); it != b->valoraciones.end(); ++it)
        mapaB[it->cancion_id] = it->valoracion;

    vector<int> comunes;
    for (map<int, float>::iterator it = mapaA.begin(); it != mapaA.end(); ++it)
        if (mapaB.count(it->first)) comunes.push_back(it->first);

    if (comunes.size() < 2) return 0.0;

    float sumA = 0, sumB = 0, sumA2 = 0, sumB2 = 0, sumAB = 0;
    for (size_t i = 0; i < comunes.size(); ++i) {
        int id = comunes[i];
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

void mostrarUsuariosSimilares(BPlusTree& tree, int usuario_objetivo) {
    UsuarioData* target = tree.buscar(usuario_objetivo);
    if (!target) {
        cout << "Usuario no encontrado." << endl;
        return;
    }

    vector<UsuarioData*> todos;
    for (int id = 1; id <= 10000; ++id) {
        UsuarioData* u = tree.buscar(id);
        if (u && u->persona_id != usuario_objetivo)
            todos.push_back(u);
    }

    vector<pair<float, UsuarioData*> > similares;
    for (size_t i = 0; i < todos.size(); ++i) {
        float sim = calcular_pearson(target, todos[i]);
        if (sim > 0)
            similares.push_back(make_pair(sim, todos[i]));
    }

    sort(similares.begin(), similares.end(), [](const pair<float, UsuarioData*>& a, const pair<float, UsuarioData*>& b) {
        return a.first > b.first;
    });

    cout << "Usuarios más similares al usuario " << usuario_objetivo << ":\n";
    for (size_t i = 0; i < min(similares.size(), size_t(10)); ++i) {
        cout << "Usuario " << similares[i].second->persona_id
             << " con similitud " << similares[i].first << "\n";
    }
}

void recomendarCanciones(BPlusTree& tree, int usuario_objetivo) {
    UsuarioData* target = tree.buscar(usuario_objetivo);
    if (!target) {
        cout << "Usuario no encontrado." << endl;
        return;
    }

    vector<UsuarioData*> todos;
    for (int id = 1; id <= 10000; ++id) {
        UsuarioData* u = tree.buscar(id);
        if (u && u->persona_id != usuario_objetivo)
            todos.push_back(u);
    }

    vector<pair<float, UsuarioData*> > similares;
    for (size_t i = 0; i < todos.size(); ++i) {
        float sim = calcular_pearson(target, todos[i]);
        if (sim > 0)
            similares.push_back(make_pair(sim, todos[i]));
    }

    map<int, float> recomendaciones;
    set<int> canciones_objetivo;
    for (list<Valoracion>::const_iterator it = target->valoraciones.begin(); it != target->valoraciones.end(); ++it)
        canciones_objetivo.insert(it->cancion_id);

    for (size_t i = 0; i < min(similares.size(), size_t(5)); ++i) {
        float peso = similares[i].first;
        for (list<Valoracion>::const_iterator it = similares[i].second->valoraciones.begin(); it != similares[i].second->valoraciones.end(); ++it) {
            if (!canciones_objetivo.count(it->cancion_id))
                recomendaciones[it->cancion_id] += peso * it->valoracion;
        }
    }

    vector<pair<float, int> > top;
    for (map<int, float>::iterator it = recomendaciones.begin(); it != recomendaciones.end(); ++it) {
        top.push_back(make_pair(it->second, it->first));
    }

    sort(top.begin(), top.end(), greater<pair<float, int> >());

    cout << "\nRecomendaciones para el usuario " << usuario_objetivo << ":\n";
    for (size_t i = 0; i < min(top.size(), size_t(5)); ++i) {
        cout << "Canción " << top[i].second << " con score " << top[i].first << "\n";
    }
}
