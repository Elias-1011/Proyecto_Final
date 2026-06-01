#ifndef TEMBLOR_H
#define TEMBLOR_H

class Temblor {
public:
    Temblor(float amplitud       = 20.0f,
            float frecuencia     = 3.0f,
            float duracionTotal  = 2.0f,
            float amortiguamiento = 2.0f);

    // Activa el temblor reiniciando el tiempo interno
    void iniciar();

    // Actualiza el tiempo transcurrido y calcula el offset actual
    // dt: tiempo en segundos desde el último frame
    void actualizar(float dt);

    // Retorna true si el temblor sigue activo
    bool estaActivo() const;

    // Retorna el desplazamiento horizontal actual en píxeles
    float getOffset() const;

private:
    float m_amplitud;
    float m_frecuencia;
    float m_duracionTotal;
    float m_amortiguamiento;

    bool  m_activo;
    float m_tiempoTranscurrido;
    float m_offsetActual;
};

#endif // TEMBLOR_H
