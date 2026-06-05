#include "nivel2.h"
#include <cmath>
#include <cstdlib>

Nivel_2::Nivel_2()
    : m_jugador(nullptr)
    , m_enemigo(nullptr)
    , m_temblor(nullptr)
    , m_tiempoHasteTemblor(8.0f)
    , m_intervaloTemblor(8.0f)
    , m_puntosPrincipal(0)
    , m_puntosEnemigo(0)
    , m_toquesPrincipal(0)
    , m_toquesEnemigo(0)
    , m_puntosParaGanar(3)
    , m_finalizado(false)
    , m_ultimaAccionJugador(Accion::MOVER)
    , m_xMinPlataforma(50.0f)
    , m_xMaxPlataforma(750.0f)
{}

Nivel_2::~Nivel_2() {
    delete m_jugador;
    delete m_enemigo;
    delete m_temblor;
}

void Nivel_2::iniciar() {
    const float Y_SUELO = 500.0f;

    m_jugador        = new Principal(150.0f, Y_SUELO);
    m_jugador->input = &m_input;
    m_enemigo        = new Enemigo(650.0f, Y_SUELO, this);

    m_temblor = new Temblor(18.0f, 21.991f, 2.5f, 2.0f);

    m_puntosPrincipal = 0;
    m_puntosEnemigo   = 0;
    m_finalizado      = false;
    reiniciarRonda();
}

void Nivel_2::actualizar(float dt) {
    if (m_finalizado) return;

    m_jugador->procesarEntrada();
    m_jugador->actualizar(dt);
    m_enemigo->actualizar(dt);

    m_tiempoHasteTemblor -= dt;
    if (m_tiempoHasteTemblor <= 0.0f) {
        m_temblor->iniciar();
        m_tiempoHasteTemblor = m_intervaloTemblor +
                               static_cast<float>(std::rand() % 5);
    }
    if (m_temblor->estaActivo()) {
        m_temblor->actualizar(dt);
        float offset = m_temblor->getOffset();
        m_jugador->aplicarOffsetTemblor(offset);
        m_enemigo->aplicarOffsetTemblor(offset);
    }

    limitarPersonajesEnPlataforma();
    detectarToques();
    gestionarCombate();
}

void Nivel_2::detectarToques() {
    float dist = getDistanciaEntrePersonajes();
    const float RADIO_RECEPTOR = 30.0f;

    if (m_jugador->estaEmbistiendo() && !m_jugador->yaGolpeoEnEstaEmbestida()) {
        bool enemigoInmune = m_enemigo->estaEsquivando() ||
                             (!m_enemigo->estaEnSuelo() && m_enemigo->getVelY() < 0.0f);
        if (!enemigoInmune) {
            float hitbox = m_jugador->getRangoAtaque() / 2.0f + RADIO_RECEPTOR;
            if (dist <= hitbox) {
                m_jugador->marcarGolpe();
                m_toquesPrincipal++;
                float dirRetroceso = (m_jugador->getX() < m_enemigo->getX()) ? 1.0f : -1.0f;
                bool puntoCompleto = m_enemigo->recibirImpacto(dirRetroceso, m_toquesPrincipal);
                if (puntoCompleto) {
                    m_puntosPrincipal++;
                    m_enemigo->aprender();
                    reiniciarRonda();
                }
            }
        }
    }

    if (m_enemigo->estaEmbistiendo() && !m_enemigo->yaGolpeoEnEstaEmbestida()) {
        bool jugadorInmune = m_jugador->estaEsquivando() ||
                             (!m_jugador->estaEnSuelo() && m_jugador->getVelY() < 0.0f);
        if (!jugadorInmune) {
            float hitbox = m_enemigo->getRangoAtaque() / 2.0f + RADIO_RECEPTOR;
            if (dist <= hitbox) {
                m_enemigo->marcarGolpe();
                m_toquesEnemigo++;
                float dirRetroceso = (m_enemigo->getX() < m_jugador->getX()) ? 1.0f : -1.0f;
                bool puntoCompleto = m_jugador->recibirImpacto(dirRetroceso, m_toquesEnemigo);
                if (puntoCompleto) {
                    m_puntosEnemigo++;
                    m_enemigo->aprender();
                    reiniciarRonda();
                }
            }
        }
    }
}

void Nivel_2::reiniciarRonda() {
    m_toquesPrincipal = 0;
    m_toquesEnemigo   = 0;
    if (m_jugador) { m_jugador->reiniciarBarra(); m_jugador->setPosicion(150.0f, 500.0f); }
    if (m_enemigo) { m_enemigo->reiniciarBarra(); m_enemigo->setPosicion(650.0f, 500.0f); }
}

void Nivel_2::gestionarCombate() {
    if (m_puntosPrincipal >= m_puntosParaGanar ||
        m_puntosEnemigo   >= m_puntosParaGanar) {
        m_finalizado = true;
        finalizar();
    }
}

void Nivel_2::finalizar() {}

float Nivel_2::getDistanciaEntrePersonajes() const {
    if (!m_jugador || !m_enemigo) return 0.0f;
    return std::abs(m_jugador->getX() - m_enemigo->getX());
}

Accion Nivel_2::getUltimaAccionJugador() const { return m_ultimaAccionJugador; }

bool Nivel_2::getTemblorActivo() const {
    return m_temblor && m_temblor->estaActivo();
}

void Nivel_2::limitarPersonajesEnPlataforma() {
    if (!m_jugador || !m_enemigo) return;
    if (m_jugador->getX() < m_xMinPlataforma) m_jugador->setX(m_xMinPlataforma);
    if (m_jugador->getX() > m_xMaxPlataforma) m_jugador->setX(m_xMaxPlataforma);
    if (m_enemigo->getX() < m_xMinPlataforma) m_enemigo->setX(m_xMinPlataforma);
    if (m_enemigo->getX() > m_xMaxPlataforma) m_enemigo->setX(m_xMaxPlataforma);
}
