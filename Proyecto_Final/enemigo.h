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
    bool estaMoviendose() const override;
    EstadoAgente getEstado() const { return m_estadoActual; }

private:
    Nivel_2* m_referenciaEstado;
    EstadoAgente         m_estadoActual;
    std::vector<Accion>  m_historial;
    PerfilJugador        m_perfil;
    float  m_distanciaAlJugador;
    Accion m_ultimaAccionJugador;
    bool   m_temblor_activo;
    int    m_marcadorAgente;
    int    m_marcadorJugador;
    float m_distanciaObjetivo;
    float m_distanciaMinima;
    float m_tiempoEsperaMin;
    float m_tiempoEsperaMax;
    float m_tiempoEsperaActual;
    float m_tiempoTranscurrido;

    float m_probSaltar;     // probabilidad de saltar cuando el jugador ataca
    float m_probEsquivar;   // probabilidad de esquivar cuando el jugador ataca

    float calcularDireccionAlJugador() const;
    bool  jugadorEstaCerca()           const;
    float randomEntre(float min, float max) const;
    bool  lanzarMoneda(float probabilidad)  const;
};
#endif // ENEMIGO_H
