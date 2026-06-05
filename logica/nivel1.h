#ifndef NIVEL1_H
#define NIVEL1_H

#include <vector>
#include "jugador.h"
#include "roca.h"
#include "temblor.h"
#include "dificultad.h"

using namespace std;

class Nivel1 {
public:
    Nivel1(Dificultad* dificultad, short tiempoSegundos = 60);
    ~Nivel1();

    void actualizar(float dt);
    void spawnRoca(float rx, float ry);
    void activarTemblor();
    void tickSegundo();
    void moverJugador(bool arriba, bool abajo,
                      bool izquierda, bool derecha);

    void iniciarAtaque();
    void finalizarAtaque()    { atacando = false;          }
    bool estaAtacando()       const { return atacando;     }
    void registrarImpacto(short idxRoca);
    bool debeTerminar()       const { return pendienteTerminar; }

    bool  nivelTerminado()    const { return !activo;                       }
    bool  fueExitoso()        const { return exito;                         }
    short getTiempo()         const { return tiempoRestante;                }
    short getRocasDestr()     const { return rocasDestruidas;               }
    short getRocasObj()       const { return dif->getRocasObjetivo();       }
    short getMaxSimult()      const { return dif->getMaxRocasSimultaneas(); }
    float getOffsetX()        const { return temblor->getOffsetX();         }
    float getOffsetY()        const { return temblor->getOffsetY();         }
    bool  temblorActivo()     const { return temblor->estaActivo();         }

    bool  getHayImpacto()     const { return hayImpactoNuevo;  }
    float getImpactoX()       const { return ultimoImpactoX;   }
    float getImpactoY()       const { return ultimoImpactoY;   }
    void  resetImpacto()            { hayImpactoNuevo = false; }

    const Jugador&       getJugador() const { return *jugador; }
    const vector<Roca*>& getRocas()   const { return rocas;    }

private:
    Jugador*      jugador;
    vector<Roca*> rocas;
    Temblor*      temblor;
    Dificultad*   dif;

    short tiempoRestante;
    short rocasDestruidas;
    bool  activo;
    bool  exito;
    bool  atacando;
    bool  pendienteTerminar;

    float ultimoImpactoX;
    float ultimoImpactoY;
    bool  hayImpactoNuevo;

    void verificarColisionRocaJugador();
    void limpiarInactivos();
    void terminar();

    static constexpr float RADIO_JUGADOR = 20.0f;
};

#endif
