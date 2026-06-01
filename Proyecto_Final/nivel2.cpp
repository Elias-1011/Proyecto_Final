#include "nivel2.h"

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

// ─────────────────────────────────────────────────────────────────────────────
// Iniciar
// ─────────────────────────────────────────────────────────────────────────────
void Nivel_2::iniciar() {
    // Posiciones iniciales: jugador a la izquierda, agente a la derecha
    const float Y_SUELO = 500.0f;

    m_jugador = new Principal(150.0f, Y_SUELO);
    m_jugador->input = &m_input;

    m_enemigo = new Enemigo(650.0f, Y_SUELO, this);

    // Temblor con parámetros calibrados para el duelo
    m_temblor = new Temblor(/*amplitud*/ 18.0f,
                            /*frecuencia*/ 3.5f,
                            /*duracion*/ 2.0f,
                            /*amortiguamiento*/ 2.5f);

    m_puntosPrincipal = 0;
    m_puntosEnemigo   = 0;
    m_finalizado      = false;
    reiniciarRonda();
}

// ─────────────────────────────────────────────────────────────────────────────
// Actualizar (game loop del nivel)
// ─────────────────────────────────────────────────────────────────────────────
void Nivel_2::actualizar(float dt) {
    if (m_finalizado) return;

    // ── 1. Input del jugador ──────────────────────────────────────────────────
    m_jugador->procesarEntrada();

    // ── 2. Actualizar personajes ──────────────────────────────────────────────
    m_jugador->actualizar(dt);
    m_enemigo->actualizar(dt);   // incluye el ciclo percibir/razonar/actuar

    // ── 3. Temblor sísmico ────────────────────────────────────────────────────
    m_tiempoHasteTemblor -= dt;
    if (m_tiempoHasteTemblor <= 0.0f) {
        m_temblor->iniciar();
        m_tiempoHasteTemblor = m_intervaloTemblor
                               + static_cast<float>(std::rand() % 5); // variación ±5s
    }

    if (m_temblor->estaActivo()) {
        m_temblor->actualizar(dt);
        float offset = m_temblor->getOffset();
        m_jugador->aplicarOffsetTemblor(offset);
        m_enemigo->aplicarOffsetTemblor(offset);
    }

    // ── 4. Limitar personajes a la plataforma ─────────────────────────────────
    limitarPersonajesEnPlataforma();

    // ── 5. Detección de toques ────────────────────────────────────────────────
    detectarToques();

    // ── 6. Gestionar combate (puntos, victoria) ───────────────────────────────
    gestionarCombate();
}

// ─────────────────────────────────────────────────────────────────────────────
// Detección de toques
// ─────────────────────────────────────────────────────────────────────────────
void Nivel_2::detectarToques() {
    const float RADIO_IMPACTO = 30.0f;  // píxeles — ajustar con sprites reales

    // Proyectil del jugador impacta al enemigo
    if (m_jugador->hayProyectilActivo()) {
        float dx = m_jugador->getProyectilX() - m_enemigo->getX();
        float dy = m_jugador->getProyectilY() - m_enemigo->getY();
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist <= RADIO_IMPACTO) {
            m_jugador->desactivarProyectil();
            m_toquesPrincipal++;  // toques del jugador sobre el enemigo

            float dirRetroceso = (m_jugador->getX() < m_enemigo->getX()) ? 1.0f : -1.0f;
            bool puntoCompleto = m_enemigo->recibirImpacto(dirRetroceso, m_toquesPrincipal);

            if (puntoCompleto) {
                m_puntosPrincipal++;
                m_enemigo->aprender();   // el agente aprende al final del punto
                reiniciarRonda();
            }
        }
    }

    // Proyectil del enemigo impacta al jugador
    if (m_enemigo->hayProyectilActivo()) {
        float dx = m_enemigo->getProyectilX() - m_jugador->getX();
        float dy = m_enemigo->getProyectilY() - m_jugador->getY();
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist <= RADIO_IMPACTO) {
            m_enemigo->desactivarProyectil();
            m_toquesEnemigo++;   // toques del enemigo sobre el jugador

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

// ─────────────────────────────────────────────────────────────────────────────
// Reiniciar ronda
// ─────────────────────────────────────────────────────────────────────────────
void Nivel_2::reiniciarRonda() {
    m_toquesPrincipal = 0;
    m_toquesEnemigo   = 0;

    if (m_jugador) m_jugador->reiniciarBarra();
    if (m_enemigo) m_enemigo->reiniciarBarra();
}

// ─────────────────────────────────────────────────────────────────────────────
// Gestionar combate — comprobar condición de victoria
// ─────────────────────────────────────────────────────────────────────────────
void Nivel_2::gestionarCombate() {
    if (m_puntosPrincipal >= m_puntosParaGanar ||
        m_puntosEnemigo   >= m_puntosParaGanar) {
        m_finalizado = true;
        finalizar();
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Finalizar
// ─────────────────────────────────────────────────────────────────────────────
void Nivel_2::finalizar() {
    // En la integración con Qt, aquí se emitirá una señal al objeto Juego
    // para que cambie el estado a VICTORIA o GAME_OVER según jugadorGano().
}

// ─────────────────────────────────────────────────────────────────────────────
// Acceso al estado (para Enemigo::percibir)
// ─────────────────────────────────────────────────────────────────────────────
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

// ─────────────────────────────────────────────────────────────────────────────
// Limitar personajes a la plataforma
// ─────────────────────────────────────────────────────────────────────────────
void Nivel_2::limitarPersonajesEnPlataforma() {
    // Jugador
    if (m_jugador->getX() < m_xMinPlataforma) {
        // Forzar posición: ajuste directo vía mover
        // (en Qt se expondrá un setter; por ahora se compensa con mover)
    }
    // (La restricción completa se implementa con setX en la capa gráfica)
}
