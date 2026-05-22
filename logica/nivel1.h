#ifndef NIVEL1_H
#define NIVEL1_H

#include <vector>
#include "jugador.h"
#include "roca.h"
#include "fisicatemblor.h"
#include "dificultad.h"

using namespace std;

struct Lanza {
    float x, y;
    float dirX, dirY;
    float distRecorrida;
    bool  activa;
};


class Nivel1 {
public:
    //60 juego normal, 15 para sustentacion
    Nivel1(Dificultad* dificultad, short tiempoSegundos = 60);
    ~Nivel1();

    void actualizar(float dt);
    void spawnRoca(float rx, float ry);
    void activarTemblor();
    void tickSegundo();

    void moverJugador(bool arriba, bool abajo,
                      bool izquierda, bool derecha);
    void accionAtacar();

    bool  nivelTerminado() const { return !activo;                      }
    bool  fueExitoso()     const { return exito;                        }
    short getTiempo()      const { return tiempoRestante;               }
    short getRocasDestr()  const { return rocasDestruidas;              }
    short getRocasObj()    const { return dif->getRocasObjetivo();      }
    short getMaxSimult()   const { return dif->getMaxRocasSimultaneas();}
    float getOffsetX()     const { return temblor->getOffsetX();        }
    float getOffsetY()     const { return temblor->getOffsetY();        }

    const Jugador&       getJugador() const { return *jugador; }
    const vector<Roca*>& getRocas()   const { return rocas;    }
    const Lanza&         getLanza()   const { return lanza;    }

private:
    Jugador*       jugador;
    vector<Roca*>  rocas;
    Lanza          lanza;
    FisicaTemblor* temblor;
    Dificultad*    dif;

    short tiempoRestante;
    short rocasDestruidas;
    bool  activo;
    bool  exito;

    void actualizarLanza(float dt);
    float getLanzaPuntaX() const { return lanza.x + lanza.dirX * LONG_LANZA; }
    float getLanzaPuntaY() const { return lanza.y + lanza.dirY * LONG_LANZA; }

    void verificarColisiones();
    void limpiarInactivos();
    void terminar();

    static constexpr float LONG_LANZA     = 42.0f;
    static constexpr float VEL_LANZA      = 480.0f;
    static constexpr float DIST_MAX_LANZA = 260.0f;
};

#endif
