#ifndef PERSONAJE_H
#define PERSONAJE_H

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

    float getX()          const { return m_x; }
    float getY()          const { return m_y; }
    int   getPuntosVida() const { return m_puntosVida; }
    float getRangoAtaque()const { return m_rangoAtaque; }
    bool  estaEnSuelo()   const { return m_enSuelo; }
    bool  puedeAtacar()   const { return m_cooldownRestante <= 0.0f && !m_embistiendo; }

    bool  estaEmbistiendo()    const { return m_embistiendo; }
    float getDirEmbestida()    const { return m_dirEmbestida; }
    bool  yaGolpeoEnEstaEmbestida() const { return m_yaGolpeo; }
    void  marcarGolpe()              { m_yaGolpeo = true; }
    bool  estaRecuperando()    const { return m_recuperando; }

protected:
    float m_x, m_y;
    float m_velX, m_velY;
    bool  m_enSuelo;

    int   m_puntosVida;
    float m_rangoAtaque;        // hitbox de la embestida (ancho en px)
    float m_cooldownRestante;

    bool  m_embistiendo;        // true mientras avanza la embestida
    bool  m_recuperando;        // true durante el lag post-embestida
    bool  m_yaGolpeo;           // evita registrar el mismo toque varios frames
    float m_dirEmbestida;       // +1 / -1
    float m_distEmbestidaRecorrida; // cuánto ha avanzado ya
    float m_tiempoRecuperacion; // contador de lag restante

    float m_velImpulsoEsquive;
    float m_friccion;
    float m_umbralParada;

    float m_velVerticalSalto;
    float m_gravedad;
    float m_yBase;

    float m_velEmbestida;       // velocidad horizontal de la embestida (px/s)
    float m_distEmbestida;      // distancia fija que recorre la embestida (px)
    float m_cooldownAtaque;     // segundos de recuperación post-embestida
    float m_duracionRecuperacion; // lag de vulnerabilidad tras embestida

    float m_velBaseRetroceso;
    float m_factorEscalaRetroceso;

private:
    void actualizarEmbestida(float dt);
    void actualizarMovimiento(float dt);
    void actualizarSalto(float dt);
};

#endif // PERSONAJE_H
