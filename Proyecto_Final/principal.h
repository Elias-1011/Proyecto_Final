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

    InputManager* input;

    void procesarEntrada() override;

    float getDireccion() const { return m_ultimaDireccion; }

    bool estaMoviendose() const;

private:
    float m_ultimaDireccion;
};

#endif // PRINCIPAL_H
