#ifndef DIFICULTADFACIL_H
#define DIFICULTADFACIL_H

#include "dificultad.h"

class DificultadFacil : public Dificultad {
public:
    float getVelCrecimientoRoca()  const override { return 14.0f; }
    float getTamanoMaxRoca()       const override { return 80.0f; }
    int   getIntervaloSpawnMs()    const override { return 2200;  }
    short getRocasObjetivo()       const override { return 10;    }
    short getMaxRocasSimultaneas() const override { return 3;     }
    int   getIntervaloTemblorMs()  const override { return 12000; }
    float getAmplitudTemblor()     const override { return 7.0f;  }
};

#endif
