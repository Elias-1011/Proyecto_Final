#ifndef TEMBLOR_H
#define TEMBLOR_H


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
