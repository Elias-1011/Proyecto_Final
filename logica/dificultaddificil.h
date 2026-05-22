#ifndef DIFICULTADDIFICIL_H
#define DIFICULTADDIFICIL_H

#include "dificultad.h"

class DificultadDificil : public Dificultad {
public:
    float getVelCrecimientoRoca()  const override { return 26.0f; }
    float getTamanoMaxRoca()       const override { return 80.0f; }
    int   getIntervaloSpawnMs()    const override { return 1200;  }
    short getRocasObjetivo()       const override { return 18;    }
    short getMaxRocasSimultaneas() const override { return 6;     }
    int   getIntervaloTemblorMs()  const override { return 6000;  }
    float getAmplitudTemblor()     const override { return 16.0f; }
};

#endif
