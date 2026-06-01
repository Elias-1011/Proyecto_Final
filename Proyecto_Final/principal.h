#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include "personaje.h"

struct InputManager {
    bool izquierda = false;
    bool derecha   = false;
    bool saltar    = false;
    bool esquivar  = false;
    bool atacar    = false;
};

class Principal : public Personaje {
public:
    Principal(float x, float y);

    // Referencia al InputManager; Nivel_2 lo actualiza con eventos de teclado
    InputManager* input;

    // Procesa el InputManager y ejecuta las acciones físicas correspondientes
    void procesarEntrada() override;

    // Última dirección de movimiento (para saber hacia dónde esquivar/atacar)
    float getDireccion() const { return m_ultimaDireccion; }

private:
    float m_ultimaDireccion; // +1.0 derecha, -1.0 izquierda
};

#endif // PRINCIPAL_H
