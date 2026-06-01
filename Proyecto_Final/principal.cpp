#include "principal.h"

Principal::Principal(float x, float y)
    : Personaje(x, y)
    , input(nullptr)
    , m_ultimaDireccion(1.0f)  // empieza mirando a la derecha
{}

void Principal::procesarEntrada() {
    if (!input) return;

    // Movimiento horizontal
    if (input->izquierda) {
        m_ultimaDireccion = -1.0f;
        mover(-1.0f);
    } else if (input->derecha) {
        m_ultimaDireccion = 1.0f;
        mover(1.0f);
    }

    // Salto
    if (input->saltar) {
        saltar();
        input->saltar = false;   // consumir evento puntual
    }

    // Esquive
    if (input->esquivar) {
        esquivar(m_ultimaDireccion);
        input->esquivar = false; // consumir
    }

    // Ataque
    if (input->atacar) {
        atacar(m_ultimaDireccion);
        input->atacar = false;   // consumir
    }
}
