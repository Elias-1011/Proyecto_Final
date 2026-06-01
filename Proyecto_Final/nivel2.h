#ifndef NIVEL2_H
#define NIVEL2_H

class Nivel_2 {
public:
    Nivel_2();
    ~Nivel_2();

    void iniciar();
    void actualizar(float dt);
    void finalizar();

    // ── Detección de impactos ─────────────────────────────────────────────────
    void detectarToques();

    // ── Gestión de combate ────────────────────────────────────────────────────
    void reiniciarRonda();
    void gestionarCombate();

    // ── Acceso al estado (usado por AgenteInteli::percibir) ───────────────────
    float        getDistanciaEntrePersonajes() const;
    Accion       getUltimaAccionJugador()      const;
    bool         getTemblorActivo()            const;
    int          getPuntosAgente()             const { return m_puntosAgenteInteli; }
    int          getPuntosJugador()            const { return m_puntosPrincipal; }
    int          getToquesAgente()             const { return m_toquesAgenteInteli; }
    int          getToquesJugador()            const { return m_toquesPrincipal; }
    Principal*   getJugador()                  const { return m_jugador; }
    AgenteInteli* getAgenteInteli()                 const { return m_enemigo; }

    // ── Condición de victoria ─────────────────────────────────────────────────
    bool nivelFinalizado() const { return m_finalizado; }
    bool jugadorGano()     const { return m_puntosPrincipal >= m_puntosParaGanar; }

    // ── InputManager (Nivel_2 lo actualiza con eventos Qt) ────────────────────
    InputManager& getInput() { return m_input; }

    // ── Última acción del jugador (para el agente) ────────────────────────────
    void registrarAccionJugador(Accion a) { m_ultimaAccionJugador = a; }

private:
    // ── Personajes ────────────────────────────────────────────────────────────
    Principal*    m_jugador;
    AgenteInteli* m_enemigo;

    // ── Temblor ───────────────────────────────────────────────────────────────
    Temblor* m_temblor;
    float    m_tiempoHasteTemblor;   // segundos hasta el próximo temblor
    float    m_intervaloTemblor;     // segundos entre temblores

    // ── Marcador ─────────────────────────────────────────────────────────────
    int   m_puntosPrincipal;
    int   m_puntosAgenteInteli;
    int   m_toquesPrincipal;   // toques del jugador sobre el enemigo en el punto actual
    int   m_toquesAgenteInteli;     // toques del enemigo sobre el jugador en el punto actual
    int   m_puntosParaGanar;

    // ── Estado del nivel ──────────────────────────────────────────────────────
    bool  m_finalizado;
    Accion m_ultimaAccionJugador;

    // ── Input ─────────────────────────────────────────────────────────────────
    InputManager m_input;

    // ── Límites de la plataforma ──────────────────────────────────────────────
    float m_xMinPlataforma;
    float m_xMaxPlataforma;

    void limitarPersonajesEnPlataforma();
};

#endif // NIVEL2_H
