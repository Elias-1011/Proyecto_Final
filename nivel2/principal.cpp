#include "principal.h"

Principal::Principal(float x, float y)
    : Combatiente(x, y)
    , input(nullptr)
    , m_ultimaDireccion(1.0f)
{}

void Principal::procesarEntrada() {
    if (!input) return;

    if (input->izquierda) { m_ultimaDireccion = -1.0f; mover(-1.0f); }
    else if (input->derecha) { m_ultimaDireccion = 1.0f; mover(1.0f); }

    if (input->saltar)   { saltar();                    input->saltar   = false; }
    if (input->esquivar) { esquivar(m_ultimaDireccion); input->esquivar = false; }
    if (input->atacar)   { atacar(m_ultimaDireccion);   input->atacar   = false; }
}

bool Principal::estaMoviendose() const {
    if (!input) return false;
    return input->izquierda || input->derecha;
}
