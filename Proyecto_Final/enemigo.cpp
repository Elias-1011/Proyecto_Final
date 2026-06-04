#include "enemigo.h"
#include "nivel2.h"
#include "principal.h"
#include <cstdlib>
#include <cmath>

Enemigo::Enemigo(float x, float y, Nivel_2* referenciaEstado)
    : Personaje(x, y),
    m_referenciaEstado(referenciaEstado),
    m_estadoActual(EstadoAgente::ESPERAR),
    m_distanciaAlJugador(0.0f),
    m_ultimaAccionJugador(Accion::MOVER),
    m_temblor_activo(false),
    m_marcadorAgente(0),
    m_marcadorJugador(0),
    m_distanciaObjetivo(160.0f),
    m_distanciaMinima(80.0f),
    m_tiempoEsperaMin(0.5f),
    m_tiempoEsperaMax(1.5f),
    m_tiempoEsperaActual(0.8f),
    m_tiempoTranscurrido(0.0f),
    m_probSaltar(0.3f),
    m_probEsquivar(0.4f)
{}

void Enemigo::actualizar(float dt) {
    percibir();
    razonar();
    actuar(dt);
    Personaje::actualizar(dt);
}

void Enemigo::percibir() {
    m_distanciaAlJugador   = m_referenciaEstado->getDistanciaEntrePersonajes();
    m_ultimaAccionJugador  = m_referenciaEstado->getUltimaAccionJugador();
    m_temblor_activo       = m_referenciaEstado->getTemblorActivo();
    m_marcadorAgente       = m_referenciaEstado->getPuntosAgente();
    m_marcadorJugador      = m_referenciaEstado->getPuntosJugador();
    m_historial.push_back(m_ultimaAccionJugador);
}

