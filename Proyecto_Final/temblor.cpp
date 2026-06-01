#include "temblor.h"
#include <cmath>

Temblor::Temblor(float amplitud, float frecuencia, float duracionTotal, float amortiguamiento)
    : m_amplitud(amplitud)
    , m_frecuencia(frecuencia)
    , m_duracionTotal(duracionTotal)
    , m_amortiguamiento(amortiguamiento)
    , m_activo(false)
    , m_tiempoTranscurrido(0.0f)
    , m_offsetActual(0.0f)
{}

void Temblor::iniciar() {
    m_activo = true;
    m_tiempoTranscurrido = 0.0f;
    m_offsetActual = 0.0f;
}

void Temblor::actualizar(float dt) {
    if (!m_activo) return;

    m_tiempoTranscurrido += dt;

    if (m_tiempoTranscurrido >= m_duracionTotal) {
        m_activo = false;
        m_offsetActual = 0.0f;
        return;
    }

    // offset(t) = A * sin(2π * f * t) * e^(-k * t)
    float t = m_tiempoTranscurrido;
    float oscilacion    = std::sin(2.0f * M_PI * m_frecuencia * t);
    float amortiguacion = std::exp(-m_amortiguamiento * t);
    m_offsetActual = m_amplitud * oscilacion * amortiguacion;
}

bool Temblor::estaActivo() const {
    return m_activo;
}

float Temblor::getOffset() const {
    return m_offsetActual;
}
