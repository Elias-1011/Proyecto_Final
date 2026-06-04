#ifndef NIVEL2_H
#define NIVEL2_H

#include "enums.h"
#include "temblor.h"
#include "principal.h"
#include "enemigo.h"

class Nivel_2 {
public:
    Nivel_2();
    ~Nivel_2();

    void iniciar();
    void actualizar(float dt);
    void finalizar();
    void detectarToques();
    void reiniciarRonda();
    void gestionarCombate();

    float        getDistanciaEntrePersonajes() const;
    Accion       getUltimaAccionJugador()      const;
    bool         getTemblorActivo()            const;
    int          getPuntosAgente()             const { return m_puntosEnemigo; }
    int          getPuntosJugador()            const { return m_puntosPrincipal; }
    int          getToquesAgente()             const { return m_toquesEnemigo; }
    int          getToquesJugador()            const { return m_toquesPrincipal; }
    Principal*   getJugador()                  const { return m_jugador; }
    Enemigo* getEnemigo()                 const { return m_enemigo; }

    bool nivelFinalizado() const { return m_finalizado; }
    bool jugadorGano()     const { return m_puntosPrincipal >= m_puntosParaGanar; }

    InputManager& getInput() { return m_input; }

    void registrarAccionJugador(Accion a) { m_ultimaAccionJugador = a; }

private:
    Principal*    m_jugador;
    Enemigo* m_enemigo;

    Temblor* m_temblor;
    float    m_tiempoHasteTemblor;   // segundos hasta el próximo temblor
    float    m_intervaloTemblor;     // segundos entre temblores

    int   m_puntosPrincipal;
    int   m_puntosEnemigo;
    int   m_toquesPrincipal;   // toques del jugador sobre el enemigo en el punto actual
    int   m_toquesEnemigo;     // toques del enemigo sobre el jugador en el punto actual
    int   m_puntosParaGanar;

    bool  m_finalizado;
    Accion m_ultimaAccionJugador;

    InputManager m_input;

    float m_xMinPlataforma;
    float m_xMaxPlataforma;

    void limitarPersonajesEnPlataforma();
};

#endif // NIVEL2_H
