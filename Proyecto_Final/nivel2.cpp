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

    m_jugador = new Principal(150.0f, Y_SUELO);
    m_jugador->input = &m_input;

    m_enemigo = new Enemigo(650.0f, Y_SUELO, this);

    m_temblor = new Temblor(18.0f, 3.5f, 2.0f, 2.5f);

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

    // Temblor periódico
    m_tiempoHasteTemblor -= dt;
    if (m_tiempoHasteTemblor <= 0.0f) {
        m_temblor->iniciar();
        m_tiempoHasteTemblor = m_intervaloTemblor + static_cast<float>(std::rand() % 5);
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

// ─────────────────────────────────────────────────────────────────────────────
// Detección de toques — basada en embestida, no en proyectil
//
// Un toque es válido cuando el personaje que embiste se solapa con el oponente.
// "Solapar" = la distancia entre sus centros es menor que la suma de
// sus medios-hitbox (rangoAtaque / 2 de cada uno).
// ─────────────────────────────────────────────────────────────────────────────
void Nivel_2::detectarToques() {
    float dist = getDistanciaEntrePersonajes();

    // Hitbox combinado: medio hitbox del atacante + radio del receptor (30px fijo)
    const float RADIO_RECEPTOR = 30.0f;

    // ── Jugador embiste al enemigo ────────────────────────────────────────────
    if (m_jugador->estaEmbistiendo() && !m_jugador->yaGolpeoEnEstaEmbestida()) {
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

    // ── Agente embiste al jugador ─────────────────────────────────────────────
    if (m_enemigo->estaEmbistiendo() && !m_enemigo->yaGolpeoEnEstaEmbestida()) {
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

void Nivel_2::reiniciarRonda() {
    m_toquesPrincipal = 0;
    m_toquesEnemigo   = 0;
    if (m_jugador) m_jugador->reiniciarBarra();
    if (m_enemigo) m_enemigo->reiniciarBarra();
}

void Nivel_2::gestionarCombate() {
    if (m_puntosPrincipal >= m_puntosParaGanar ||
        m_puntosEnemigo   >= m_puntosParaGanar) {
        m_finalizado = true;
        finalizar();
    }
}

void Nivel_2::finalizar() {
    // Al integrar Qt: emitir señal al objeto Juego con el resultado
}

float Nivel_2::getDistanciaEntrePersonajes() const {
    if (!m_jugador || !m_enemigo) return 0.0f;
    return std::abs(m_jugador->getX() - m_enemigo->getX());
}

Accion Nivel_2::getUltimaAccionJugador() const {
    return m_ultimaAccionJugador;
}

bool Nivel_2::getTemblorActivo() const {
    return m_temblor && m_temblor->estaActivo();
}

void Nivel_2::limitarPersonajesEnPlataforma() {
    // Se implementa con setX() al integrar Qt
}
