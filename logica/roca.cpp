#include "roca.h"
#include <stdexcept>

using namespace std;

Roca::Roca(float x, float y, float velCrecimiento, float tamanoMax)
    : Entidad(x, y),
    tamano(8.0f),
    tamanoMax(tamanoMax),
    velCrecimiento(velCrecimiento),
    destruida(false)
{
    if (velCrecimiento <= 0)
        throw invalid_argument("velCrecimiento debe ser positiva.");
    if (tamanoMax <= 8.0f)
        throw invalid_argument("tamanoMax debe superar el tamanio inicial.");
}

Roca::~Roca() {}

void Roca::actualizar(float dt) {
    if (!activo) return;
    tamano += velCrecimiento * dt;
    if (tamano >= tamanoMax) {
        tamano = tamanoMax;
        desactivar();
    }
}

void Roca::destruir() {
    destruida = true;
    desactivar();
}
