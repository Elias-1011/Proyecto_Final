#include "jugador.h"
#include <cmath>
#include <algorithm>

using namespace std;

Jugador::Jugador(float x, float y)
    : Personaje(x, y, 180.0f),
    subiendo(false), bajando(false),
    yendoLeft(false), yendoRight(false),
    atacando(false),
    dirX(1.0f), dirY(0.0f),
    puntos(0),
    limAncho(800.0f), limAlto(600.0f)
{}

Jugador::~Jugador() {}

void Jugador::actualizar(float dt) {
    float dx = 0, dy = 0;

    if (subiendo)   dy -= velocidad * dt;
    if (bajando)    dy += velocidad * dt;
    if (yendoLeft)  dx -= velocidad * dt;
    if (yendoRight) dx += velocidad * dt;

    // Evitar velocidad extra en diagonal
    if (dx != 0.0f && dy != 0.0f) {
        dx *= 0.7071f;
        dy *= 0.7071f;
    }

    if (dx != 0.0f || dy != 0.0f) {
        float mag = sqrt(dx*dx + dy*dy);
        dirX = dx / mag;
        dirY = dy / mag;
    }

    x = max(22.0f, min(x + dx, limAncho - 22.0f));
    y = max(22.0f, min(y + dy, limAlto  - 22.0f));
}

bool Jugador::intentarAtacar() {
    if (atacando) return false;
    atacando = true;
    return true;
}

void Jugador::setLimites(float ancho, float alto) {
    limAncho = ancho;
    limAlto  = alto;
}
