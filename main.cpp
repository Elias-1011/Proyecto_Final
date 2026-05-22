#include <iostream>
#include <cstdlib>
#include <ctime>

#include "nivel1.h"
#include "dificultadfacil.h"
#include "dificultaddificil.h"

using namespace std;

int main() {
    srand((unsigned)time(nullptr));

    cout << "=== IGNEOS: prueba de logica Nivel 1 ===\n\n";
    cout << "Dificultad (0 = Facil, 1 = Dificil): ";
    int op;
    cin >> op;

    cout << "Tiempo en segundos (60 normal, 15 sustentacion): ";
    short tiempo;
    cin >> tiempo;

    Dificultad* dif = (op == 1)
                          ? (Dificultad*) new DificultadDificil()
                          : (Dificultad*) new DificultadFacil();

    Nivel1* nivel = new Nivel1(dif, tiempo);

    cout << "\nRocas objetivo : " << nivel->getRocasObj()  << "\n";
    cout << "Max simultaneas: " << nivel->getMaxSimult() << "\n";
    cout << "Tiempo         : " << nivel->getTiempo()    << "s\n\n";

    const float DT         = 0.016f;
    float tiempoAcum       = 0.0f;
    float siguienteSpawn   = 0.0f;
    float siguienteTemblor = 8.0f;
    float siguienteAtaque  = 0.8f;
    float siguienteTick    = 1.0f;
    bool  atacoYa          = false;

    while (tiempoAcum < (float)tiempo && !nivel->nivelTerminado()) {

        if (tiempoAcum >= siguienteSpawn) {
            float rx = 100.0f + (rand() % 600);
            float ry = 100.0f + (rand() % 400);
            nivel->spawnRoca(rx, ry);
            siguienteSpawn += dif->getIntervaloSpawnMs() / 1000.0f;
            cout << "[t=" << tiempoAcum << "] Roca spawneada en ("
                 << rx << ", " << ry << ")\n";
        }

        if (tiempoAcum >= siguienteTemblor) {
            nivel->activarTemblor();
            siguienteTemblor += dif->getIntervaloTemblorMs() / 1000.0f;
            cout << "[t=" << tiempoAcum << "] TEMBLOR activado\n";
        }

        if (tiempoAcum >= siguienteAtaque && !atacoYa) {
            nivel->moverJugador(false, false, false, true);
            nivel->accionAtacar();
            atacoYa = true;
            cout << "[t=" << tiempoAcum << "] Jugador ataca\n";
        }

        if (tiempoAcum >= siguienteTick) {
            nivel->tickSegundo();
            siguienteTick += 1.0f;
            cout << "[t=" << tiempoAcum << "] Tiempo restante: "
                 << nivel->getTiempo() << "s\n";
        }

        nivel->actualizar(DT);
        tiempoAcum += DT;
    }

    cout << "\n=== Resultado ===\n";
    cout << "Rocas destruidas : " << nivel->getRocasDestr() << "\n";
    cout << "Objetivo         : " << nivel->getRocasObj()   << "\n";
    cout << "Exitoso          : " << (nivel->fueExitoso() ? "Si" : "No") << "\n";

    delete nivel;
    delete dif;

    cout << "\nMemoria liberada correctamente.\n";
    return 0;
}
