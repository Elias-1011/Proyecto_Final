#include "temblor.h"
#include <cmath>
#include <stdexcept>

using namespace std;

Temblor::Temblor(float A, float omega, float lambda, float duracion)
    : A(A), omega(omega), lambda(lambda), duracion(duracion),
    t(0.0f), activo(false)
{
    if (A < 0)
        throw invalid_argument("La amplitud no puede ser negativa.");
    if (omega <= 0 || lambda <= 0)
        throw invalid_argument("omega y lambda deben ser positivos.");
}

void Temblor::iniciar() {
    t      = 0.0f;
    activo = true;
}

void Temblor::actualizar(float dt) {
    if (!activo) return;
    t += dt;
    if (t >= duracion) activo = false;
}

float Temblor::getOffsetX() const {
    if (!activo) return 0.0f;
    return A * sin(omega * t) * exp(-lambda * t);
}

float Temblor::getOffsetY() const {
    if (!activo) return 0.0f;
    return (A * 0.6f) * sin(omega * t + 1.047f) * exp(-lambda * t);
}
