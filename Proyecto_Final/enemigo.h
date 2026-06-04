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
    void ajustarDificultad(bool jugadorGanoRonda);

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
    float m_probSaltar;
    float m_probEsquivar;

    static constexpr float ESPERA_MIN_TOPE  = 0.1f;   // mínimo absoluto
    static constexpr float ESPERA_MAX_TOPE  = 2.5f;   // máximo absoluto
    static constexpr float PROB_MIN_TOPE    = 0.05f;  // mínimo absoluto
    static constexpr float PROB_MAX_TOPE    = 0.95f;  // máximo absoluto

    static constexpr float VEL_EMBESTIDA_MIN = 400.0f;
    static constexpr float VEL_EMBESTIDA_MAX = 750.0f;

    static constexpr float DIST_OBJ_MIN     = 100.0f;
    static constexpr float DIST_OBJ_MAX     = 250.0f;

    float calcularDireccionAlJugador() const;
    bool  jugadorEstaCerca()           const;
    float randomEntre(float min, float max) const;
    bool  lanzarMoneda(float probabilidad)  const;

    float clamp(float valor, float minVal, float maxVal) const {
        return valor < minVal ? minVal : (valor > maxVal ? maxVal : valor);
    }
};
#endif // ENEMIGO_H
