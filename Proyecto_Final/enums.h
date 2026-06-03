#ifndef ENUMS_H
#define ENUMS_H

enum class EstadoJuego {
    MENU,
    JUGANDO,
    PAUSA,
    GAME_OVER,
    VICTORIA
};

enum class EstadoAgente {
    ESPERAR,
    AVANZAR,
    ATACAR,
    RETROCEDER,
    SALTAR,
    ESQUIVAR
};

enum class Accion {
    ATACAR,
    ESQUIVAR,
    SALTAR,
    MOVER
};

#endif // ENUMS_H
