#ifndef TEMBLOR_H
#define TEMBLOR_H

// Fisica del temblor sismico unificada para ambos niveles.
// Ecuacion: offset(t) = A * sin(omega * t) * e^(-lambda * t)
//
// Nivel 1 usa getOffsetX() y getOffsetY() (con fase Y desplazada)
// Nivel 2 usa getOffset() (equivalente a getOffsetX())

class Temblor {
public:
    Temblor(float A, float omega, float lambda, float duracion);

    void iniciar();
    void actualizar(float dt);

    bool  estaActivo() const { return activo; }

    float getOffsetX() const;
    float getOffsetY() const;
    float getOffset()  const { return getOffsetX(); }

private:
    float A, omega, lambda, duracion;
    float t;
    bool  activo;
};

#endif