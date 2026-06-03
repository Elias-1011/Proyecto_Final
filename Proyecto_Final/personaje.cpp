#include "personaje.h"
#include <cmath>

Personaje::Personaje(float x, float y)
    : m_x(x), m_y(y)
    , m_velX(0.0f), m_velY(0.0f)
    , m_enSuelo(true)
    , m_puntosVida(0)
    , m_rangoAtaque(50.0f)
    , m_cooldownRestante(0.0f)
    , m_embistiendo(false)
    , m_recuperando(false)
    , m_yaGolpeo(false)
    , m_esquivando(false)
    , m_dirEmbestida(1.0f)
    , m_distEmbestidaRecorrida(0.0f)
    , m_tiempoRecuperacion(0.0f)
    , m_velImpulsoEsquive(1100.0f)
    , m_friccion(0.85f)
    , m_umbralParada(5.0f)
    , m_velVerticalSalto(-500.0f)
    , m_gravedad(1000.0f)
    , m_yBase(y)
    , m_velEmbestida(550.0f)
    , m_distEmbestida(200.0f)
    , m_cooldownAtaque(1.2f)
    , m_duracionRecuperacion(0.4f)
    , m_velBaseRetroceso(150.0f)
    , m_factorEscalaRetroceso(1.5f)
{}

void Personaje::actualizar(float dt) {
    actualizarEmbestida(dt);
    actualizarSalto(dt);
    if (!m_embistiendo && !m_recuperando)
        actualizarMovimiento(dt);
    if (m_cooldownRestante > 0.0f) {
        m_cooldownRestante -= dt;
        if (m_cooldownRestante < 0.0f) m_cooldownRestante = 0.0f;
    }
}

void Personaje::esquivar(float direccion) {
    if (m_embistiendo || m_recuperando) return;
    m_velX       = m_velImpulsoEsquive * direccion;
    m_esquivando = true;
}

void Personaje::saltar() {
    if (!m_enSuelo || m_embistiendo || m_recuperando) return;
    m_velY    = m_velVerticalSalto;
    m_enSuelo = false;
}

void Personaje::mover(float direccion) {
    if (m_embistiendo || m_recuperando) return;
    if (m_esquivando) return;
    const float VEL_MOV = 180.0f;
    m_velX = VEL_MOV * direccion;
}

void Personaje::atacar(float direccion) {
    if (!puedeAtacar()) return;
    m_embistiendo            = true;
    m_recuperando            = false;
    m_yaGolpeo               = false;
    m_esquivando             = false;
    m_dirEmbestida           = direccion;
    m_distEmbestidaRecorrida = 0.0f;
    m_velX                   = 0.0f;
    m_cooldownRestante       = m_cooldownAtaque;
}

bool Personaje::recibirImpacto(float dirRetroceso, int toquesAcumulados) {
    m_puntosVida++;
    float escala = 1.0f + (toquesAcumulados - 1) * (m_factorEscalaRetroceso - 1.0f);
    m_velX = m_velBaseRetroceso * escala * dirRetroceso;
    if (m_embistiendo) {
        m_embistiendo = false;
        m_recuperando = false;
    }
    m_esquivando = false;
    return (m_puntosVida >= 3);
}

void Personaje::reiniciarBarra() {
    m_puntosVida = 0;
}

void Personaje::aplicarOffsetTemblor(float offset) {
    if (!m_embistiendo) m_x += offset;
}

void Personaje::actualizarEmbestida(float dt) {
    if (m_recuperando) {
        m_tiempoRecuperacion -= dt;
        if (m_tiempoRecuperacion <= 0.0f) {
            m_recuperando        = false;
            m_tiempoRecuperacion = 0.0f;
        }
        return;
    }
    if (!m_embistiendo) return;
    float paso = m_velEmbestida * dt;
    m_x += paso * m_dirEmbestida;
    m_distEmbestidaRecorrida += paso;
    if (m_distEmbestidaRecorrida >= m_distEmbestida) {
        m_embistiendo        = false;
        m_recuperando        = true;
        m_tiempoRecuperacion = m_duracionRecuperacion;
        m_velX               = 0.0f;
    }
}

void Personaje::actualizarMovimiento(float dt) {
    m_x += m_velX * dt;
    if (m_esquivando) {
        if (std::abs(m_velX) > m_umbralParada)
            m_velX *= m_friccion;
        else {
            m_velX       = 0.0f;
            m_esquivando = false;
        }
    } else {
        m_velX = 0.0f;
    }
}

void Personaje::actualizarSalto(float dt) {
    if (m_enSuelo) return;
    m_velY += m_gravedad * dt;
    m_y    += m_velY * dt;
    if (m_y >= m_yBase) {
        m_y       = m_yBase;
        m_velY    = 0.0f;
        m_enSuelo = true;
    }
}
