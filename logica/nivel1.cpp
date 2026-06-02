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
    exito(false),
    atacando(false),
    ultimoImpactoX(0.0f),
    ultimoImpactoY(0.0f),
    hayImpactoNuevo(false),
    pendienteTerminar(false)
{
    if (!dificultad)
        throw invalid_argument("La dificultad no puede ser nula.");
    if (tiempoSegundos <= 0)
        throw invalid_argument("El tiempo debe ser positivo.");

    jugador = new Jugador(450.0f, 350.0f);
    jugador->setLimites(900.0f, 700.0f);

    temblor = new FisicaTemblor(
        dif->getAmplitudTemblor(),
        15.0f, 2.5f, 2.5f
        );
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

    verificarColisionRocaJugador();
    limpiarInactivos();

    if (pendienteTerminar) terminar();
}

void Nivel1::iniciarAtaque() {
    if (!activo || atacando) return;
    atacando = true;
}

void Nivel1::registrarImpacto(short idxRoca) {
    if (idxRoca < 0 || idxRoca >= (short)rocas.size()) return;
    Roca* r = rocas[idxRoca];
    if (!r->estaActivo()) return;

    ultimoImpactoX  = r->getX();
    ultimoImpactoY  = r->getY();
    hayImpactoNuevo = true;

    r->destruir();
    rocasDestruidas++;
    jugador->sumarPunto();

    if (rocasDestruidas >= dif->getRocasObjetivo())
        pendienteTerminar = true;
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

void Nivel1::verificarColisionRocaJugador() {
    for (Roca* r : rocas) {
        if (!r->estaActivo()) continue;

        float dx   = jugador->getX() - r->getX();
        float dy   = jugador->getY() - r->getY();
        float dist = sqrt(dx*dx + dy*dy);

        if (dist <= r->getRadio() + RADIO_JUGADOR) {
            jugador->perderVida();
            r->desactivar();
            if (!jugador->estaVivo()) terminar();
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
    activo            = false;
    atacando          = false;
    pendienteTerminar = false;
    exito = (rocasDestruidas >= dif->getRocasObjetivo())
            && jugador->estaVivo();
}
