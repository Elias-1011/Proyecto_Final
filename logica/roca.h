#ifndef ROCA_H
#define ROCA_H

#include "entidad.h"

class Roca : public Entidad {
public:
    Roca(float x, float y, float velCrecimiento, float tamanoMax);
    ~Roca() override;

    void actualizar(float dt) override;

    float getTamano()    const { return tamano;        }
    float getTamanoMax() const { return tamanoMax;     }
    float getRadio()     const { return tamano / 2.0f; }
    bool  fueDestruida() const { return destruida;     }

    // BRUTO: un golpe siempre destruye sin importar tamanio
    void destruir();

private:
    float tamano;
    float tamanoMax;
    float velCrecimiento;
    bool  destruida;
};

#endif
