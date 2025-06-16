#pragma once
#include <vector>

struct Valoracion {
    int cancion_id;
    float valoracion;
};

struct UsuarioData {
    int persona_id;
    std::vector<Valoracion> valoraciones;
};
