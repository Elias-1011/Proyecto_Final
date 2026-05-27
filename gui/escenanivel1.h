#ifndef ESCENANIVEL1_H
#define ESCENANIVEL1_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QKeyEvent>
#include <QList>

#include "nivel1.h"
#include "dificultadfacil.h"
#include "dificultaddificil.h"

class MainWindow;

// Direccion actual del jugador para seleccionar el set de sprites correcto
enum class Direccion { Arriba, Abajo, Izquierda, Derecha };

// Datos de un efecto de impacto temporal
struct EfectoImpacto {
    QGraphicsPixmapItem* item;
    short                framesTick;   // cuantos ticks le quedan visible
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

    // ── Sprites jugador ──────────────────────────────────────────
    // vector<QPixmap> justificado: acceso por indice O(1) para
    // seleccionar frame segun direccion y tick de animacion
    QPixmap sprJugadorArriba[3];
    QPixmap sprJugadorAbajo[3];
    QPixmap sprJugadorIzq[3];
    QPixmap sprJugadorDer[3];
    QPixmap sprAtaque;

    // ── Sprites rocas ────────────────────────────────────────────
    QPixmap sprRocas[5];
    QPixmap sprImpacto;

    // ── Items graficos ───────────────────────────────────────────
    QGraphicsPixmapItem*        itemFondo;
    QGraphicsPixmapItem*        itemJugador;
    QList<QGraphicsPixmapItem*> itemsRocas;
    QList<EfectoImpacto>        impactos;   // efectos temporales activos

    // ── HUD ──────────────────────────────────────────────────────
    QGraphicsTextItem* textoTiempo;
    QGraphicsTextItem* textoRocas;
    QGraphicsTextItem* textoTemblor;

    // ── Estado de animacion jugador ──────────────────────────────
    Direccion dirActual;
    short     frameActual;      // 0, 1 o 2
    short     ticksAnimacion;   // contador de frames para cambiar sprite
    bool      estabaAtacando;
    short     cantRocasAntes;   // para detectar nueva destruccion

    // ── Timers ───────────────────────────────────────────────────
    QTimer* timerLoop;
    QTimer* timerSpawn;
    QTimer* timerTemblor;
    QTimer* timerSegundo;

    qint64 ultimoMs;

    // ── Metodos privados ─────────────────────────────────────────
    void cargarSprites();
    void actualizarJugadorGrafico();
    void sincronizarRocas();
    void actualizarImpactos();
    void actualizarHUD();
    void verificarFin();

    QPixmap* getSpriteSetActual();
    short    getFrameRoca(const Roca* r) const;
};

#endif
