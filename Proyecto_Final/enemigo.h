#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "personaje.h"
#include "perfilJugador.h"
#include "enums.h"
#include <vector>

class Nivel_2;

class Enemigo : public Personaje {
public:
    Enemigo(float x, float y, Nivel_2* referenciaEstado);

    void percibir();
    void razonar();
    void actuar(float dt);
    void aprender();
    void actualizar(float dt) override;

    EstadoAgente getEstado() const { return m_estadoActual; }

private:
    Nivel_2* m_referenciaEstado;

    EstadoAgente         m_estadoActual;
    std::vector<Accion>  m_historial;      // acciones del jugador en el punto actual
    PerfilJugador        m_perfil;

    float  m_distanciaAlJugador;
    Accion m_ultimaAccionJugador;
    bool   m_temblor_activo;
    int    m_marcadorAgente;
    int    m_marcadorJugador;

    float m_distanciaObjetivo;   // distancia a la que el agente ataca
    float m_distanciaMinima;     // si el jugador está más cerca → RETROCEDER
    float m_tiempoEsperaMin;     // segundos mínimos en estado ESPERAR
    float m_tiempoEsperaMax;     // segundos máximos en estado ESPERAR
    float m_tiempoEsperaActual;  // tiempo restante en ESPERAR
    float m_tiempoTranscurrido;  // acumulador de dt para ESPERAR

    float calcularDireccionAlJugador() const;
    bool  jugadorEstaCerca()           const;
    float randomEntre(float min, float max) const;
};

#endif // ENEMIGO_H
