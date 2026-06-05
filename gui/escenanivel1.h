#ifndef ESCENANIVEL1_H
#define ESCENANIVEL1_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QKeyEvent>
#include <QList>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "nivel1.h"
#include "dificultadfacil.h"
#include "dificultaddificil.h"

class MainWindow;

enum class Direccion { Arriba, Abajo, Izquierda, Derecha };

struct EfectoImpacto {
    QGraphicsPixmapItem* item;
    short                framesTick;
};

class EscenaNivel1 : public QGraphicsScene {
    Q_OBJECT

public:
    EscenaNivel1(MainWindow* ventana, bool esDificil);
    ~EscenaNivel1() override;

protected:
    void keyPressEvent  (QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent* e) override;

private slots:
    void actualizarFrame();
    void spawnRocaAleatoria();
    void dispararTemblor();
    void tickSegundo();

private:
    MainWindow* ventana;
    Nivel1*     nivel;
    Dificultad* dif;

    QPixmap sprJugadorArriba[3];
    QPixmap sprJugadorAbajo[3];
    QPixmap sprJugadorIzq[3];
    QPixmap sprJugadorDer[3];
    QPixmap sprAtaque[4];
    QPixmap sprRocas[5];
    QPixmap sprImpacto;

    bool teclas[4];

    QGraphicsPixmapItem*        itemFondo;
    QGraphicsPixmapItem*        itemJugador;
    QList<QGraphicsPixmapItem*> itemsRocas;
    QList<short>                frameRocaAnterior;
    QList<EfectoImpacto>        impactos;

    QGraphicsTextItem* textoTiempo;
    QGraphicsTextItem* textoRocas;
    QGraphicsTextItem* textoVidas;
    QGraphicsTextItem* textoTemblor;

    QMediaPlayer* musicaNivel;
    QAudioOutput* audioNivel;
    QMediaPlayer* sonidoTemblor;
    QAudioOutput* audioTemblor;
    QMediaPlayer* sonidoAtaque;
    QAudioOutput* audioAtaque;
    QMediaPlayer* sonidoImpacto;
    QAudioOutput* audioImpacto;
    QMediaPlayer* sonidoDanio;
    QAudioOutput* audioDanio;

    bool temblorSonando;


    Direccion dirActual;
    short     frameActual;
    short     ticksAnimacion;
    short     ticksAtaqueVisible;

    QTimer* timerLoop;
    QTimer* timerSpawn;
    QTimer* timerTemblor;
    QTimer* timerSegundo;
    QTimer* timerCooldownAtaque;

    bool   puedeAtacar;
    qint64 ultimoMs;

    short hudTiempoAnterior;
    short hudRocasAnterior;
    short hudVidasAnterior;
    bool  hudTemborAnterior;

    void cargarSprites();
    void actualizarJugadorGrafico();
    void verificarColisionAtaqueRoca();
    void sincronizarRocas();
    void actualizarImpactos();
    void actualizarHUD();
    void verificarFin();

    QPixmap* getSpriteSetActual();
    short    getFrameRoca(const Roca* r) const;
    short    getIdxDireccion()           const;

    static constexpr short TAM_JUGADOR     = 64;
    static constexpr short TAM_ATAQUE      = 110;
    static constexpr short TAM_ROCA_BASE   = 20;
    static constexpr short TAM_ROCA_MAX    = 80;
    static constexpr short TICKS_POR_FRAME = 8;
    static constexpr short TICKS_IMPACTO   = 18;
    static constexpr short TICKS_ATAQUE    = 12;
    static constexpr float RANGO_ATAQUE    = 70.0f;
};

#endif