void Enemigo::razonar() {
    bool jugadorAtacando = (m_ultimaAccionJugador == Accion::ATACAR);
    bool puedeReaccionar = m_enSuelo && !m_embistiendo && !m_recuperando &&
                           m_estadoActual != EstadoAgente::ESQUIVAR &&
                           m_estadoActual != EstadoAgente::SALTAR;

    if (jugadorAtacando && puedeReaccionar) {
        float r = static_cast<float>(std::rand()) / RAND_MAX;
        if (r < m_probEsquivar) {
            m_estadoActual = EstadoAgente::ESQUIVAR;
            return;
        } else if (r < m_probEsquivar + m_probSaltar) {
            m_estadoActual = EstadoAgente::SALTAR;
            return;
        }
    }

    if (jugadorEstaCerca()) {
        float r = static_cast<float>(std::rand()) / RAND_MAX;
        if (r < m_perfil.probRetrocederSiCerca) {
            m_estadoActual = EstadoAgente::RETROCEDER;
            return;
        }
    }

    switch (m_estadoActual) {

    case EstadoAgente::ESPERAR:
        m_tiempoTranscurrido += 0.016f;
        if (m_tiempoTranscurrido >= m_tiempoEsperaActual) {
            m_tiempoTranscurrido = 0.0f;
            if (m_distanciaAlJugador <= m_distanciaObjetivo && puedeAtacar()) {
                m_estadoActual = EstadoAgente::ATACAR;
            } else {
                m_estadoActual = EstadoAgente::AVANZAR;
            }
        }
        break;

    case EstadoAgente::AVANZAR:
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
        if (!m_embistiendo && m_cooldownRestante > 0.0f) {
            m_estadoActual = EstadoAgente::ESPERAR;
            m_tiempoEsperaActual = randomEntre(m_tiempoEsperaMin, m_tiempoEsperaMax);
        }
        break;

    case EstadoAgente::RETROCEDER:
        if (m_distanciaAlJugador >= m_distanciaObjetivo) {
            m_estadoActual = EstadoAgente::ESPERAR;
            m_tiempoEsperaActual = randomEntre(m_tiempoEsperaMin, m_tiempoEsperaMax);
        }
        break;

    case EstadoAgente::SALTAR:
        if (m_enSuelo) {
            m_estadoActual = EstadoAgente::ESPERAR;
            m_tiempoEsperaActual = randomEntre(m_tiempoEsperaMin, m_tiempoEsperaMax);
        }
        break;

    case EstadoAgente::ESQUIVAR:
        if (!m_esquivando) {
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
        m_tiempoTranscurrido += dt;
        break;

    case EstadoAgente::AVANZAR: {
        float dirAvance = dirAlJugador;
        float r = static_cast<float>(std::rand()) / RAND_MAX;
        if (r < m_perfil.probAvanzarDirOpuesta)
            dirAvance = dirAlJugador;
        mover(dirAvance);
        break;
    }

    case EstadoAgente::ATACAR:
        if (puedeAtacar()) {
            if (m_distanciaAlJugador <= m_distanciaObjetivo + 20.0f)
                atacar(dirAlJugador);
        }
        break;

    case EstadoAgente::RETROCEDER:
        mover(-dirAlJugador);
        break;

    case EstadoAgente::SALTAR:
        if (!m_enSuelo) break;
        saltar();
        break;

    case EstadoAgente::ESQUIVAR:
        if (!m_esquivando)
            esquivar(-dirAlJugador);
        break;
    }
}

void Enemigo::aprender() {
    m_perfil.totalPuntos++;

    Principal* jugador = m_referenciaEstado->getJugador();
    bool jugadorGanoEstaRonda = (m_referenciaEstado->getPuntosJugador() >
                                 m_referenciaEstado->getPuntosAgente());

    for (Accion a : m_historial) {
        if (a == Accion::ATACAR) {
            if (m_distanciaAlJugador < m_distanciaMinima * 1.5f)
                m_perfil.ataquesCortoRango++;
            else
                m_perfil.ataquesMedioRango++;
        } else if (a == Accion::ESQUIVAR) {
            if (jugador->getX() < m_x)
                m_perfil.esquivesIzquierda++;
            else
                m_perfil.esquivesDerecha++;
        } else if (a == Accion::SALTAR) {
            m_perfil.saltos++;
        }
    }

    m_historial.clear();
    m_perfil.recalcularProbabilidades();

    ajustarDificultad(jugadorGanoEstaRonda);
}

void Enemigo::ajustarDificultad(bool jugadorGanoRonda) {
    if (jugadorGanoRonda) {

        m_tiempoEsperaMin = clamp(m_tiempoEsperaMin - 0.08f,
                                  ESPERA_MIN_TOPE, m_tiempoEsperaMax);
        m_tiempoEsperaMax = clamp(m_tiempoEsperaMax - 0.15f,
                                  m_tiempoEsperaMin, ESPERA_MAX_TOPE);

        m_probEsquivar = clamp(m_probEsquivar + 0.07f, PROB_MIN_TOPE, PROB_MAX_TOPE);
        m_probSaltar   = clamp(m_probSaltar   + 0.05f, PROB_MIN_TOPE, PROB_MAX_TOPE);

        m_velEmbestida = clamp(m_velEmbestida + 30.0f,
                               VEL_EMBESTIDA_MIN, VEL_EMBESTIDA_MAX);

        m_distanciaObjetivo = clamp(m_distanciaObjetivo + 15.0f,
                                    DIST_OBJ_MIN, DIST_OBJ_MAX);

    } else {

        m_tiempoEsperaMin = clamp(m_tiempoEsperaMin + 0.08f,
                                  ESPERA_MIN_TOPE, ESPERA_MAX_TOPE);
        m_tiempoEsperaMax = clamp(m_tiempoEsperaMax + 0.15f,
                                  m_tiempoEsperaMin, ESPERA_MAX_TOPE);

        m_probEsquivar = clamp(m_probEsquivar - 0.07f, PROB_MIN_TOPE, PROB_MAX_TOPE);
        m_probSaltar   = clamp(m_probSaltar   - 0.05f, PROB_MIN_TOPE, PROB_MAX_TOPE);

        m_velEmbestida = clamp(m_velEmbestida - 30.0f,
                               VEL_EMBESTIDA_MIN, VEL_EMBESTIDA_MAX);

        m_distanciaObjetivo = clamp(m_distanciaObjetivo - 15.0f,
                                    DIST_OBJ_MIN, DIST_OBJ_MAX);
    }
}

bool Enemigo::estaMoviendose() const {
    return m_estadoActual == EstadoAgente::AVANZAR   ||
           m_estadoActual == EstadoAgente::RETROCEDER ||
           m_estadoActual == EstadoAgente::ESQUIVAR   ||
           m_embistiendo;
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

bool Enemigo::lanzarMoneda(float probabilidad) const {
    float r = static_cast<float>(std::rand()) / RAND_MAX;
    return r < probabilidad;
}
