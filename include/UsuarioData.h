#pragma once
#include "Valoracion.h"
#include <list>

struct UsuarioData {
    int persona_id;
    std::list<Valoracion> valoraciones;
};
