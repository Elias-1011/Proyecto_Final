#include "enemigo.h"
#include "nivel2.h"
#include "principal.h"
#include <cstdlib>
#include <cmath>

Enemigo::Enemigo(float x, float y, Nivel_2* referenciaEstado)
    : Personaje(x, y)
    , m_referenciaEstado(referenciaEstado)
    , m_estadoActual(EstadoAgente::ESPERAR)
    , m_distanciaAlJugador(0.0f)
    , m_ultimaAccionJugador(Accion::MOVER)
    , m_temblor_activo(false)
    , m_marcadorAgente(0)
    , m_marcadorJugador(0)
    , m_distanciaObjetivo(160.0f)
    , m_distanciaMinima(80.0f)
    , m_tiempoEsperaMin(0.5f)
    , m_tiempoEsperaMax(1.5f)
    , m_tiempoEsperaActual(0.8f)
    , m_tiempoTranscurrido(0.0f)
{}

void Enemigo::actualizar(float dt) {
    percibir();
    razonar();
    actuar(dt);
    Personaje::actualizar(dt);  // física base (movimiento, salto, embestida, cooldown)
}

void Enemigo::percibir() {
    m_distanciaAlJugador   = m_referenciaEstado->getDistanciaEntrePersonajes();
    m_ultimaAccionJugador  = m_referenciaEstado->getUltimaAccionJugador();
    m_temblor_activo       = m_referenciaEstado->getTemblorActivo();
    m_marcadorAgente       = m_referenciaEstado->getPuntosAgente();
    m_marcadorJugador      = m_referenciaEstado->getPuntosJugador();

    // Registrar la acción del jugador en el historial del punto actual
    m_historial.push_back(m_ultimaAccionJugador);
}

void Enemigo::razonar() {
    // Siempre prioritario: si el jugador está muy cerca, retroceder
    if (jugadorEstaCerca()) {
        float probRetroceder = m_perfil.probRetrocederSiCerca;
        float r = static_cast<float>(std::rand()) / RAND_MAX;
        if (r < probRetroceder) {
            m_estadoActual = EstadoAgente::RETROCEDER;
            return;
        }
    }

    switch (m_estadoActual) {

    case EstadoAgente::ESPERAR:
        m_tiempoTranscurrido += 0.016f; // aprox. un frame a 60fps; se sobreescribe en actuar
        if (m_tiempoTranscurrido >= m_tiempoEsperaActual) {
            m_tiempoTranscurrido = 0.0f;
            // Decidir: avanzar o atacar si ya estamos en rango
            if (m_distanciaAlJugador <= m_distanciaObjetivo && puedeAtacar()) {
                m_estadoActual = EstadoAgente::ATACAR;
            } else {
                m_estadoActual = EstadoAgente::AVANZAR;
            }
        }
        break;

    case EstadoAgente::AVANZAR:
        // Al llegar al rango objetivo, pasar a esperar o atacar
        if (m_distanciaAlJugador <= m_distanciaObjetivo) {
            if (puedeAtacar()) {
                m_estadoActual = EstadoAgente::ATACAR;
            } else {
                m_estadoActual = EstadoAgente::ESPERAR;
                m_tiempoEsperaActual = randomEntre(m_tiempoEsperaMin, m_tiempoEsperaMax);
            }
        }
        break;

    case EstadoAgente::ATACAR:
        // Tras la embestida, volver a esperar
        if (!m_embistiendo && m_cooldownRestante > 0.0f) {
            m_estadoActual = EstadoAgente::ESPERAR;
            m_tiempoEsperaActual = randomEntre(m_tiempoEsperaMin, m_tiempoEsperaMax);
        }
        break;

    case EstadoAgente::RETROCEDER:
        // Al recuperar distancia segura, volver a esperar
        if (m_distanciaAlJugador >= m_distanciaObjetivo) {
            m_estadoActual = EstadoAgente::ESPERAR;
            m_tiempoEsperaActual = randomEntre(m_tiempoEsperaMin, m_tiempoEsperaMax);
        }
        break;
    }
}

void Enemigo::actuar(float dt) {
    float dirAlJugador = calcularDireccionAlJugador();

    switch (m_estadoActual) {

    case EstadoAgente::ESPERAR:
        m_tiempoTranscurrido += dt;  // acumular tiempo real en lugar de frame fijo
        // Sin movimiento
        break;

    case EstadoAgente::AVANZAR: {
        // Ajuste de dirección basado en aprendizaje: si el jugador esquiva
        // consistentemente hacia un lado, el agente puede cortar por el otro
        float dirAvance = dirAlJugador;
        float r = static_cast<float>(std::rand()) / RAND_MAX;
        if (r < m_perfil.probAvanzarDirOpuesta) {
            // avanzar en la dirección "inteligente" (ya codificada en el perfil)
            dirAvance = dirAlJugador;
        }
        mover(dirAvance);
        break;
    }

    case EstadoAgente::ATACAR:
        // Solo atacar si el cooldown lo permite y hay proyectil libre
        if (puedeAtacar()) {
            // Verificar que la distancia es válida (rango de ataque)
            if (m_distanciaAlJugador <= m_distanciaObjetivo + 20.0f) {
                atacar(dirAlJugador);
            }
        }
        break;

    case EstadoAgente::RETROCEDER:
        // Alejarse del jugador
        mover(-dirAlJugador);
        break;
    }
}

void Enemigo::aprender() {
    m_perfil.totalPuntos++;

    Principal* jugador = m_referenciaEstado->getJugador();

    // Analizar el historial del punto recién terminado
    for (Accion a : m_historial) {
        if (a == Accion::ATACAR) {
            // Registrar si el ataque fue de corto o medio rango
            if (m_distanciaAlJugador < m_distanciaMinima * 1.5f) {
                m_perfil.ataquesCortoRango++;
            } else {
                m_perfil.ataquesMedioRango++;
            }
        } else if (a == Accion::ESQUIVAR) {
            // Determinar dirección del esquive comparando posición con el agente
            if (jugador->getX() < m_x) {
                m_perfil.esquivesIzquierda++;
            } else {
                m_perfil.esquivesDerecha++;
            }
        } else if (a == Accion::SALTAR) {
            m_perfil.saltos++;
        }
    }

    m_historial.clear();
    m_perfil.recalcularProbabilidades();
}

float Enemigo::calcularDireccionAlJugador() const {
    Principal* jugador = m_referenciaEstado->getJugador();
    float dx = jugador->getX() - m_x;
    return (dx >= 0.0f) ? 1.0f : -1.0f;
}

bool Enemigo::jugadorEstaCerca() const {
    return m_distanciaAlJugador < m_distanciaMinima;
}

float Enemigo::randomEntre(float min, float max) const {
    float r = static_cast<float>(std::rand()) / RAND_MAX;
    return min + r * (max - min);
}
