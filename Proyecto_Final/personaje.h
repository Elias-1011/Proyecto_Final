#ifndef PERSONAJE_H
#define PERSONAJE_H
#include "enums.h"
#include <cmath>
class Personaje {
public:
    Personaje(float x, float y);
    virtual ~Personaje() = default;
    virtual void procesarEntrada() {}
    virtual void actualizar(float dt);
    void esquivar(float direccion);
    void saltar();
    void mover(float direccion);
    void atacar(float direccion);
    bool recibirImpacto(float dirRetroceso, int toquesAcumulados);
    void reiniciarBarra();
    void aplicarOffsetTemblor(float offset);
    void setPosicion(float x, float y) { m_x = x; m_y = y; m_yBase = y; }
    void setX(float x) { m_x = x; }
    void setY(float y) { m_y = y; }
    float getX()          const { return m_x; }
    float getY()          const { return m_y; }
    float getVelY()       const { return m_velY; }
    int   getPuntosVida() const { return m_puntosVida; }
    float getRangoAtaque()const { return m_rangoAtaque; }
    bool  estaEnSuelo()     const { return m_enSuelo; }
    bool  puedeAtacar()     const { return m_cooldownRestante <= 0.0f && !m_embistiendo; }
    virtual bool estaMoviendose() const { return std::abs(m_velX) > m_umbralParada || m_embistiendo; }
    bool  estaEmbistiendo()         const { return m_embistiendo; }
    float getDirEmbestida()         const { return m_dirEmbestida; }
    bool  yaGolpeoEnEstaEmbestida() const { return m_yaGolpeo; }
    void  marcarGolpe()                   { m_yaGolpeo = true; }
    bool  estaRecuperando()         const { return m_recuperando; }
    bool  estaEsquivando()          const { return m_esquivando; }

protected:
    float m_x, m_y;
    float m_velX, m_velY;
    bool  m_enSuelo;
    int   m_puntosVida;
    float m_rangoAtaque;
    float m_cooldownRestante;
    bool  m_embistiendo;
    bool  m_recuperando;
    bool  m_yaGolpeo;
    bool  m_esquivando;
    float m_dirEmbestida;
    float m_distEmbestidaRecorrida;
    float m_tiempoRecuperacion;
    float m_velImpulsoEsquive;
    float m_friccion;
    float m_umbralParada;
    float m_velVerticalSalto;
    float m_gravedad;
    float m_yBase;
    float m_velEmbestida;
    float m_distEmbestida;
    float m_cooldownAtaque;
    float m_duracionRecuperacion;
    float m_velBaseRetroceso;
    float m_factorEscalaRetroceso;

private:
    void actualizarEmbestida(float dt);
    void actualizarMovimiento(float dt);
    void actualizarSalto(float dt);
};
#endif // PERSONAJE_H
