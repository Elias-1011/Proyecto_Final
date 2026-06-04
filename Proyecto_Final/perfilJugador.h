#ifndef PERFILJUGADOR_H
#define PERFILJUGADOR_H

struct PerfilJugador {

    int ataquesCortoRango  = 0;
    int ataquesMedioRango  = 0;
    int esquivesIzquierda  = 0;
    int esquivesDerecha    = 0;
    int saltos             = 0;
    int totalPuntos        = 0;


    float probRetrocederSiCerca   = 0.3f;
    float probAvanzarDirOpuesta   = 0.5f;

    void recalcularProbabilidades() {
        if (totalPuntos == 0) return;

        float freqAtaqueCerca = static_cast<float>(ataquesCortoRango) / totalPuntos;
        probRetrocederSiCerca = 0.3f + freqAtaqueCerca * 0.5f;
        if (probRetrocederSiCerca > 0.95f) probRetrocederSiCerca = 0.95f;

        int totalEsquives = esquivesIzquierda + esquivesDerecha;
        if (totalEsquives > 0) {
            float tendenciaIzq = static_cast<float>(esquivesIzquierda) / totalEsquives;
            probAvanzarDirOpuesta = 0.5f + (tendenciaIzq - 0.5f) * 0.6f;
        }
    }
};

#endif // PERFILJUGADOR_H
