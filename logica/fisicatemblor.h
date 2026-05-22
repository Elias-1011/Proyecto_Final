#ifndef FISICATEMBLOR_H
#define FISICATEMBLOR_H


class FisicaTemblor {
public:
    FisicaTemblor(float A, float omega, float lambda, float duracion);

    void iniciar();
    void actualizar(float dt);

    bool  estaActivo() const { return activo; }
    float getOffsetX() const;
    float getOffsetY() const;

private:
    float A, omega, lambda, duracion;
    float t;
    bool  activo;
};

#endif
