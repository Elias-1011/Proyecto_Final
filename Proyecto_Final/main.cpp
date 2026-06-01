#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
#include "nivel2.h"

void imprimirLinea(char c = '-', int n = 60) {
    std::cout << std::string(n, c) << "\n";
}

void imprimirEstadoAgente(EstadoAgente e) {
    switch (e) {
    case EstadoAgente::ESPERAR:    std::cout << "ESPERAR   "; break;
    case EstadoAgente::AVANZAR:    std::cout << "AVANZAR   "; break;
    case EstadoAgente::ATACAR:     std::cout << "ATACAR    "; break;
    case EstadoAgente::RETROCEDER: std::cout << "RETROCEDER"; break;
    }
}

// Barra de resistencia visual
std::string barraVida(int toques) {
    std::string s = "[";
    for (int i = 0; i < 3; i++)
        s += (i < toques) ? "X" : "O";
    s += "]";
    return s;
}

// Plataforma visual en ASCII
void imprimirPlataforma(float xJugador, float xEnemigo,
                        float xMin = 50.f, float xMax = 750.f) {
    const int ANCHO = 50;
    std::string linea(ANCHO, '.');
    linea[0] = '|'; linea[ANCHO - 1] = '|';

    // Mapear coordenadas al ancho de la consola
    auto mapear = [&](float x) -> int {
        int pos = static_cast<int>((x - xMin) / (xMax - xMin) * (ANCHO - 2)) + 1;
        if (pos < 1) pos = 1;
        if (pos > ANCHO - 2) pos = ANCHO - 2;
        return pos;
    };

    int posJ = mapear(xJugador);
    int posE = mapear(xEnemigo);

    if (posJ == posE) {
        linea[posJ] = '!';  // colisión visual
    } else {
        linea[posJ] = 'P';  // Principal
        linea[posE] = 'A';  // Agente
    }
    std::cout << linea << "\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// Simulación de input automático para pruebas
// ─────────────────────────────────────────────────────────────────────────────

// Simula que el jugador presiona teclas de forma aleatoria cada N frames
void simularInputJugador(InputManager& input, Nivel_2& nivel, int frame) {
    // Cada 30 frames (~0.5s a 60fps) el jugador hace algo
    if (frame % 30 != 0) return;

    int accion = std::rand() % 5;
    switch (accion) {
    case 0:
        input.derecha = true;
        nivel.registrarAccionJugador(Accion::MOVER);
        break;
    case 1:
        input.izquierda = true;
        nivel.registrarAccionJugador(Accion::MOVER);
        break;
    case 2:
        input.saltar = true;
        nivel.registrarAccionJugador(Accion::SALTAR);
        break;
    case 3:
        input.esquivar = true;
        nivel.registrarAccionJugador(Accion::ESQUIVAR);
        break;
    case 4:
        input.atacar = true;
        nivel.registrarAccionJugador(Accion::ATACAR);
        break;
    }
}

void limpiarInput(InputManager& input) {
    input.izquierda = false;
    input.derecha   = false;
    // saltar/esquivar/atacar ya se consumen dentro de Principal::procesarEntrada
}

// ─────────────────────────────────────────────────────────────────────────────
// Main
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::cout << "\n";
    imprimirLinea('=');
    std::cout << "   PRUEBA LOGICA — NIVEL 2: El Duelo del Crater\n";
    imprimirLinea('=');
    std::cout << "  P = Jugador  |  A = Agente  |  ! = misma posicion\n";
    std::cout << "  Toques: [OOO] = barra llena  [XXX] = 3 toques = punto\n";
    imprimirLinea();

    Nivel_2 nivel;
    nivel.iniciar();

    const float DT        = 1.0f / 60.0f;  // 60 fps simulados
    const int   MAX_FRAMES = 60 * 120;      // 2 minutos máximo de simulación
    int         frame      = 0;
    int         puntoAnteriorJ = 0;
    int         puntoAnteriorA = 0;

    while (!nivel.nivelFinalizado() && frame < MAX_FRAMES) {

        // Simular input del jugador
        simularInputJugador(nivel.getInput(), nivel, frame);

        // Actualizar lógica
        nivel.actualizar(DT);

        // Limpiar flags de movimiento continuo
        limpiarInput(nivel.getInput());

        // ── Imprimir estado cada 60 frames (≈ cada segundo) ──────────────────
        if (frame % 60 == 0) {
            float xJ = nivel.getJugador()->getX();
            float xA = nivel.getEnemigo()->getX();
            float dist = nivel.getDistanciaEntrePersonajes();

            std::cout << "\n[t=" << std::setw(4) << frame / 60 << "s] "
                      << "P:" << std::setw(6) << std::fixed << std::setprecision(1) << xJ
                      << "  A:" << std::setw(6) << xA
                      << "  dist:" << std::setw(6) << dist
                      << "\n";

            // Plataforma ASCII
            imprimirPlataforma(xJ, xA);

            // Barras de resistencia
            std::cout << "  Jugador " << barraVida(nivel.getToquesJugador())
                      << "  Agente " << barraVida(nivel.getToquesAgente())
                      << "\n";

            // Marcador
            std::cout << "  Marcador -> Jugador: " << nivel.getPuntosJugador()
                      << "  |  Agente: " << nivel.getPuntosAgente() << "\n";

            // Estado del agente
            std::cout << "  Estado agente: ";
            imprimirEstadoAgente(nivel.getEnemigo()->getEstado());
            std::cout << "\n";

            // Temblor activo
            if (nivel.getTemblorActivo())
                std::cout << "  *** TEMBLOR SISMICO ACTIVO ***\n";
        }

        // ── Detectar cambio de punto ──────────────────────────────────────────
        if (nivel.getPuntosJugador() != puntoAnteriorJ ||
            nivel.getPuntosAgente()  != puntoAnteriorA) {

            imprimirLinea('*');
            if (nivel.getPuntosJugador() > puntoAnteriorJ)
                std::cout << "  >>> PUNTO para el JUGADOR! ("
                          << nivel.getPuntosJugador() << " - "
                          << nivel.getPuntosAgente() << ")\n";
            else
                std::cout << "  >>> PUNTO para el AGENTE! ("
                          << nivel.getPuntosJugador() << " - "
                          << nivel.getPuntosAgente() << ")\n";
            imprimirLinea('*');

            puntoAnteriorJ = nivel.getPuntosJugador();
            puntoAnteriorA = nivel.getPuntosAgente();
        }

        frame++;
    }

    // ── Resultado final ───────────────────────────────────────────────────────
    std::cout << "\n";
    imprimirLinea('=');
    if (nivel.nivelFinalizado()) {
        if (nivel.jugadorGano())
            std::cout << "  RESULTADO: VICTORIA del JUGADOR\n";
        else
            std::cout << "  RESULTADO: VICTORIA del AGENTE\n";
        std::cout << "  Marcador final: Jugador " << nivel.getPuntosJugador()
                  << " — Agente " << nivel.getPuntosAgente() << "\n";
    } else {
        std::cout << "  Simulacion terminada por tiempo limite (" << MAX_FRAMES/60 << "s)\n";
        std::cout << "  Marcador: Jugador " << nivel.getPuntosJugador()
                  << " — Agente " << nivel.getPuntosAgente() << "\n";
    }
    std::cout << "  Frames simulados: " << frame
              << " (≈" << frame/60 << "s a 60fps)\n";
    imprimirLinea('=');

    std::cout << "\nPresiona ENTER para salir...";
    std::cin.get();

    return 0;
}
