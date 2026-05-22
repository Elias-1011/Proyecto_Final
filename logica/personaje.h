#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "entidad.h"

class Personaje : public Entidad {
public:
    Personaje(float x, float y, float velocidad);
    virtual ~Personaje();

    virtual void actualizar(float dt) override = 0;

    float getVelocidad() const { return velocidad; }
    short getVidas()     const { return vidas;     }
    bool  estaVivo()     const { return vidas > 0; }
    void  perderVida();

protected:
    float velocidad;
    short vidas;
};

#endif
