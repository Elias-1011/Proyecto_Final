#ifndef JUGADOR_H
#define JUGADOR_H

#include "personaje.h"

class Jugador : public Personaje {
public:
    Jugador(float x, float y);
    ~Jugador() override;

    void actualizar(float dt) override;

    void moverArriba   (bool estado) { subiendo   = estado; }
    void moverAbajo    (bool estado) { bajando    = estado; }
    void moverIzquierda(bool estado) { yendoLeft  = estado; }
    void moverDerecha  (bool estado) { yendoRight = estado; }

    bool intentarAtacar();
    void finalizarAtaque() { atacando = false; }
    bool estaAtacando()    const { return atacando; }

    float getDirX() const { return dirX; }
    float getDirY() const { return dirY; }

    short getPuntos() const { return puntos; }
    void  sumarPunto()      { puntos++;      }

    void setLimites(float ancho, float alto);

private:
    bool  subiendo, bajando, yendoLeft, yendoRight;
    bool  atacando;
    float dirX, dirY;
    short puntos;
    float limAncho, limAlto;
};

#endif
