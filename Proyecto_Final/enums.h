#ifndef ENUMS_H
#define ENUMS_H

// Estados del juego principal
enum class EstadoJuego {
    MENU,
    JUGANDO,
    PAUSA,
    GAME_OVER,
    VICTORIA
};

// Estados de la máquina de estados del agente autónomo
enum class EstadoAgente {
    ESPERAR,
    AVANZAR,
    ATACAR,
    RETROCEDER
};

// Acciones que puede ejecutar un personaje (usadas en historial del agente)
enum class Accion {
    ATACAR,
    ESQUIVAR,
    SALTAR,
    MOVER
};

#endif // ENUMS_H
