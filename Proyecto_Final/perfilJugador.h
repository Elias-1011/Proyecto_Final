#ifndef PERFILJUGADOR_H
#define PERFILJUGADOR_H

struct PerfilJugador {
    // Frecuencias de acción global
    int ataquesCortoRango  = 0;  // atacó cuando distancia < umbralCerca
    int ataquesMedioRango  = 0;
    int esquivesIzquierda  = 0;
    int esquivesDerecha    = 0;
    int saltos             = 0;
    int totalPuntos        = 0;  // puntos disputados (para normalizar)

    // Probabilidades ajustadas (recalculadas en cada Aprender)
    float probRetrocederSiCerca   = 0.3f;  // base
    float probAvanzarDirOpuesta   = 0.5f;  // base

    void recalcularProbabilidades() {
        if (totalPuntos == 0) return;

        // Si el jugador ataca frecuentemente de cerca, el agente huye más
        float freqAtaqueCerca = static_cast<float>(ataquesCortoRango) / totalPuntos;
        probRetrocederSiCerca = 0.3f + freqAtaqueCerca * 0.5f;
        if (probRetrocederSiCerca > 0.95f) probRetrocederSiCerca = 0.95f;

        // Si el jugador siempre esquiva en la misma dirección, el agente
        // ajusta su avance para cortarle el paso
        int totalEsquives = esquivesIzquierda + esquivesDerecha;
        if (totalEsquives > 0) {
            float tendenciaIzq = static_cast<float>(esquivesIzquierda) / totalEsquives;
            // Si esquiva más a la izquierda, el agente avanza por la derecha
            probAvanzarDirOpuesta = 0.5f + (tendenciaIzq - 0.5f) * 0.6f;
        }
    }
};

#endif // PERFILJUGADOR_H
