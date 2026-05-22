#include "fisicatemblor.h"
#include <cmath>
#include <stdexcept>

using namespace std;

FisicaTemblor::FisicaTemblor(float A, float omega, float lambda, float duracion)
    : A(A), omega(omega), lambda(lambda), duracion(duracion),
    t(0.0f), activo(false)
{
    if (A < 0)
        throw invalid_argument("La amplitud no puede ser negativa.");
    if (omega <= 0 || lambda <= 0)
        throw invalid_argument("omega y lambda deben ser positivos.");
}

void FisicaTemblor::iniciar() {
    t      = 0.0f;
    activo = true;
}

void FisicaTemblor::actualizar(float dt) {
    if (!activo) return;
    t += dt;
    if (t >= duracion) activo = false;
}

float FisicaTemblor::getOffsetX() const {
    if (!activo) return 0.0f;
    return A * sin(omega * t) * exp(-lambda * t);
}

float FisicaTemblor::getOffsetY() const {
    if (!activo) return 0.0f;
    // Fase desplazada para que X e Y no sean identicos
    return (A * 0.6f) * sin(omega * t + 1.047f) * exp(-lambda * t);
}
