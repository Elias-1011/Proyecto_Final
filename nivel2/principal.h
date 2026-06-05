#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include "combatiente.h"

struct InputManager {
    bool izquierda = false;
    bool derecha   = false;
    bool saltar    = false;
    bool esquivar  = false;
    bool atacar    = false;
};

class Principal : public Combatiente {
public:
    Principal(float x, float y);

    InputManager* input;

    void  procesarEntrada() override;
    float getDireccion()    const { return m_ultimaDireccion; }
    bool  estaMoviendose()  const override;

private:
    float m_ultimaDireccion;
};

#endif
