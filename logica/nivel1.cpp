#include "nivel1.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>

using namespace std;

Nivel1::Nivel1(Dificultad* dificultad, short tiempoSegundos)
    : dif(dificultad),
    tiempoRestante(tiempoSegundos),
    rocasDestruidas(0),
    activo(true),
    exito(false)
{
    if (!dificultad)
        throw invalid_argument("La dificultad no puede ser nula.");
    if (tiempoSegundos <= 0)
        throw invalid_argument("El tiempo debe ser positivo.");

    jugador = new Jugador(400.0f, 300.0f);
    jugador->setLimites(800.0f, 600.0f);

    temblor = new FisicaTemblor(
        dif->getAmplitudTemblor(),
        15.0f, 2.5f, 2.5f
        );
    lanza = {0, 0, 0, 0, 0, false};
}

Nivel1::~Nivel1() {
    delete jugador;
    delete temblor;
    for (Roca* r : rocas) delete r;
    rocas.clear();
}

void Nivel1::actualizar(float dt) {
    if (!activo) return;

    temblor->actualizar(dt);
    jugador->actualizar(dt);

    for (Roca* r : rocas)
        if (r->estaActivo()) r->actualizar(dt);

    if (lanza.activa)
        actualizarLanza(dt);

    verificarColisiones();
    limpiarInactivos();
}

void Nivel1::actualizarLanza(float dt) {
    float paso = VEL_LANZA * dt;
    lanza.x += lanza.dirX * paso;
    lanza.y += lanza.dirY * paso;
    lanza.distRecorrida += paso;

    if (lanza.distRecorrida >= DIST_MAX_LANZA) {
        lanza.activa = false;
        jugador->finalizarAtaque();
    }
}

void Nivel1::spawnRoca(float rx, float ry) {
    if (!activo) return;
    if ((short)rocas.size() >= dif->getMaxRocasSimultaneas()) return;

    try {
        rocas.push_back(new Roca(
            rx, ry,
            dif->getVelCrecimientoRoca(),
            dif->getTamanoMaxRoca()
            ));
    } catch (const invalid_argument&) {}
}

void Nivel1::activarTemblor() {
    if (activo) temblor->iniciar();
}

void Nivel1::moverJugador(bool arriba, bool abajo,
                          bool izquierda, bool derecha) {
    jugador->moverArriba(arriba);
    jugador->moverAbajo(abajo);
    jugador->moverIzquierda(izquierda);
    jugador->moverDerecha(derecha);
}

void Nivel1::accionAtacar() {
    if (!activo || lanza.activa) return;
    if (jugador->intentarAtacar()) {
        lanza.x             = jugador->getX();
        lanza.y             = jugador->getY();
        lanza.dirX          = jugador->getDirX();
        lanza.dirY          = jugador->getDirY();
        lanza.distRecorrida = 0.0f;
        lanza.activa        = true;
    }
}

void Nivel1::verificarColisiones() {
    if (!lanza.activa) return;

    float px = getLanzaPuntaX();
    float py = getLanzaPuntaY();

    for (Roca* r : rocas) {
        if (!r->estaActivo()) continue;

        float dx   = px - r->getX();
        float dy   = py - r->getY();
        float dist = sqrt(dx*dx + dy*dy);

        if (dist <= r->getRadio()) {
            r->destruir();
            rocasDestruidas++;
            jugador->sumarPunto();
            lanza.activa = false;
            jugador->finalizarAtaque();
            break;
        }
    }
}

void Nivel1::limpiarInactivos() {
    auto fin = remove_if(rocas.begin(), rocas.end(),
                         [](Roca* r) {
                             if (!r->estaActivo()) {
                                 delete r;
                                 return true;
                             }
                             return false;
                         });
    rocas.erase(fin, rocas.end());
}

void Nivel1::tickSegundo() {
    if (!activo) return;
    tiempoRestante--;
    if (tiempoRestante <= 0) terminar();
}

void Nivel1::terminar() {
    activo = false;
    exito  = (rocasDestruidas >= dif->getRocasObjetivo());
}
