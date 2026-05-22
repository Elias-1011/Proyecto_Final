#ifndef DIFICULTAD_H
#define DIFICULTAD_H


class Dificultad {
public:
    virtual ~Dificultad() {}

    virtual float getVelCrecimientoRoca()  const = 0;
    virtual float getTamanoMaxRoca()       const = 0;
    virtual int   getIntervaloSpawnMs()    const = 0;
    virtual short getRocasObjetivo()       const = 0;
    virtual short getMaxRocasSimultaneas() const = 0;
    virtual int   getIntervaloTemblorMs()  const = 0;
    virtual float getAmplitudTemblor()     const = 0;
};

#endif
