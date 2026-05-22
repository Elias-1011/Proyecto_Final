#include "personaje.h"
#include <stdexcept>

using namespace std;

Personaje::Personaje(float x, float y, float velocidad)
    : Entidad(x, y), velocidad(velocidad), vidas(3)
{
    if (velocidad <= 0)
        throw invalid_argument("La velocidad debe ser positiva.");
}

Personaje::~Personaje() {}

void Personaje::perderVida() {
    if (vidas > 0) vidas--;
}
